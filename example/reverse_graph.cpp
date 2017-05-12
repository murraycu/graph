//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>

#include <algorithm>
#include <vector>
#include <utility>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/reverse_graph.hpp>
#include <boost/graph/graph_utility.hpp>

int
main()
{
  using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS>;

  Graph G(5);
  boost::add_edge(0, 2, G);
  boost::add_edge(1, 1, G);
  boost::add_edge(1, 3, G);
  boost::add_edge(1, 4, G);
  boost::add_edge(2, 1, G);
  boost::add_edge(2, 3, G);
  boost::add_edge(2, 4, G);
  boost::add_edge(3, 1, G);
  boost::add_edge(3, 4, G);
  boost::add_edge(4, 0, G);
  boost::add_edge(4, 1, G);

  std::cout << "original graph:" << std::endl;
  boost::print_graph(G, boost::get(boost::vertex_index, G));


  std::cout << std::endl << "reversed graph:" << std::endl;
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300  // avoid VC++ bug...
  reverse_graph<Graph> R(G);
  boost::print_graph(R, boost::get(boost::vertex_index, G));
#else
  boost::print_graph(boost::make_reverse_graph(G), boost::get(boost::vertex_index, G));
#endif

  return EXIT_SUCCESS;
}
