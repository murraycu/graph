//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <iostream>
#include <string>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/read_dimacs.hpp>
#include "range_pair.hpp"

// Use a DIMACS network flow file as stdin.
// push-relabel-eg < max_flow.dat
//
// Sample output:
//  c  The total flow:
//  s 13
//
//  c flow values:
//  f 0 6 3
//  f 0 1 0
//  f 0 2 10
//  f 1 5 1
//  f 1 0 0
//  f 1 3 0
//  f 2 4 4
//  f 2 3 6
//  f 2 0 0
//  f 3 7 5
//  f 3 2 0
//  f 3 1 1
//  f 4 5 4
//  f 4 6 0
//  f 5 4 0
//  f 5 7 5
//  f 6 7 3
//  f 6 4 0
//  f 7 6 0
//  f 7 5 0


int
main()
{
  using namespace boost::graph;
  using Traits = adjacency_list_traits<vecS, vecS, directedS>;
  using Graph = adjacency_list < vecS, vecS, directedS,
    boost::property<vertex_name_t, std::string>,
    boost::property< edge_capacity_t, long,
    boost::property< edge_residual_capacity_t, long,
    boost::property<edge_reverse_t, Traits::edge_descriptor>>>>;
  Graph g;

  auto capacity = boost::get(edge_capacity, g);
  auto residual_capacity = boost::get(edge_residual_capacity, g);
  auto rev = boost::get(edge_reverse, g);
  Traits::vertex_descriptor s, t;
  read_dimacs_max_flow(g, capacity, rev, s, t);

#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  auto indexmap = boost::get(vertex_index, g);
  long flow = push_relabel_max_flow(g, s, t, capacity, residual_capacity, rev,
                                    indexmap);
#else
  long flow = push_relabel_max_flow(g, s, t);
#endif

  std::cout << "c  The total flow:" << std::endl;
  std::cout << "s " << flow << std::endl << std::endl;
  std::cout << "c flow values:" << std::endl;
  graph_traits<Graph>::vertex_iterator u_iter, u_end;
  graph_traits<Graph>::out_edge_iterator ei, e_end;
  for (const auto& vertex : make_range_pair(vertices(g)))
    for (const auto& edge : make_range_pair(out_edges(vertex, g)))
      if (capacity[edge] > 0)
        std::cout << "f " << *u_iter << " " << target(edge, g) << " "
          << (capacity[edge] - residual_capacity[edge]) << std::endl;
  return EXIT_SUCCESS;
}
