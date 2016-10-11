//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <boost/concept/assert.hpp>
#include <iostream>
#include <fstream>
#include <stack>
#include <map>
#include <boost/lexical_cast.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/reverse_graph.hpp>
#include "range_pair.hpp"

using namespace boost;

template <typename OutputIterator> 
class back_edge_recorder : public default_dfs_visitor
{
public:
  back_edge_recorder(OutputIterator out):m_out(out) { }

  template <typename Edge, typename Graph>
  void back_edge(Edge e, const Graph &)
  {
    *m_out++ = e;
  }
private:
  OutputIterator m_out;
};

// object generator function
template <typename OutputIterator>
back_edge_recorder<OutputIterator>
make_back_edge_recorder(OutputIterator out)
{
  return back_edge_recorder<OutputIterator> (out);
}

template <typename Graph, typename Loops> void
find_loops(typename graph_traits<Graph>::vertex_descriptor entry, 
           const Graph & g, 
           Loops & loops)    // A container of sets of vertices
{
  BOOST_CONCEPT_ASSERT(( BidirectionalGraphConcept<Graph> ));
  using Edge = typename graph_traits<Graph>::edge_descriptor;
  std::vector<Edge> back_edges;
  std::vector<default_color_type> color_map(num_vertices(g));
  depth_first_visit(g, entry,
                    make_back_edge_recorder(std::back_inserter(back_edges)),
                    boost::make_iterator_property_map(color_map.begin(),
                                               boost::get(vertex_index, g), color_map[0]));

  for (typename std::vector<Edge>::size_type i = 0; i < back_edges.size(); ++i) {
    typename Loops::value_type x;
    loops.emplace_back(x);
    compute_loop_extent(back_edges[i], g, loops.back());
  }
}

template <typename Graph, typename Set> void
compute_loop_extent(typename graph_traits <
                    Graph >::edge_descriptor back_edge, const Graph & g,
                    Set & loop_set)
{
  BOOST_CONCEPT_ASSERT(( BidirectionalGraphConcept<Graph> ));
  using Vertex = typename graph_traits<Graph>::vertex_descriptor;
  using Color = color_traits<default_color_type>;

  Vertex loop_head, loop_tail;
  loop_tail = source(back_edge, g);
  loop_head = target(back_edge, g);

  std::vector<default_color_type>
    reachable_from_head(num_vertices(g), Color::white());
  default_color_type c;
  depth_first_visit(g, loop_head, default_dfs_visitor(),
                    boost::make_iterator_property_map(reachable_from_head.begin(),
                                               boost::get(vertex_index, g), c));

  std::vector<default_color_type> reachable_to_tail(num_vertices(g));
  reverse_graph<Graph> reverse_g(g);
  depth_first_visit(reverse_g, loop_tail, default_dfs_visitor(),
                    boost::make_iterator_property_map(reachable_to_tail.begin(),
                                               boost::get(vertex_index, g), c));

  typename graph_traits<Graph>::vertex_iterator vi, vi_end;
  for (std::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
    if (reachable_from_head[*vi] != Color::white()
        && reachable_to_tail[*vi] != Color::white())
      loop_set.insert(*vi);
}


int
main(int argc, char *argv[])
{
  if (argc < 3) {
    std::cerr << "usage: loops_dfs <in-file> <out-file>" << std::endl;
    return -1;
  }
  GraphvizDigraph g_in;
  read_graphviz(argv[1], g_in);

  using Graph = adjacency_list < vecS, vecS, bidirectionalS,
    GraphvizVertexProperty,
    GraphvizEdgeProperty, GraphvizGraphProperty >;
  using Vertex = graph_traits<Graph>::vertex_descriptor;

  Graph g;
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  // VC++ has trouble with the boost::get_property() function
  boost::get_property(g, graph_name) = "loops";
#endif

  copy_graph(g_in, g);

  using set_t = std::set<Vertex>;
  using list_of_sets_t = std::list<set_t>;
  list_of_sets_t loops;
  auto entry = *vertices(g).first;

  find_loops(entry, g, loops);

  auto vattr_map = boost::get(vertex_attribute, g);
  auto eattr_map = boost::get(edge_attribute, g);

  for (auto i = loops.begin(); i != loops.end(); ++i) {
    std::vector<bool> in_loop(num_vertices(g), false);
    for (auto j = (*i).begin(); j != (*i).end(); ++j) {
      vattr_map[*j]["color"] = "gray";
      in_loop[*j] = true;
    }
    for (const auto& edge : make_range_pair(edges(g)))
      if (in_loop[source(edge, g)] && in_loop[target(edge, g)])
        eattr_map[edge]["color"] = "gray";
  }

  std::ofstream loops_out(argv[2]);
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  // VC++ has trouble with the boost::get_property() functions
  loops_out << "digraph loops {\n"
            << "size=\"3,3\"\n"
            << "ratio=\"fill\"\n"
            << "shape=\"box\"\n";
  graph_traits<Graph>::vertex_iterator vi, vi_end;
  for (std::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    loops_out << *vi << "[";
    for (auto ai = vattr_map[*vi].begin();
         ai != vattr_map[*vi].end(); ++ai) {
      loops_out << ai->first << "=" << ai->second;
      if (next(ai) != vattr_map[*vi].end())
        loops_out << ", ";
    }
    loops_out<< "]";
  }

  for (const auto& edge : make_range_pair(edges(g))) {
    loops_out << source(edge, g) << " -> " << target(edge, g) << "[";
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
  boost::get_property(g, graph_graph_attribute)["size"] = "3,3";
  boost::get_property(g, graph_graph_attribute)["ratio"] = "fill";
  boost::get_property(g, graph_vertex_attribute)["shape"] = "box";

  write_graphviz(loops_out, g,
                 make_vertex_attributes_writer(g),
                 make_edge_attributes_writer(g),
                 make_graph_attributes_writer(g));
#endif
  return 0;
}
