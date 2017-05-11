//=======================================================================
// Copyright 2007 Aaron Windsor
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/ref.hpp>
#include <vector>

#include <boost/graph/planar_canonical_ordering.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include "range_pair.hpp"

int main(int argc, char** argv)
{

  using graph = boost::adjacency_list<boost::vecS, boost::vecS,
    boost::undirectedS,
    boost::property<boost::vertex_index_t, int>,
    boost::property<boost::edge_index_t, int>>;

  // Create a maximal planar graph on 6 vertices
  graph g(6);

  boost::add_edge(0,1,g);
  boost::add_edge(1,2,g);
  boost::add_edge(2,3,g);
  boost::add_edge(3,4,g);
  boost::add_edge(4,5,g);
  boost::add_edge(5,0,g);

  boost::add_edge(0,2,g);
  boost::add_edge(0,3,g);
  boost::add_edge(0,4,g);

  boost::add_edge(1,3,g);
  boost::add_edge(1,4,g);
  boost::add_edge(1,5,g);

  // Initialize the interior edge index
  auto e_index = boost::get(boost::edge_index, g);
  boost::graph_traits<graph>::edges_size_type edge_count = 0;
  for(const auto& edge : make_range_pair(boost::edges(g)))
    put(e_index, edge, edge_count++);
  

  // Test for planarity - we know it is planar, we just want to 
  // compute the planar embedding as a side-effect
  using vec_t = std::vector<boost::graph_traits<graph>::edge_descriptor>;
  std::vector<vec_t> embedding(boost::num_vertices(g));
  if (boost::boyer_myrvold_planarity_test(boost::boyer_myrvold_params::graph = g,
                                   boost::boyer_myrvold_params::embedding = 
                                       boost::make_iterator_property_map(
                                         embedding.begin(), boost::get(boost::vertex_index, g))
                                   )
      )
    std::cout << "Input graph is planar" << std::endl;
  else
    std::cout << "Input graph is not planar" << std::endl;

  using ordering_storage_t = std::vector<boost::graph_traits<graph>::vertex_descriptor>;
  
  ordering_storage_t ordering;
  planar_canonical_ordering(g,
                            boost::make_iterator_property_map(
                              embedding.begin(), boost::get(boost::vertex_index, g)),
                            std::back_inserter(ordering));

  std::cout << "The planar canonical ordering is: ";
  for(const auto& o : ordering)
    std::cout << o << " ";
  std::cout << std::endl;

  return 0;  
}
