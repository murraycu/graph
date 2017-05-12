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

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include "range_pair.hpp"

int
main(int, char *[])
{
  using vertex_descriptor = boost::adjacency_list_traits<boost::listS, boost::listS, 
    boost::directedS>::vertex_descriptor;
  using graph_t = boost::adjacency_list<boost::listS, boost::listS, boost::directedS,
    boost::property<boost::vertex_index_t, int, 
    boost::property<boost::vertex_name_t, char,
    boost::property<boost::vertex_distance_t, int,
    boost::property<boost::vertex_predecessor_t, vertex_descriptor>>>>, 
    boost::property<boost::edge_weight_t, int>>;
  using Edge = std::pair<int, int>;

  const int num_nodes = 5;
  enum nodes { A, B, C, D, E };
  Edge edge_array[] = { Edge(A, C), Edge(B, B), Edge(B, D), Edge(B, E),
    Edge(C, B), Edge(C, D), Edge(D, E), Edge(E, A), Edge(E, B)
  };
  int weights[] = { 1, 2, 1, 2, 7, 3, 1, 1, 1 };
  int num_arcs = sizeof(edge_array) / sizeof(Edge);

  graph_t g(edge_array, edge_array + num_arcs, weights, num_nodes);
  auto weightmap = boost::get(boost::edge_weight, g);

  // Manually intialize the vertex index and name maps
  auto indexmap = boost::get(boost::vertex_index, g);
  auto name = boost::get(boost::vertex_name, g);
  int c = 0;
  for (const auto& vertex : make_range_pair(boost::vertices(g))) {
    indexmap[vertex] = c;
    name[vertex] = 'A' + c;
    ++c;
  }

  auto s = vertex(A, g);

  auto d = boost::get(boost::vertex_distance, g);
  auto p = boost::get(boost::vertex_predecessor, g);
  boost::dijkstra_shortest_paths(g, s, boost::predecessor_map(p).distance_map(d));

  std::cout << "distances and parents:" << std::endl;
  for(const auto& vertex : make_range_pair(boost::vertices(g))) {
    std::cout << "distance(" << name[vertex] << ") = " << d[vertex] << ", ";
    std::cout << "parent(" << name[vertex] << ") = " << name[p[vertex]] << std::endl;
  }
  std::cout << std::endl;

  std::ofstream dot_file("figs/dijkstra-eg.dot");
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
