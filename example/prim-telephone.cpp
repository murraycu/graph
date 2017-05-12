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
#include <vector>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include "range_pair.hpp"

int
main()
{
  GraphvizGraph g_dot;
  boost::read_graphviz("figs/telephone-network.dot", g_dot);

  using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
    boost::property<boost::edge_weight_t, int>>;
  Graph g(boost::num_vertices(g_dot));
  auto edge_attr_map = boost::get(edge_attribute, g_dot);
  for (const auto& edge : make_range_pair(boost::edges(g_dot))) {
    auto weight = std::stoi(edge_attr_map[edge]["label"]);
    boost::property<boost::edge_weight_t, int> edge_property(weight);
    boost::add_edge(boost::source(edge, g_dot), boost::target(edge, g_dot), edge_property, g);
  }

  using Vertex = boost::graph_traits < Graph >::vertex_descriptor;
  std::vector < Vertex > parent(boost::num_vertices(g));
  auto weight = boost::get(boost::edge_weight, g);
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  auto indexmap = boost::get(boost::vertex_index, g);  
  std::vector<std::size_t> distance(boost::num_vertices(g));
  prim_minimum_spanning_tree(g, *vertices(g).first, &parent[0], &distance[0],
                             weight, indexmap, boost::default_dijkstra_visitor());
#else
  prim_minimum_spanning_tree(g, &parent[0]);
#endif

  int total_weight = 0;
  for (std::size_t v = 0; v < num_vertices(g); ++v)
    if (parent[v] != v)
      total_weight += boost::get(weight, edge(parent[v], v, g).first);
  std::cout << "total weight: " << total_weight << std::endl;

  for (std::size_t  u = 0; u < num_vertices(g); ++u)
    if (parent[u] != u)
      edge_attr_map[edge(parent[u], u, g_dot).first]["color"] = "black";
  std::ofstream out("figs/telephone-mst-prim.dot");
  auto& graph_edge_attr_map = boost::get_priority(g_dot, graph_edge_attribute);
  graph_edge_attr_map["color"] = "gray";
  boost::write_graphviz(out, g_dot);

  return EXIT_SUCCESS;
}
