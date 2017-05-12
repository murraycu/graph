// Copyright 2008 Trustees of Indiana University

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// An example of using read_graphviz to load a GraphViz Dot textual
// graph into a BGL adjacency_list graph that has custom properties.

// Author: Ronald Garcia



#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include "range_pair.hpp"
#include <string>
#include <sstream>
#include <cstdlib>


//
// Create a custom graph properties
//  (see the documentation for adjacency list)
struct graph_identifier_t { using kind = boost::graph_property_tag; };
struct vertex_label_t { using kind = boost::vertex_property_tag; };

int main() {

  // Vertex properties
  using vertex_p = boost::property<boost::vertex_name_t, std::string,
    boost::property< vertex_label_t, std::string,
    boost::property<boost::vertex_root_t, int>>>;  
  // Edge properties
  using edge_p = boost::property<boost::edge_name_t, std::string>;
  // Graph properties
  using graph_p = boost::property<boost::graph_name_t, std::string,
    boost::property<graph_identifier_t, std::string>>;
  // boost::adjacency_list-based type
  using graph_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
    vertex_p, edge_p, graph_p>;

  // Construct an empty graph and prepare the dynamic_property_maps.
  graph_t graph(0);
  boost::dynamic_properties dp;

  auto vname = boost::get(boost::vertex_name, graph);
  dp.property("node_id",vname);

  auto vlabel = boost::get(vertex_label_t(), graph);
  dp.property("label",vlabel);

  auto root = boost::get(boost::vertex_root, graph);
  dp.property("root",root);

  auto elabel = boost::get(boost::edge_name, graph);
  dp.property("label",elabel);

  // Use ref_property_map to turn a graph property into a property map
  boost::ref_property_map<graph_t*, std::string> 
    gname(boost::get_property(graph, boost::graph_name));
  dp.property("name",gname);

  // Use ref_property_map to turn a graph property into a property map
  boost::ref_property_map<graph_t*, std::string> 
    gid(boost::get_property(graph,graph_identifier_t()));
  dp.property("identifier",gid);
  // Sample graph as an istream;

const char* dot = 
"digraph \
{ \
  graph [name=\"GRAPH\", identifier=\"CX2A1Z\"] \
    \
    a [label=\"NODE_A\", root=\"1\"] \
    b [label=\"NODE_B\", root=\"0\"] \
 \
 a -> b [label=\"EDGE_1\"] \
 b -> c [label=\"EDGE_2\"] \
}";


  std::istringstream gvgraph(dot);

  bool status = boost::read_graphviz(gvgraph,graph,dp,"node_id");
  if (!status) {
    std::cerr << "read_graphviz() failed." << std::endl;
    return -1;
  }

  std::cout << "graph " << boost::get("name",dp,&graph) <<
      " (" << boost::get("identifier",dp,&graph) << ")\n\n";

  for (auto v : make_range_pair(vertices(graph))) {
    std::cout << "vertex " << boost::get("node_id",dp,v) <<
      " (" << boost::get("label",dp,v) << ")\n";
  }

  return status ? EXIT_SUCCESS : EXIT_FAILURE;
}
