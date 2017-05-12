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

#include <boost/graph/planar_face_traversal.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include "range_pair.hpp"

// Some planar face traversal visitors that will 
// print the vertices and edges on the faces

struct output_visitor : public boost::planar_face_traversal_visitor
{
  void begin_face() { std::cout << "New face: "; }
  void end_face() { std::cout << std::endl; }
};



struct vertex_output_visitor : public output_visitor
{
  template <typename Vertex> 
  void next_vertex(Vertex v) 
  { 
    std::cout << v << " "; 
  }
};



struct edge_output_visitor : public output_visitor
{
  template <typename Edge> 
  void next_edge(Edge e) 
  { 
    std::cout << e << " "; 
  }
};


int main(int argc, char** argv)
{

  using graph = boost::adjacency_list
    < boost::vecS,
      boost::vecS,
      boost::undirectedS,
      boost::property<boost::vertex_index_t, int>,
      boost::property<boost::edge_index_t, int>
    >;

  // Create a graph - this is a biconnected, 3 x 3 grid.
  // It should have four small (four vertex/four edge) faces and
  // one large face that contains all but the interior vertex
  graph g(9);

  boost::add_edge(0,1,g);
  boost::add_edge(1,2,g);

  boost::add_edge(3,4,g);
  boost::add_edge(4,5,g);
  
  boost::add_edge(6,7,g);
  boost::add_edge(7,8,g);


  boost::add_edge(0,3,g);
  boost::add_edge(3,6,g);

  boost::add_edge(1,4,g);
  boost::add_edge(4,7,g);

  boost::add_edge(2,5,g);
  boost::add_edge(5,8,g);
  

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
                                       &embedding[0]
                                   )
      )
    std::cout << "Input graph is planar" << std::endl;
  else
    std::cout << "Input graph is not planar" << std::endl;

  
  std::cout << std::endl << "Vertices on the faces: " << std::endl;
  vertex_output_visitor v_vis;
  planar_face_traversal(g, &embedding[0], v_vis);

  std::cout << std::endl << "Edges on the faces: " << std::endl;
  edge_output_visitor e_vis;
  planar_face_traversal(g, &embedding[0], e_vis);

  return 0;  
}
