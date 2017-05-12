//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>

template <typename T>
  std::istream & operator >> (std::istream & in, std::pair<T, T> &p) {
  in >> p.first >> p.second;
  return in;
}


int
main()
{
  using graph_type = boost::adjacency_list <
    boost::listS,                     // Store out-edges of each vertex in a std::list
    boost::vecS,                      // Store vertex set in a std::vector
    boost::directedS                  // The graph is directed
    >;

  std::ifstream file_in("makefile-dependencies.dat");
  using size_type = boost::graph_traits<graph_type>::vertices_size_type;
  size_type n_vertices;
  file_in >> n_vertices;        // read in number of vertices

  graph_type
  g(n_vertices);                // create graph with n vertices

  // Read in edges
  boost::graph_traits<graph_type>::vertices_size_type u, v;
  while (file_in >> u)
    if (file_in >> v)
      boost::add_edge(u, v, g);
    else
      break;

  assert(boost::num_vertices(g) == 15);
  assert(boost::num_edges(g) == 19);
  return 0;
}
