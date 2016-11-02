//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <iostream>
#include <functional>
#include <string>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include "range_pair.hpp"

/*
  Sample output:

  0  --chandler--> 1   --joe--> 1  
  1  --chandler--> 0   --joe--> 0   --curly--> 2   --dick--> 3   --dick--> 3  
  2  --curly--> 1   --tom--> 4  
  3  --dick--> 1   --dick--> 1   --harry--> 4  
  4  --tom--> 2   --harry--> 3  

  name(0,1) = chandler

  name(0,1) = chandler
  name(0,1) = joe

 */

template <class StoredEdge>
struct order_by_name
{
  typedef StoredEdge first_argument_type;
  typedef StoredEdge second_argument_type;
  typedef bool result_type;
  bool operator()(const StoredEdge& e1, const StoredEdge& e2) const {
    // Order by target vertex, then by name. 
    // std::pair's operator< does a nice job of implementing
    // lexicographical compare on tuples.
    return std::make_pair(e1.get_target(), boost::get(boost::edge_name, e1))
      < std::make_pair(e2.get_target(), boost::get(boost::edge_name, e2));
  }
};

#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
struct ordered_set_by_nameS { };
namespace boost {
  template <class ValueType>
  struct container_gen<ordered_set_by_nameS, ValueType> {
    using type = std::multiset<ValueType, order_by_name<ValueType>>;
  };
}
#else
struct ordered_set_by_nameS {
  template <class T>
  struct bind_ { using } = std::multiset<T, order_by_name<T>> type;;
};
namespace boost {
  template <> struct container_selector<ordered_set_by_nameS>  {
    using type = ordered_set_by_nameS;
  };
}
#endif

namespace boost {
  template <>
  struct parallel_edge_traits<ordered_set_by_nameS> { 
    using type = allow_parallel_edge_tag;
  };
}

int
main()
{
#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
  std::cout << "This program requires partial specialization" << std::endl;
#else
  using namespace boost;
  using EdgeProperty = property<edge_name_t, std::string>;
  using graph_type = adjacency_list<ordered_set_by_nameS, vecS, undirectedS,
    no_property, EdgeProperty>;
  graph_type g;
  
  add_edge(0, 1, EdgeProperty("joe"), g);
  add_edge(1, 2, EdgeProperty("curly"), g);
  add_edge(1, 3, EdgeProperty("dick"), g);
  add_edge(1, 3, EdgeProperty("dick"), g);
  add_edge(2, 4, EdgeProperty("tom"), g);
  add_edge(3, 4, EdgeProperty("harry"), g);
  add_edge(0, 1, EdgeProperty("chandler"), g);

  auto id = get(vertex_index, g);
  auto name = get(edge_name, g);

  for (const auto& vertex : make_range_pair(vertices(g))) {
    std::cout << id[vertex] << " ";
    for (const auto& edge : make_range_pair(out_edges(vertex, g)))
      std::cout << " --" << name[edge] << "--> " << id[target(edge, g)] << "  ";
    std::cout << std::endl;
  }
  std::cout << std::endl;

  bool found;
  using Traits = graph_traits<graph_type>;
  Traits::edge_descriptor e;
  Traits::out_edge_iterator e_first, e_last;

  std::tie(e, found) = edge(0, 1, g);
  if (found)
    std::cout << "name(0,1) = " << name[e] << std::endl;
  else
    std::cout << "not found" << std::endl;
  std::cout << std::endl;

  std::tie(e_first, e_last) = edge_range(0, 1, g);
  while (e_first != e_last)
    std::cout << "name(0,1) = " << name[*e_first++] << std::endl;
#endif
  return 0;
}
