//=======================================================================
// Copyright 2013 University of Warsaw.
// Authors: Piotr Wygocki 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#define BOOST_TEST_MODULE cycle_canceling_test

#include <boost/test/unit_test.hpp>

#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

#include "min_cost_max_flow_utils.hpp"


BOOST_AUTO_TEST_CASE(cycle_canceling_def_test) {
    boost::graph::SampleGraph::vertex_descriptor s,t;
    boost::graph::SampleGraph::Graph g;
    boost::graph::SampleGraph::getSampleGraph(g, s, t);

    boost::graph::edmonds_karp_max_flow(g, s, t);
    boost::graph::cycle_canceling(g);

    int cost = boost::graph::find_flow_cost(g);
    BOOST_CHECK_EQUAL(cost, 29);
}

BOOST_AUTO_TEST_CASE(path_augmentation_def_test2) {
    boost::graph::SampleGraph::vertex_descriptor s,t;
    boost::graph::SampleGraph::Graph g; 
    boost::graph::SampleGraph::getSampleGraph2(g, s, t);

    boost::graph::edmonds_karp_max_flow(g, s, t);
    boost::graph::cycle_canceling(g);

    int cost =  boost::graph::find_flow_cost(g);
    BOOST_CHECK_EQUAL(cost, 7);
}

BOOST_AUTO_TEST_CASE(cycle_canceling_test) {
    boost::graph::SampleGraph::vertex_descriptor s,t;
    typedef boost::graph::SampleGraph::Graph Graph;
    boost::graph::SampleGraph::Graph g; 
    boost::graph::SampleGraph::getSampleGraph(g, s, t);

    int N = boost::graph::num_vertices(g);
    std::vector<int> dist(N);
    typedef boost::graph::graph_traits<Graph>::edge_descriptor edge_descriptor;
    std::vector<edge_descriptor> pred(N);

    boost::property_map<Graph, boost::graph::vertex_index_t>::const_type idx = boost::get(boost::graph::vertex_index, g);

    boost::graph::edmonds_karp_max_flow(g, s, t);
    boost::graph::cycle_canceling(g, boost::graph::distance_map(boost::make_iterator_property_map(dist.begin(), idx)).predecessor_map(boost::make_iterator_property_map(pred.begin(), idx)).vertex_index_map(idx));

    int cost = boost::graph::find_flow_cost(g);
    BOOST_CHECK_EQUAL(cost, 29);
}

