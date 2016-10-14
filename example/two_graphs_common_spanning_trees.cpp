//          Copyright (C) 2012, Michele Caini.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//          Two Graphs Common Spanning Trees Algorithm
//      Based on academic article of Mint, Read and Tarjan
//     Efficient Algorithm for Common Spanning Tree Problem
// Electron. Lett., 28 April 1983, Volume 19, Issue 9, p.346-347


#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/two_graphs_common_spanning_trees.hpp>
#include <exception>
#include <vector>


using Graph =
boost::graph::adjacency_list
  <
    boost::graph::vecS,         // OutEdgeList
    boost::graph::vecS,         // VertexList
    boost::graph::undirectedS,  // Directed
    boost::no_property,  // VertexProperties
    boost::no_property,  // EdgeProperties
    boost::no_property,  // GraphProperties
    boost::graph::listS         // EdgeList
  >;

using vertex_descriptor =
boost::graph::graph_traits<Graph>::vertex_descriptor;

using edge_descriptor =
boost::graph::graph_traits<Graph>::edge_descriptor;

using vertex_iterator =
boost::graph::graph_traits<Graph>::vertex_iterator;

using edge_iterator =
boost::graph::graph_traits<Graph>::edge_iterator;


int main(int argc, char **argv)
{
  Graph iG, vG;
  std::vector<edge_descriptor> iG_o = {
    boost::graph::add_edge(0, 1, iG).first,
    boost::graph::add_edge(0, 2, iG).first,
    boost::graph::add_edge(0, 3, iG).first,
    boost::graph::add_edge(0, 4, iG).first,
    boost::graph::add_edge(1, 2, iG).first,
    boost::graph::add_edge(3, 4, iG).first};

  std::vector<edge_descriptor> vG_o= {
    boost::graph::add_edge(1, 2, vG).first,
    boost::graph::add_edge(2, 0, vG).first,
    boost::graph::add_edge(2, 3, vG).first,
    boost::graph::add_edge(4, 3, vG).first,
    boost::graph::add_edge(0, 3, vG).first,
    boost::graph::add_edge(0, 4, vG).first};

  std::vector<bool> inL(iG_o.size(), false);

  std::vector<std::vector<bool>> coll;
  boost::graph::tree_collector<
      std::vector<std::vector<bool>>,
      std::vector<bool>
    > tree_collector(coll);
  boost::graph::two_graphs_common_spanning_trees
    (
      iG,
      iG_o,
      vG,
      vG_o,
      tree_collector,
      inL
    );

  for(const auto& vec : coll) {
    // Here you can play with the trees that the algorithm has found.
  }

  return 0;
}
