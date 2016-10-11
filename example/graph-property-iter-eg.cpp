//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <boost/config.hpp>
#include <string>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/property_iter_range.hpp>

int
main()
{
  using namespace boost;
  using graph_t = adjacency_list<listS, vecS, directedS,
    boost::property<vertex_name_t, std::string>>;
  graph_t g(3);

  const char *vertex_names[] = { "Kubrick", "Clark", "Hal" };
  int i = 0;
  graph_property_iter_range<graph_t, vertex_name_t>::iterator v, v_end;
  for (std::tie(v, v_end) = boost::get_property_iter_range(g, vertex_name);
       v != v_end; ++v, ++i)
    *v = vertex_names[i];

  std::tie(v, v_end) = boost::get_property_iter_range(g, vertex_name);
  std::copy(v, v_end, std::ostream_iterator<std::string> (std::cout, " "));
  std::cout << std::endl;
  return 0;
}
