//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <iostream>
#include <boost/graph/transpose_graph.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include "range_pair.hpp"

int
main()
{
  using graph_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,
    boost::property<boost::vertex_name_t, char>>;

  enum { a, b, c, d, e, f, g, N };
  graph_t G(N);
  auto name_map = boost::get(boost::vertex_name, G);
  char name = 'a';
  for (const auto& vertex : make_range_pair(boost::vertices(G)))
    name_map[vertex] = name;

  using E = std::pair<int, int>;
  E edge_array[] = { E(a, c), E(a, d), E(b, a), E(b, d), E(c, f),
    E(d, c), E(d, e), E(d, f), E(e, b), E(e, g), E(f, e), E(f, g)
  };
  for (const auto& edge : edge_array)
    boost::add_edge(edge.first, edge.second, G);

  boost::print_graph(G, name_map);
  std::cout << std::endl;

  graph_t G_T;
  boost::transpose_graph(G, G_T);

  boost::print_graph(G_T, name_map);

  for (const auto& e : make_range_pair(boost::edges(G)))
    assert(edge(boost::target(e, G), boost::source(e, G), G_T).second == true);
  return 0;
}
