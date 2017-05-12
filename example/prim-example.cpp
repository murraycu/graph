//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

int
main()
{
  using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
    boost::property<boost::vertex_distance_t, int>, boost::property<boost::edge_weight_t, int >>;
  using E = std::pair<int, int>;
  const int num_nodes = 5;
  const auto edges = { E(0, 2), E(1, 3), E(1, 4), E(2, 1), E(2, 3),
    E(3, 4), E(4, 0)
  };
  int weights[] = { 1, 1, 2, 7, 3, 1, 1 };
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  Graph g(num_nodes);
  auto weightmap = boost::get(boost::edge_weight, g); 
  for (std::size_t j = 0; j < sizeof(edges) / sizeof(E); ++j) {
    auto [e, inserted] = boost::add_edge(edges[j].first, edges[j].second, g);
    weightmap[e] = weights[j];
  }
#else
  Graph g(std::begin(edges), std::end(edges), weights, num_nodes);
#endif
  std::vector < boost::graph_traits < Graph >::vertex_descriptor >
    p(boost::num_vertices(g));

#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  auto distance = boost::get(boost::vertex_distance, g);
  auto indexmap = boost::get(boost::vertex_index, g);
  prim_minimum_spanning_tree
    (g, *vertices(g).first, &p[0], distance, weightmap, indexmap, 
     boost::default_dijkstra_visitor());
#else
  prim_minimum_spanning_tree(g, &p[0]);
#endif

  for (std::size_t i = 0; i != p.size(); ++i)
    if (p[i] != i)
      std::cout << "parent[" << i << "] = " << p[i] << std::endl;
    else
      std::cout << "parent[" << i << "] = no parent" << std::endl;

  return EXIT_SUCCESS;
}
