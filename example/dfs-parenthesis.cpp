//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/graph/graphviz.hpp>
#include <boost/graph/depth_first_search.hpp>
#include "range_pair.hpp"

char name[] = "abcdefghij";

struct parenthesis_visitor : public boost::default_dfs_visitor
{
  template <class Vertex, class Graph> void
  start_vertex(Vertex v, const Graph &)
  {
    std::cout << ' ';
  }
  template <class Vertex, class Graph> void
  discover_vertex(Vertex v, const Graph &)
  {
    std::cout << "(" << name[v] << ' ';
  }
  template <class Vertex, class Graph> void
  finish_vertex(Vertex v, const Graph &)
  {
    std::cout << ' ' << name[v] << ")";
  }
};

int
main()
{
  GraphvizGraph g;
  boost::read_graphviz("figs/dfs-example.dot", g);
  for (const auto& edge : make_range_pair(boost::edges(g)))
    std::cout << '(' << name[boost::source(edge, g)] << ' '
      << name[boost::target(edge, g)] << ')' << std::endl;
  parenthesis_visitor
    paren_vis;
  boost::depth_first_search(g, boost::visitor(paren_vis));
  std::cout << std::endl;
  return 0;
}
