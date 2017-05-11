//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>

template <typename Graph> void
read_graph_file(std::istream & in, Graph & g)
{
  using Vertex = typename boost::graph_traits<Graph>::vertex_descriptor;
  using size_type = typename boost::graph_traits<Graph>::vertices_size_type;
  size_type n_vertices;
  in >> n_vertices;             // read in number of vertices
  std::vector<Vertex> vertex_set(n_vertices);
  for (size_type i = 0; i < n_vertices; ++i)
    vertex_set[i] = add_vertex(g);

  size_type u, v;
  while (in >> u)
    if (in >> v)
      boost::add_edge(vertex_set[u], vertex_set[v], g);
    else
      break;
}


int
main()
{
  using graph_type = boost::adjacency_list < boost::listS,       // Store out-edges of each vertex in a std::list
    boost::vecS,                       // Store vertex set in a std::vector
    boost::directedS                   // The graph is directed
  >;

  graph_type g;                 // use default constructor to create empty graph
  std::ifstream file_in("makefile-dependencies.dat");
  read_graph_file(file_in, g);

  assert(boost::num_vertices(g) == 15);
  assert(boost::num_edges(g) == 19);
  return 0;
}
