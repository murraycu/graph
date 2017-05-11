//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <vector>
#include <list>
#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <iterator>
#include <iostream>
#include "range_pair.hpp"

namespace boost
{
  struct edge_component_t
  {
    enum
    { num = 555 };
    using kind = boost::edge_property_tag;
  }
  edge_component;
}

int
main()
{
  using graph_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
    boost::no_property, boost::property<boost::edge_component_t, std::size_t>>;
  using vertex_t = boost::graph_traits<graph_t>::vertex_descriptor;
  graph_t g(9);
  boost::add_edge(0, 5, g);
  boost::add_edge(0, 1, g);
  boost::add_edge(0, 6, g);
  boost::add_edge(1, 2, g);
  boost::add_edge(1, 3, g);
  boost::add_edge(1, 4, g);
  boost::add_edge(2, 3, g);
  boost::add_edge(4, 5, g);
  boost::add_edge(6, 8, g);
  boost::add_edge(6, 7, g);
  boost::add_edge(7, 8, g);

  auto component = boost::get(boost::edge_component, g);

  auto num_comps = biconnected_components(g, component);
  std::cerr << "Found " << num_comps << " biconnected components.\n";

  std::vector<vertex_t> art_points;
  articulation_points(g, std::back_inserter(art_points));
  std::cerr << "Found " << art_points.size() << " articulation points.\n";

  std::cout << "graph A {\n" << "  node[shape=\"circle\"]\n";

  for (std::size_t i = 0; i < art_points.size(); ++i) {
    std::cout << (char)(art_points[i] + 'A') 
              << " [ style=\"filled\", fillcolor=\"red\" ];" 
              << std::endl;
  }

  for(const auto& edge : make_range_pair(boost::edges(g)))
    std::cout << (char)(boost::source(edge, g) + 'A') << " -- " 
              << (char)(boost::target(edge, g) + 'A')
              << "[label=\"" << component[edge] << "\"]\n";
  std::cout << "}\n";

  return 0;
}
