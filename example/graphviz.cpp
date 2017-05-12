// Copyright 2005 Trustees of Indiana University

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Author: Douglas Gregor
#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/test/minimal.hpp>
#include <string>
#include <fstream>
#include <boost/graph/iteration_macros.hpp>

using Digraph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                              boost::property<boost::vertex_name_t, std::string>,
                              boost::property<boost::edge_weight_t, double>>;

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::property<boost::vertex_name_t, std::string>,
                              boost::property<boost::edge_weight_t, double>>;

void test_graph_read_write(const std::string& filename)
{
  std::ifstream in(filename.c_str());
  BOOST_REQUIRE(in);

  Graph g;
  boost::dynamic_properties dp;
  dp.property("id", boost::get(boost::vertex_name, g));
  dp.property("weight", boost::get(boost::edge_weight, g));
  BOOST_CHECK(boost::read_graphviz(in, g, dp, "id"));

  BOOST_CHECK(boost::num_vertices(g) == 4);
  BOOST_CHECK(boost::num_edges(g) == 4);
  
  using Vertex = boost::graph_traits<Graph>::vertex_descriptor;

  std::map<std::string, Vertex> name_to_vertex;
  BGL_FORALL_VERTICES(v, g, Graph) 
    name_to_vertex[get(boost::vertex_name, g, v)] = v;

  // Check vertices
  BOOST_CHECK(name_to_vertex.find("0") != name_to_vertex.end());
  BOOST_CHECK(name_to_vertex.find("1") != name_to_vertex.end());
  BOOST_CHECK(name_to_vertex.find("foo") != name_to_vertex.end());
  BOOST_CHECK(name_to_vertex.find("bar") != name_to_vertex.end());

  // Check edges
  BOOST_CHECK(edge(name_to_vertex["0"], name_to_vertex["1"], g).second);
  BOOST_CHECK(edge(name_to_vertex["1"], name_to_vertex["foo"], g).second);
  BOOST_CHECK(edge(name_to_vertex["foo"], name_to_vertex["bar"], g).second);
  BOOST_CHECK(edge(name_to_vertex["1"], name_to_vertex["bar"], g).second);

  BOOST_CHECK(boost::get(boost::edge_weight, g, 
                  edge(name_to_vertex["0"], name_to_vertex["1"], g).first)
                == 3.14159);
  BOOST_CHECK(boost::get(boost::edge_weight, g, 
                  edge(name_to_vertex["1"], name_to_vertex["foo"], g).first)
                == 2.71828);
  BOOST_CHECK(boost::get(boost::edge_weight, g, 
                  edge(name_to_vertex["foo"], name_to_vertex["bar"], g).first)
                == 10.0);
  BOOST_CHECK(boost::get(boost::edge_weight, g, 
                  edge(name_to_vertex["1"], name_to_vertex["bar"], g).first)
                == 10.0);

  // Write out the graph
  boost::write_graphviz_dp(std::cout, g, dp, std::string("id"));
}

int test_main(int, char*[])
{
  test_graph_read_write("graphviz_example.dot");
  
  return 0;
}
