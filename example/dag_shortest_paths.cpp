//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <boost/graph/dag_shortest_paths.hpp>
#include <boost/graph/adjacency_list.hpp>
#include "range_pair.hpp"

#include <iostream>

// Example from Introduction to Algorithms by Cormen, et all p.537.

// Sample output:
//  r: inifinity
//  s: 0
//  t: 2
//  u: 6
//  v: 5
//  x: 3

int main()
{
  using graph_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, 
    boost::property<boost::vertex_distance_t, int>, boost::property<boost::edge_weight_t, int>>;
  graph_t g(6);
  enum verts { r, s, t, u, v, x };
  char name[] = "rstuvx";
  boost::add_edge(r, s, 5, g);
  boost::add_edge(r, t, 3, g);
  boost::add_edge(s, t, 2, g);
  boost::add_edge(s, u, 6, g);
  boost::add_edge(t, u, 7, g);
  boost::add_edge(t, v, 4, g);
  boost::add_edge(t, x, 2, g);
  boost::add_edge(u, v, -1, g);
  boost::add_edge(u, x, 1, g);
  boost::add_edge(v, x, -2, g);

  auto d_map = boost::get(boost::vertex_distance, g);

#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  // VC++ has trouble with the named-parameter mechanism, so
  // we make a direct call to the underlying implementation function.
  std::vector<boost::default_color_type> color(boost::num_vertices(g));
  std::vector<std::size_t> pred(boost::num_vertices(g));
  boost::default_dijkstra_visitor vis;
  std::less<int> compare;
  boost::closed_plus<int> combine;
  auto w_map = boost::get(boost::edge_weight, g);
  dag_shortest_paths(g, s, d_map, w_map, &color[0], &pred[0], 
     vis, compare, combine, (std::numeric_limits<int>::max)(), 0);
#else
  dag_shortest_paths(g, s, boost::distance_map(d_map));
#endif

  for(const auto& vertex : make_range_pair(boost::vertices(g)))
    if (d_map[vertex] == (std::numeric_limits<int>::max)())
      std::cout << name[vertex] << ": inifinity\n";
    else
      std::cout << name[vertex] << ": " << d_map[vertex] << '\n';
  return 0;
}
