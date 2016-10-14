//=======================================================================
// Copyright (C) 2012 Flavio De Lorenzi (fdlorenzi@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/vf2_sub_graph_iso.hpp>
using namespace boost::graph;

int main() {
  using edge_property = boost::property<edge_name_t, char>;
  using vertex_property = boost::property<vertex_name_t, char, boost::property<vertex_index_t, int>>;

  // Using a vecS graphs => the index maps are implicit.
  using graph_type = adjacency_list<vecS, vecS, bidirectionalS, vertex_property, edge_property>;
  
  // Build graph1
  graph_type graph1;
  
  add_vertex(vertex_property('a'), graph1);
  add_vertex(vertex_property('a'), graph1);
  add_vertex(vertex_property('a'), graph1);
  
  add_edge(0, 1, edge_property('b'), graph1); 
  add_edge(0, 1, edge_property('b'), graph1); 
  add_edge(0, 1, edge_property('d'), graph1); 
  
  add_edge(1, 2, edge_property('s'), graph1); 
  
  add_edge(2, 2, edge_property('l'), graph1); 
  add_edge(2, 2, edge_property('l'), graph1); 
  
  // Build graph2
  graph_type graph2;
  
  add_vertex(vertex_property('a'), graph2);
  add_vertex(vertex_property('a'), graph2);
  add_vertex(vertex_property('a'), graph2);
  add_vertex(vertex_property('a'), graph2);
  add_vertex(vertex_property('a'), graph2);
  add_vertex(vertex_property('a'), graph2);
  
  add_edge(0, 1, edge_property('a'), graph2); 
  add_edge(0, 1, edge_property('a'), graph2); 
  add_edge(0, 1, edge_property('b'), graph2); 

  add_edge(1, 2, edge_property('s'), graph2); 
  
  add_edge(2, 3, edge_property('b'), graph2); 
  add_edge(2, 3, edge_property('d'), graph2); 
  add_edge(2, 3, edge_property('b'), graph2); 
  
  add_edge(3, 4, edge_property('s'), graph2); 
  
  add_edge(4, 4, edge_property('l'), graph2); 
  add_edge(4, 4, edge_property('l'), graph2); 

  add_edge(4, 5, edge_property('c'), graph2); 
  add_edge(4, 5, edge_property('c'), graph2); 
  add_edge(4, 5, edge_property('c'), graph2); 
  
  add_edge(5, 0, edge_property('s'), graph2); 
  
  // create predicates
  auto vertex_comp =
    make_property_map_equivalent(boost::get(vertex_name, graph1), boost::get(vertex_name, graph2));
  auto edge_comp =
    make_property_map_equivalent(boost::get(edge_name, graph1), boost::get(edge_name, graph2));
 
  // Create callback
  vf2_print_callback<graph_type, graph_type> callback(graph1, graph2);

  // Print out all subgraph isomorphism mappings between graph1 and graph2.
  // Function vertex_order_by_mult is used to compute the order of 
  // vertices of graph1. This is the order in which the vertices are examined
  // during the matching process.
  vf2_subgraph_iso(graph1, graph2, callback, vertex_order_by_mult(graph1),
                   edges_equivalent(edge_comp).vertices_equivalent(vertex_comp));
  
  return 0;
}
