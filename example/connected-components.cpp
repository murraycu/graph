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
#include <boost/graph/connected_components.hpp>
#include <boost/graph/adjacency_list.hpp>

int
main()
{
  using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;

  const int N = 6;
  Graph G(N);
  boost::add_edge(0, 1, G);
  boost::add_edge(1, 4, G);
  boost::add_edge(4, 0, G);
  boost::add_edge(2, 5, G);

  std::vector<int> c(boost::num_vertices(G));
  auto num = boost::connected_components
    (G, boost::make_iterator_property_map(c.begin(), boost::get(boost::vertex_index, G), c[0]));

  std::cout << std::endl;
  std::cout << "Total number of components: " << num << std::endl;
  for (auto i = c.begin(); i != c.end(); ++i)
    std::cout << "Vertex " << i - c.begin()
      << " is in component " << *i << std::endl;
  std::cout << std::endl;
  return EXIT_SUCCESS;
}
