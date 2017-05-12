//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include "range_pair.hpp"

template <typename Graph, typename VertexNamePropertyMap> void
read_graph_file(std::istream & graph_in, std::istream & name_in,
                Graph & g, VertexNamePropertyMap name_map)
{
  using size_type = typename boost::graph_traits<Graph>::vertices_size_type;
  size_type n_vertices;
  typename boost::property_traits<VertexNamePropertyMap>::value_type name;

  graph_in >> n_vertices;       // read in number of vertices
  for (size_type i = 0; i < n_vertices; ++i) {  // Add n vertices to the graph
    auto u = add_vertex(g);
    name_in >> name;
    put(name_map, u, name);     // ** Attach name property to vertex u **
  }
  size_type src, targ;
  while (graph_in >> src)       // Read in edges
    if (graph_in >> targ)
      boost::add_edge(src, targ, g);   // add an edge to the graph
    else
      break;
}

template <typename Graph, typename VertexNameMap> void
print_dependencies(std::ostream & out, const Graph & g,
                   VertexNameMap name_map)
{
  for (const auto& edge : make_range_pair(boost::edges(g)))
    out << boost::get(name_map, boost::source(edge, g)) << " -$>$ "
      << boost::get(name_map, boost::target(edge, g)) << std::endl;
}


int
main()
{
  using graph_type = boost::adjacency_list < boost::listS,       // Store out-edges of each vertex in a std::list
    boost::vecS,                       // Store vertex set in a std::vector
    boost::directedS,                  // The graph is directed
    boost::property<boost::vertex_name_t, std::string>     // Add a vertex property
   >;

  graph_type g;                 // use default constructor to create empty graph
  const char* dep_file_name = "makefile-dependencies.dat";
  const char* target_file_name = "makefile-target-names.dat";
  std::ifstream file_in(dep_file_name), name_in(target_file_name);
  if (!file_in) {
    std::cerr << "** Error: could not open file " << dep_file_name
      << std::endl;
    return -1;
  }
  if (!name_in) {
    std::cerr << "** Error: could not open file " << target_file_name
      << std::endl;
    return -1;
  }

  // Obtain internal property map from the graph
  auto name_map = boost::get(boost::vertex_name, g);
  read_graph_file(file_in, name_in, g, name_map);

  print_dependencies(std::cout, g, boost::get(boost::vertex_name, g));

  assert(boost::num_vertices(g) == 15);
  assert(boost::num_edges(g) == 19);
  return 0;
}
