//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
// Copyright 2009 Trustees of Indiana University.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek, Michael Hansen
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/incremental_components.hpp>
#include <boost/pending/disjoint_sets.hpp>
#include "range_pair.hpp"

using namespace boost;

int main(int argc, char* argv[]) 
{
  using Graph = adjacency_list <vecS, vecS, undirectedS>;
  using Vertex = graph_traits<Graph>::vertex_descriptor;
  //using Edge = graph_traits<Graph>::edge_descriptor;
  using VertexIndex = graph_traits<Graph>::vertices_size_type;
 
  // Create a graph
  const int VERTEX_COUNT = 6;
  Graph graph(VERTEX_COUNT);

  add_edge(0, 1, graph);
  add_edge(1, 4, graph);

  // reate the disjoint-sets object, which requires rank and parent
  // vertex properties.
  std::vector<Vertex> rank(num_vertices(graph));
  std::vector<Vertex> parent(num_vertices(graph));

  using Rank = VertexIndex*;
  using Parent = Vertex*;
  disjoint_sets<Rank, Parent> ds(&rank[0], &parent[0]);

  // Determine the connected components, storing the results in the
  // disjoint-sets object.
  initialize_incremental_components(graph, ds);
  incremental_components(graph, ds);

  // Add a couple more edges and update the disjoint-sets
  add_edge(4, 0, graph);
  add_edge(2, 5, graph);

  ds.union_set(4, 0);
  ds.union_set(2, 5);

  for (auto current_vertex : make_range_pair(vertices(graph))) {
    std::cout << "representative[" << current_vertex << "] = " <<
      ds.find_set(current_vertex) << std::endl;
  }

  std::cout << std::endl;

  // Generate component index. NOTE: We would need to pass in a vertex
  // index map into the component_index constructor if our graph type
  // used listS instead of vecS (identity_property_map is used by
  // default).
  using Components = component_index<VertexIndex>;
  Components components(parent.begin(), parent.end());

  // Iterate through the component indices
  for (auto component_index : components) {
    std::cout << "component " << component_index << " contains: ";

    // Iterate through the child vertex indices for [component_index]
    for (auto child_index : make_range_pair(components[component_index])) {
      std::cout << child_index << " ";
    }

    std::cout << std::endl;
  }

  return (0);
}
