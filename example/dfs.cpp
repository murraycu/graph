//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <cassert>
#include <iostream>

#include <vector>
#include <algorithm>
#include <utility>


#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/visitors.hpp>

/*
  This calculates the discover finishing time.

  Sample Output

  Tree edge: 0 --> 2
  Tree edge: 2 --> 1
  Back edge: 1 --> 1
  Finish edge: 1 --> 1
  Tree edge: 1 --> 3
  Back edge: 3 --> 1
  Finish edge: 3 --> 1
  Tree edge: 3 --> 4
  Back edge: 4 --> 0
  Finish edge: 4 --> 0
  Back edge: 4 --> 1
  Finish edge: 4 --> 1
  Forward or cross edge: 2 --> 3
  Finish edge: 2 --> 3
  Finish edge: 0 --> 2
  1 10
  3 8
  2 9
  4 7
  5 6

 */

template <class VisitorList>
struct edge_categorizer : public boost::dfs_visitor<VisitorList> {
  using Base = boost::dfs_visitor<VisitorList>;

  edge_categorizer(const VisitorList& v = boost::null_visitor()) : Base(v) { }

  template <class Edge, class Graph>
  void tree_edge(Edge e, Graph& G) {
    std::cout << "Tree edge: " << boost::source(e, G) <<
      " --> " <<  boost::target(e, G) << std::endl;
    Base::tree_edge(e, G);
  }
  template <class Edge, class Graph>
  void back_edge(Edge e, Graph& G) {
    std::cout << "Back edge: " << boost::source(e, G)
         << " --> " <<  boost::target(e, G) << std::endl;
    Base::back_edge(e, G);
  }
  template <class Edge, class Graph>
  void forward_or_cross_edge(Edge e, Graph& G) {
    std::cout << "Forward or cross edge: " << boost::source(e, G)
         << " --> " <<  boost::target(e, G) << std::endl;
    Base::forward_or_cross_edge(e, G);
  }
  template <class Edge, class Graph> 
  void finish_edge(Edge e, Graph& G) { 
    std::cout << "Finish edge: " << boost::source(e, G) << 
      " --> " <<  boost::target(e, G) << std::endl;
    Base::finish_edge(e, G); 
  } 
};
template <class VisitorList>
edge_categorizer<VisitorList>
categorize_edges(const VisitorList& v) {
  return edge_categorizer<VisitorList>(v);
}

int 
main(int , char* [])
{
  
  using Graph = boost::adjacency_list<>;
  
  Graph G(5);
  boost::add_edge(0, 2, G);
  boost::add_edge(1, 1, G);
  boost::add_edge(1, 3, G);
  boost::add_edge(2, 1, G);
  boost::add_edge(2, 3, G);
  boost::add_edge(3, 1, G);
  boost::add_edge(3, 4, G);
  boost::add_edge(4, 0, G);
  boost::add_edge(4, 1, G);

  using size_type = boost::graph_traits<Graph>::vertices_size_type;

  std::vector<size_type> d(boost::num_vertices(G));  
  std::vector<size_type> f(boost::num_vertices(G));
  int t = 0;
  boost::depth_first_search(G, boost::visitor(categorize_edges(
                     std::make_pair(stamp_times(&d[0], t, boost::on_discover_vertex()),
                               stamp_times(&f[0], t, boost::on_finish_vertex())))));

  for (auto i = d.begin(), j = f.begin(); i != d.end(); ++i, ++j)
    std::cout << *i << " " << *j << std::endl;

  return 0;
}

