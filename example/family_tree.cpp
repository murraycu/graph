//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>
#include "range_pair.hpp"

enum family
{ Jeanie, Debbie, Rick, John, Amanda, Margaret, Benjamin, N };
int
main()
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

  for (const auto& vertex : make_range_pair(boost::vertices(g))) {
    std::cout << name[get(index_map, vertex)];
    auto [ai, a_end] = boost::adjacent_vertices(vertex, g);
    if (ai == a_end)
      std::cout << " has no children";
    else
      std::cout << " is the parent of ";
    for (; ai != a_end; ++ai) {
      std::cout << name[get(index_map, *ai)];
      if (boost::next(ai) != a_end)
        std::cout << ", ";
    }
    std::cout << std::endl;
  }
  return EXIT_SUCCESS;
}
