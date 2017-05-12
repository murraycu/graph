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
#include <algorithm>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>

/*
  Exterior Decorator Basics

  An exterior decorator is a way of associating properties with the
  vertices or edges of a graph. The "exterior" part means that the
  properties are not stored inside the graph object (see
  internal_decorator_basics.cc). Instead they are stored
  separately, and passed as an extra argument to any
  algorithm they are needed in. There are several standard
  decorator types such a color and weight that are used
  in the GGCL algorithms.

  The main responsibility of a decorator is to provide an operator[]
  that maps a vertex (or vertex ID) to the property value for that
  vertex. It just so happens that a normal array provides this.  In
  addition, a decorator must provide access to the property type
  through the decorator_traits class. For convenience, GGCL
  already defines a decorator_triats class for pointer and
  array types.

  Sample Output

  Jeremy owes Rich some money
  Jeremy owes Andrew some money
  Jeremy owes Jeff some money
  Jeremy owes Kinis some money
  Andrew owes Jeremy some money
  Andrew owes Kinis some money
  Jeff owes Jeremy some money
  Jeff owes Rich some money
  Jeff owes Kinis some money
  Kinis owes Jeremy some money
  Kinis owes Rich some money

 */

template <class EdgeIter, class Graph, class Name>
void who_owes_who(EdgeIter first, EdgeIter last, const Graph& G,
                  Name name)
{
  while (first != last) {

    std::cout << name[boost::source(*first,G)] << " owes " 
         << name[boost::target(*first,G)] << " some money" << std::endl;
    ++first;
  }
}

int
main(int, char*[])
{
  /* The property will be "names" attached to the vertices */ 

  std::string* names = new std::string[5];
  names[0] = "Jeremy";
  names[1] = "Rich";
  names[2] = "Andrew";
  names[3] = "Jeff";
  names[4] = "Kinis";
  
  using MyGraphType = boost::adjacency_list<>;

  using Pair = std::pair<int,int>;
  Pair edge_array[] = { Pair(0,1), Pair(0,2), Pair(0,3), Pair(0,4),
                          Pair(2,0), Pair(3,0), Pair(2,4), Pair(3,1), 
                          Pair(3,4), Pair(4,0), Pair(4,1) };

    MyGraphType G(5);
    for (const auto& edge : edge_array)
      boost::add_edge(edge.first, edge.second, G);

  who_owes_who(boost::edges(G).first, edges(G).second, G, names);

  return 0;
}
