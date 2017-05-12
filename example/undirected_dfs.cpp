//=======================================================================
// Copyright 2002 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/undirected_dfs.hpp>
#include <boost/cstdlib.hpp>
#include <iostream>

/*
  Example graph from Tarjei Knapstad.

                   H15
                   |
          H8       C2
            \     /  \
          H9-C0-C1    C3-O7-H14
            /   |     |
          H10   C6    C4
               /  \  /  \
              H11  C5    H13
                   |
                   H12
*/

std::string name[] = { "C0", "C1", "C2", "C3", "C4", "C5", "C6", "O7",
                       "H8", "H9", "H10", "H11", "H12", "H13", "H14", "H15"};


struct detect_loops : public boost::dfs_visitor<>
{
  template <class Edge, class Graph>
  void back_edge(Edge e, const Graph& g) {
    std::cout << name[boost::source(e, g)]
              << " -- "
              << name[boost::target(e, g)] << "\n";
  }
};

int main(int, char*[])
{
  using graph_t = boost::adjacency_list< boost::vecS, boost::vecS, boost::undirectedS,
    boost::no_property,
    boost::property<boost::edge_color_t, boost::default_color_type>>;
  using vertex_t = boost::graph_traits<graph_t>::vertex_descriptor;
  
  const std::size_t N = sizeof(name)/sizeof(std::string);
  graph_t g(N);
  
  boost::add_edge(0, 1, g);
  boost::add_edge(0, 8, g);
  boost::add_edge(0, 9, g);
  boost::add_edge(0, 10, g);
  boost::add_edge(1, 2, g);
  boost::add_edge(1, 6, g);
  boost::add_edge(2, 15, g);
  boost::add_edge(2, 3, g);
  boost::add_edge(3, 7, g);
  boost::add_edge(3, 4, g);
  boost::add_edge(4, 13, g);
  boost::add_edge(4, 5, g);
  boost::add_edge(5, 12, g);
  boost::add_edge(5, 6, g);
  boost::add_edge(6, 11, g);
  boost::add_edge(7, 14, g);
  
  std::cout << "back edges:\n";
  detect_loops vis;
  boost::undirected_dfs(g, boost::root_vertex(vertex_t(0)).visitor(vis)
                 .edge_color_map(boost::get(boost::edge_color, g)));
  std::cout << std::endl;
  
  return boost::exit_success;
}
