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
#include <boost/graph/graphviz.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
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
    int weight = std::stoi(edge_attr_map[edge]["label"]);
    boost::property<boost::edge_weight_t, int> edge_property(weight);
    boost::add_edge(boost::source(edge, g_dot), boost::target(edge, g_dot), edge_property, g);
  }

  std::vector<boost::graph_traits<Graph>::edge_descriptor> mst;
  using size_type = std::vector<boost::graph_traits<Graph>::edge_descriptor>::size_type;
  kruskal_minimum_spanning_tree(g, std::back_inserter(mst));

  auto weight = boost::get(boost::edge_weight, g);
  int total_weight = 0;
  for (const auto& edge : mst)
    total_weight += boost::get(weight, edge);
  std::cout << "total weight: " << total_weight << std::endl;

  using Vertex = boost::graph_traits<Graph>::vertex_descriptor;
  for (const auto& edge : mst) {
    auto u = boost::source(edge, g), v = boost::target(edge, g);
    edge_attr_map[edge(u, v, g_dot).first]["color"] = "black";
  }
  std::ofstream out("figs/telephone-mst-kruskal.dot");
  auto graph_edge_attr_map = boost::get_priority(g_dot, graph_edge_attribute);
  graph_edge_attr_map["color"] = "gray";
  graph_edge_attr_map["style"] = "bold";
  boost::write_graphviz(out, g_dot);

  return EXIT_SUCCESS;
}
