// (C) Copyright Andrew Sutton 2009
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/subgraph.hpp>
#include "typestr.hpp"

using namespace boost::graph;

struct TestProps {
    typedef boost::property<vertex_name_t, std::size_t> VertexProp;
    typedef boost::property<edge_name_t, std::size_t> EdgeName;
    typedef boost::property<edge_index_t, std::size_t, EdgeName> EdgeProp;

    typedef adjacency_list<
        vecS, vecS, bidirectionalS, VertexProp, EdgeProp
    > Graph;

    typedef subgraph<Graph> Subgraph;
    typedef graph_traits<Subgraph>::vertex_descriptor Vertex;
    typedef graph_traits<Subgraph>::edge_descriptor Edge;
    typedef graph_traits<Subgraph>::vertex_iterator VertexIter;
    typedef std::pair<VertexIter, VertexIter> VertexRange;

    static void run() {
        // Create a graph with some vertices.
        Subgraph g(5);
        VertexRange r = vertices(g);

        // Create a child subgraph and add some vertices.
        Subgraph& sg = g.create_subgraph();
        Vertex v = add_vertex(*r.first, sg);

        typedef boost::property_map<Subgraph, vertex_name_t>::type DefaultMap;
        DefaultMap map = boost::get(vertex_name, g);
        BOOST_ASSERT(boost::get(map, v) == 0);
        boost::put(map, v, 5);
        BOOST_ASSERT(boost::get(map, v) == 5);

        typedef global_property<vertex_name_t> GlobalProp;
        typedef boost::property_map<Subgraph, GlobalProp>::type GlobalVertMap;
        GlobalVertMap groot = boost::get(global(vertex_name), g);
        GlobalVertMap gsub = boost::get(global(vertex_name), sg);
        BOOST_ASSERT(boost::get(groot, v) == 5);
        BOOST_ASSERT(boost::get(gsub, v) == 5);
        boost::put(gsub, v, 10);
        BOOST_ASSERT(boost::get(groot, v) == 10);
        BOOST_ASSERT(boost::get(gsub, v) == 10);
        BOOST_ASSERT(boost::get(map, v) == 10);

        typedef local_property<vertex_name_t> LocalProp;
        typedef boost::property_map<Subgraph, LocalProp>::type LocalVertMap;
        LocalVertMap lroot = boost::get(local(vertex_name), g); // Actually global!
        LocalVertMap lsub = boost::get(local(vertex_name), sg);
        BOOST_ASSERT(boost::get(lroot, v) == 10);  // Recall it's 10 from above!
        BOOST_ASSERT(boost::get(lsub, v) == 0);
        boost::put(lsub, v, 5);
        BOOST_ASSERT(boost::get(lsub, v) == 5);
        BOOST_ASSERT(boost::get(lroot, v) == 10);  // Don't change the root prop
        BOOST_ASSERT(boost::get(map, v) == 10);    // Don't change the root prop

//         typedef detail::subgraph_local_pmap::bind_<LocalProp, Subgraph, void> PM;
//         std::cout << typestr<PM::TagType>() << "\n";
//         std::cout << typestr<PM::PMap>() << "\n";
    }
};

struct TestBundles {
    struct Node {
        Node() : value(-1) { }
        int value;
    };
    struct Arc {
        Arc() : value(-1) { }
        int value;
    };
    typedef boost::property<edge_index_t, std::size_t, Arc> EdgeProp;

    typedef adjacency_list<
        vecS, vecS, bidirectionalS, Node, EdgeProp
    > Graph;

    typedef subgraph<Graph> Subgraph;
    typedef graph_traits<Subgraph>::vertex_descriptor Vertex;
    typedef graph_traits<Subgraph>::edge_descriptor Edge;
    typedef graph_traits<Subgraph>::vertex_iterator VertexIter;
    typedef std::pair<VertexIter, VertexIter> VertexRange;

    static void run() {
        // Create a graph with some vertices.
        Subgraph g(5);
        VertexRange r = vertices(g);

        // Create a child subgraph and add some vertices.
        Subgraph& sg = g.create_subgraph();
        Vertex v = add_vertex(*r.first, sg);

        sg[v].value = 1;
        BOOST_ASSERT(sg[v].value == 1);
        BOOST_ASSERT(sg[global(v)].value == 1);
        BOOST_ASSERT(sg[local(v)].value == -1);

        sg[local(v)].value = 5;
        BOOST_ASSERT(sg[local(v)].value == 5);
        BOOST_ASSERT(sg[global(v)].value == 1);
        BOOST_ASSERT(sg[v].value == 1);

        typedef boost::property_map<
            Subgraph, local_property<int Node::*>
        >::type LocalVertMap;
        LocalVertMap lvm = boost::get(local(&Node::value), sg);
        BOOST_ASSERT(boost::get(lvm, v) == 5);

        typedef boost::property_map<
            Subgraph, global_property<int Node::*>
        >::type GlobalVertMap;
        GlobalVertMap gvm = boost::get(global(&Node::value), sg);
        BOOST_ASSERT(boost::get(gvm, v) == 1);
    }
};

int main(int argc, char* argv[])
{
    TestProps::run();
    TestBundles::run();

    return 0;
}
