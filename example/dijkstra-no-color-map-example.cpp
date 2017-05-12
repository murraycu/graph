//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee
// Copyright 2009 Trustees of Indiana University.
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// NOTE: Based off of dijkstra-example.cpp
//=======================================================================
#include <boost/config.hpp>
#include <iostream>
#include <fstream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths_no_color_map.hpp>
#include <boost/property_map/property_map.hpp>
#include "range_pair.hpp"

int
main(int, char *[])
{
  using graph_t = boost::adjacency_list<boost::listS, boost::vecS, boost::directedS,
    boost::no_property, boost::property<boost::edge_weight_t, int>>;
  using vertex_descriptor = boost::graph_traits<graph_t>::vertex_descriptor;
  using Edge = std::pair<int, int>;

  const int num_nodes = 5;
  enum nodes { A, B, C, D, E };
  char name[] = "ABCDE";
  Edge edge_array[] = { Edge(A, C), Edge(B, B), Edge(B, D), Edge(B, E),
    Edge(C, B), Edge(C, D), Edge(D, E), Edge(E, A), Edge(E, B)
  };
  int weights[] = { 1, 2, 1, 2, 7, 3, 1, 1, 1 };
  int num_arcs = sizeof(edge_array) / sizeof(Edge);
  graph_t g(edge_array, edge_array + num_arcs, weights, num_nodes);
  auto weightmap = boost::get(boost::edge_weight, g);
  std::vector<vertex_descriptor> p(boost::num_vertices(g));
  std::vector<int> d(boost::num_vertices(g));
  auto s = vertex(A, g);

  boost::dijkstra_shortest_paths_no_color_map(g, s,
                                       boost::predecessor_map(boost::make_iterator_property_map(p.begin(), boost::get(boost::vertex_index, g))).
                                       distance_map(boost::make_iterator_property_map(d.begin(), boost::get(boost::vertex_index, g))));

  std::cout << "distances and parents:" << std::endl;
  for(const auto& vertex : make_range_pair(boost::vertices(g))) {
    std::cout << "distance(" << name[vertex] << ") = " << d[vertex] << ", ";
    std::cout << "parent(" << name[vertex] << ") = " << name[p[vertex]] << std::endl;
  }
  std::cout << std::endl;

  std::ofstream dot_file("figs/dijkstra-no-color-map-eg.dot");

  dot_file << "digraph D {\n"
    << "  rankdir=LR\n"
    << "  size=\"4,3\"\n"
    << "  ratio=\"fill\"\n"
    << "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";

  for (const auto& e : make_range_pair(boost::edges(g))) {
    auto u = boost::source(e, g), v = boost::target(e, g);
    dot_file << name[u] << " -> " << name[v]
      << "[label=\"" << boost::get(weightmap, e) << "\"";
    if (p[v] == u)
      dot_file << ", color=\"black\"";
    else
      dot_file << ", color=\"grey\"";
    dot_file << "]";
  }
  dot_file << "}";
  return EXIT_SUCCESS;
}
