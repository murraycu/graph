// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

//[code_bron_kerbosch_print_cliques
#include <iostream>

#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/bron_kerbosch_all_cliques.hpp>

#include "helper.hpp"

using namespace boost;

// The clique_printer is a visitor that will print the vertices that comprise
// a clique. Note that the vertices are not given in any specific order.
template <typename OutputStream>
struct clique_printer
{
    clique_printer(OutputStream& stream)
        : os(stream)
    { }

    template <typename Clique, typename Graph>
    void clique(const Clique& c, const Graph& g)
    {
        // Iterate over the clique and print each vertex within it.
        for(const auto& vertex : c) {
            os << g[vertex].name << " ";
        }
        os << std::endl;
    }
    OutputStream& os;
};

// The Actor type stores the name of each vertex in the graph.
struct Actor
{
    std::string name;
};

// Declare the graph type and its vertex and edge types.
using Graph = undirected_graph<Actor>;
using Vertex = graph_traits<Graph>::vertex_descriptor;
using Edge = graph_traits<Graph>::edge_descriptor;

// The name map provides an abstract accessor for the names of
// each vertex. This is used during graph creation.
using NameMap = property_map<Graph, std::string Actor::*>::type;

int
main(int argc, char *argv[])
{
    // Create the graph and and its name map accessor.
    Graph g;
    NameMap nm(get(&Actor::name, g));

    // Read the graph from standard input.
    read_graph(g, nm, std::cin);

    // Instantiate the visitor for printing cliques
    clique_printer<std::ostream> vis(std::cout);

    // Use the Bron-Kerbosch algorithm to find all cliques, printing them
    // as they are found.
    bron_kerbosch_all_cliques(g, vis);

    return 0;
}
//]
