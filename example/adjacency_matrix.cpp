//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Author: Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <iostream>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/graph_utility.hpp>


int main()
{
  enum { A, B, C, D, E, F, N };
  const char* name = "ABCDEF";

  // A directed graph
  
  using Graph = boost::adjacency_matrix<boost::directedS>;
  Graph g(N);
  boost::add_edge(B, C, g);
  boost::add_edge(B, F, g);
  boost::add_edge(C, A, g);
  boost::add_edge(C, C, g);
  boost::add_edge(D, E, g);
  boost::add_edge(E, D, g);
  boost::add_edge(F, A, g);

  std::cout << "vertex set: ";
  print_vertices(g, name);
  std::cout << std::endl;

  std::cout << "edge set: ";
  boost::print_edges(g, name);
  std::cout << std::endl;

  std::cout << "out-edges: " << std::endl;
  boost::print_graph(g, name);
  std::cout << std::endl;

  // An undirected graph

  using UGraph = boost::adjacency_matrix<boost::undirectedS>;
  UGraph ug(N);
  boost::add_edge(B, C, ug);
  boost::add_edge(B, F, ug);
  boost::add_edge(C, A, ug);
  boost::add_edge(D, E, ug);
  boost::add_edge(F, A, ug);

  std::cout << "vertex set: ";
  print_vertices(ug, name);
  std::cout << std::endl;

  std::cout << "edge set: ";
  boost::print_edges(ug, name);
  std::cout << std::endl;

  std::cout << "incident edges: " << std::endl;
  boost::print_graph(ug, name);
  std::cout << std::endl;
  return 0;
}
