//=======================================================================
// Copyright 2001 Indiana University.
// Author: Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================


#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <iostream>

enum family { Jeanie, Debbie, Rick, John, Amanda, Margaret, Benjamin, N };

int main()
{
  const char *name[] = { "Jeanie", "Debbie", "Rick", "John", "Amanda",
    "Margaret", "Benjamin"
  };

  boost::adjacency_list <> g(N);
  boost::add_edge(Jeanie, Debbie, g);
  boost::add_edge(Jeanie, Rick, g);
  boost::add_edge(Jeanie, John, g);
  boost::add_edge(Debbie, Amanda, g);
  boost::add_edge(Rick, Margaret, g);
  boost::add_edge(John, Benjamin, g);

  auto index_map = boost::get(boost::vertex_index, g);

  BGL_FORALL_VERTICES(i, g, boost::adjacency_list<>) {
    std::cout << name[get(index_map, i)];

    if (out_degree(i, g) == 0)
      std::cout << " has no children";
    else
      std::cout << " is the parent of ";

    BGL_FORALL_ADJ(i, j, g, boost::adjacency_list<>)
      std::cout << name[get(index_map, j)] << ", ";
    std::cout << std::endl;
  }
  return EXIT_SUCCESS;
}
