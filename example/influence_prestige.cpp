// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

//[influence_prestige_example
#include <iostream>
#include <iomanip>

#include <boost/graph/directed_graph.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/degree_centrality.hpp>

#include "helper.hpp"
#include "range_pair.hpp"

using namespace boost;

// The Actor type stores the name of each vertex in the graph.
struct Actor
{
    std::string name;
};

// Declare the graph type and its vertex and edge types.
using Graph = directed_graph<Actor>;
using Vertex = graph_traits<Graph>::vertex_descriptor;
using Edge = graph_traits<Graph>::edge_descriptor;

// The name map provides an abstract accessor for the names of
// each vertex. This is used during graph creation.
using NameMap = property_map<Graph, std::string Actor::*>::type;

// Declare a container type for influence and prestige (both
// of which are degree centralities) and its corresponding
// property map.
using CentralityProperty = exterior_vertex_property<Graph, unsigned>;
using CentralityContainer = CentralityProperty::container_type;
using CentralityMap = CentralityProperty::map_type;

int
main(int argc, char *argv[])
{
    // Create the graph and a property map that provides
    // access to the actor names.
    Graph g;
    NameMap nm(get(&Actor::name, g));

    // Read the graph from standard input.
    read_graph(g, nm, std::cin);

    // Compute the influence for the graph.
    CentralityContainer influence(num_vertices(g));
    CentralityMap im(influence, g);
    all_influence_values(g, im);

    // Compute the influence for the graph.
    CentralityContainer prestige(num_vertices(g));
    CentralityMap pm(prestige, g);
    all_prestige_values(g, pm);

    // Print the degree centrality of each vertex
    for(const auto& v : make_range_pair(vertices(g))) {
      std::cout << std::setiosflags(std::ios::left) << std::setw(12)
             << g[v].name << "\t"
             << im[v] << "\t"
             << pm[v] << std::endl;
    }

    return 0;
}
//]
