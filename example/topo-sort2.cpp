//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <vector>
#include <deque>
#include <iostream>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/adjacency_list.hpp>
int
main()
{
  const char *tasks[] = {
    "pick up kids from school",
    "buy groceries (and snacks)",
    "get cash at ATM",
    "drop off kids at soccer practice",
    "cook dinner",
    "pick up kids from soccer",
    "eat dinner"
  };
  const int n_tasks = sizeof(tasks) / sizeof(char *);

  boost::adjacency_list<boost::listS, boost::vecS, boost::directedS> g(n_tasks);

  boost::add_edge(0, 3, g);
  boost::add_edge(1, 3, g);
  boost::add_edge(1, 4, g);
  boost::add_edge(2, 1, g);
  boost::add_edge(3, 5, g);
  boost::add_edge(4, 6, g);
  boost::add_edge(5, 6, g);

  std::deque<int> topo_order;

  boost::topological_sort(g, std::front_inserter(topo_order),
                   boost::vertex_index_map(boost::identity_property_map()));

  for (const auto& vertex : topo_order)
    std::cout << tasks[vertex] << std::endl;

  return EXIT_SUCCESS;
}
