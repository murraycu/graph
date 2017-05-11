//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <boost/pending/stringtok.hpp>
#include <boost/utility.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/depth_first_search.hpp>
#include "range_pair.hpp"


template <class Distance>
class calc_distance_visitor : public boost::bfs_visitor<>
{
public:
  calc_distance_visitor(Distance d) : distance(d) { }

  template <class Graph>
  void tree_edge(typename boost::graph_traits<Graph>::edge_descriptor e,
                 Graph& g)
  {
    auto u = boost::source(e, g);
    auto v = boost::target(e, g);
    distance[v] = distance[u] + 1;
  }
private:
  Distance distance;
};


template <class VertexNameMap, class DistanceMap>
class print_tree_visitor : public boost::dfs_visitor<>
{
public:
  print_tree_visitor(VertexNameMap n, DistanceMap d) : name(n), distance(d) { }
  template <class Graph>
  void 
  discover_vertex(typename boost::graph_traits<Graph>::vertex_descriptor v,
            Graph&)
  {
    using Dist = typename boost::property_traits<DistanceMap>::value_type;
    // indentation based on depth
    for (Dist i = 0; i < distance[v]; ++i)
      std::cout << "  ";
    std::cout << name[v] << std::endl;
  }

  template <class Graph>
  void tree_edge(typename boost::graph_traits<Graph>::edge_descriptor e,
                 Graph& g)
  {
    distance[boost::target(e, g)] = distance[boost::source(e, g)] + 1;
  }  

private:
  VertexNameMap name;
  DistanceMap distance;
};

int
main()
{

  std::ifstream datafile("./boost_web.dat");
  if (!datafile) {
    std::cerr << "No ./boost_web.dat file" << std::endl;
    return -1;
  }

  //===========================================================================
  // Declare the graph type and object, and some property maps.

  using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, 
    boost::property<boost::vertex_name_t, std::string, 
      boost::property<boost::vertex_color_t, boost::default_color_type>>,
    boost::property<boost::edge_name_t, std::string, boost::property<boost::edge_weight_t, int>>
  >;

  using Traits = boost::graph_traits<Graph>;
  using Vertex = Traits::vertex_descriptor;
  using Edge = Traits::edge_descriptor;

  using NameVertexMap = std::map<std::string, Vertex>;
  NameVertexMap name2vertex;
  Graph g;

  auto node_name  = boost::get(boost::vertex_name, g);
  auto link_name = boost::get(boost::edge_name, g);

  //===========================================================================
  // Read the data file and construct the graph.
  
  std::string line;
  while (std::getline(datafile,line)) {

    std::list<std::string> line_toks;
    boost::stringtok(line_toks, line, "|");

    Vertex u, v;

    auto i = line_toks.begin();

    auto [pos, inserted] = name2vertex.insert(std::make_pair(*i, Vertex()));
    if (inserted) {
      u = add_vertex(g);
      put(node_name, u, *i);
      pos->second = u;
    } else
      u = pos->second;
    ++i;

    auto hyperlink_name =  *i++;
      
    std::tie(pos, inserted) = name2vertex.insert(std::make_pair(*i, Vertex()));
    if (inserted) {
      v = add_vertex(g);
      put(node_name, v, *i);
      pos->second = v;
    } else
      v = pos->second;

    Edge e;
    std::tie(e, inserted) = boost::add_edge(u, v, g);
    if (inserted) {
      put(link_name, e, hyperlink_name);
    }
  }

  //===========================================================================
  // Calculate the diameter of the graph.

  using size_type = Traits::vertices_size_type;
  using IntVector = std::vector<size_type>;
  // Create N x N matrix for storing the shortest distances
  // between each vertex. Initialize all distances to zero.
  std::vector<IntVector> d_matrix(boost::num_vertices(g),
                                  IntVector(boost::num_vertices(g), 0));

  size_type i;
  for (i = 0; i < num_vertices(g); ++i) {
    calc_distance_visitor<size_type*> vis(&d_matrix[i][0]);
    auto src = vertices(g).first[i];
    boost::breadth_first_search(g, src, boost::visitor(vis));
  }

  size_type diameter = 0;
  BOOST_USING_STD_MAX();
  for (i = 0; i < num_vertices(g); ++i)
    diameter = max BOOST_PREVENT_MACRO_SUBSTITUTION(diameter, *std::max_element(d_matrix[i].begin(), 
                                                    d_matrix[i].end()));
  
  std::cout << "The diameter of the boost web-site graph is " << diameter
            << std::endl << std::endl;

  std::cout << "Number of clicks from the home page: " << std::endl;
  for (const auto& vertex : make_range_pair(boost::vertices(g)))
    std::cout << d_matrix[0][vertex] << "\t" << node_name[vertex] << std::endl;
  std::cout << std::endl;
  
  //===========================================================================
  // Print out the breadth-first search tree starting at the home page

  // Create storage for a mapping from vertices to their parents
  std::vector<Traits::vertex_descriptor> parent(boost::num_vertices(g));
  for (const auto& vertex : make_range_pair(boost::vertices(g)))
    parent[vertex] = vertex;

  // Do a BFS starting at the home page, recording the parent of each
  // vertex (where parent is with respect to the search tree).
  auto src = vertices(g).first[0];
  boost::breadth_first_search
    (g, src, 
     boost::visitor(boost::make_bfs_visitor(record_predecessors(&parent[0],
                                                         boost::on_tree_edge()))));

  // Add all the search tree edges into a new graph
  Graph search_tree(boost::num_vertices(g));
  auto [vi, vi_end] = vertices(g);
  ++vi;
  for (; vi != vi_end; ++vi)
    boost::add_edge(parent[*vi], *vi, search_tree);

  std::cout << "The breadth-first search tree:" << std::endl;

  // Print out the search tree. We use DFS because it visits
  // the tree nodes in the order that we want to print out:
  // a directory-structure like format.
  std::vector<size_type> dfs_distances(boost::num_vertices(g), 0);

  using NameMap = boost::property_map<Graph, boost::vertex_name_t>::type;
  print_tree_visitor<NameMap, size_type*>
    tree_printer(node_name, &dfs_distances[0]);
  for(const auto& vertex : make_range_pair(boost::vertices(g)))
    boost::get(boost::vertex_color, g)[vertex] = boost::white_color;
  boost::depth_first_visit(search_tree, src, tree_printer, boost::get(boost::vertex_color, g));
  
  return EXIT_SUCCESS;
}
