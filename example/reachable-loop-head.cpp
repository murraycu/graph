//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <iostream>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/copy.hpp>
#include "range_pair.hpp"

int
main(int argc, char *argv[])
{
  if (argc < 3) {
    std::cerr << "usage: reachable-loop-head.exe <in-file> <out-file>"
      << std::endl;
    return -1;
  }
  GraphvizDigraph g;
  boost::read_graphviz(argv[1], g);
  boost::graph_traits<GraphvizDigraph>::vertex_descriptor loop_head = 1;
  using Color = boost::color_traits<boost::default_color_type>;

  std::vector<boost::default_color_type>
    reachable_from_head(boost::num_vertices(g), Color::white());
  boost::default_color_type c;
  boost::depth_first_visit(g, loop_head, boost::default_dfs_visitor(),
                    boost::make_iterator_property_map(reachable_from_head.begin(),
                                               boost::get(boost::vertex_index, g), c));

  auto vattr_map = boost::get(vertex_attribute, g);

  for (const auto& vertex : make_range_pair(boost::vertices(g)))
    if (reachable_from_head[vertex] != Color::white()) {
      vattr_map[vertex]["color"] = "gray";
      vattr_map[vertex]["style"] = "filled";
    }

  std::ofstream loops_out(argv[2]);
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  // VC++ has trouble with the get_property() functions
  loops_out << "digraph G  {\n"
            << "size=\"3,3\"\n"
            << "ratio=\"fill\"\n"
            << "shape=\"box\"\n";
  for (const auto& vertex : make_range_pair(boost::vertices(g))) {
    loops_out << vertex << "[";
    for (auto ai = vattr_map[vertex].begin();
         ai != vattr_map[vertex].end(); ++ai) {
      loops_out << ai->first << "=" << ai->second;
      if (next(ai) != vattr_map[vertex].end())
        loops_out << ", ";
    }
    loops_out<< "]";
  }
  auto eattr_map = boost::get(edge_attribute, g);
  for (const auto& edge : make_range_pair(boost::edges(g))) {
    loops_out << boost::source(edge, g) << " -> " << boost::target(edge, g) << "[";
    auto& attr_map = eattr_map[edge];
    for (auto eai = attr_map.begin();
         eai != attr_map.end(); ++eai) {
      loops_out << eai->first << "=" << eai->second;
      if (next(eai) != attr_map.end())
        loops_out << ", ";
    }
    loops_out<< "]";
  }
  loops_out << "}\n";
#else
  boost::get_priority(g, graph_graph_attribute)["size"] = "3,3";
  boost::get_priority(g, graph_graph_attribute)["ratio"] = "fill";
  boost::get_priority(g, graph_vertex_attribute)["shape"] = "box";

  boost::write_graphviz(loops_out, g,
                 boost::make_vertex_attributes_writer(g),
                 boost::make_edge_attributes_writer(g),
                 boost::make_graph_attributes_writer(g));
#endif


  return EXIT_SUCCESS;
}
