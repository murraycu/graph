//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <fstream>
#include <map>
#include <string>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/graphviz.hpp>

int
main()
{
  using namespace boost::graph;
  GraphvizDigraph g;
  read_graphviz("figs/scc.dot", g);

  using vertex_t = graph_traits<GraphvizDigraph>::vertex_descriptor;
  std::map<vertex_t, int> component;

  strong_components(g, boost::make_assoc_property_map(component));

  auto vertex_attr_map = boost::get(vertex_attribute, g);
  std::string color[] = {
  "white", "gray", "black", "lightgray"};
  graph_traits<GraphvizDigraph>::vertex_iterator vi, vi_end;
  for (std::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    vertex_attr_map[*vi]["color"] = color[component[*vi]];
    vertex_attr_map[*vi]["style"] = "filled";
    if (vertex_attr_map[*vi]["color"] == "black")
      vertex_attr_map[*vi]["fontcolor"] = "white";
  }
  write_graphviz("figs/scc-out.dot", g);

  return EXIT_SUCCESS;
}
