#define BOOST_TEST_MODULE find_flow_cost_bundled_properties_and_named_params_test

#include <boost/test/unit_test.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include "min_cost_max_flow_utils.hpp"

typedef boost::graph::adjacency_list_traits<boost::graph::vecS,boost::graph::vecS,boost::graph::directedS> traits;
struct edge_t {
  double capacity;
  float cost;
  float residual_capacity;
  traits::edge_descriptor reversed_edge;
};
struct node_t {
  traits::edge_descriptor predecessor;
  int dist;
  int dist_prev;
  boost::graph::vertex_index_t id;
};
typedef boost::graph::adjacency_list<boost::graph::listS, boost::graph::vecS, boost::graph::directedS, node_t, edge_t > Graph;

// Unit test written in order to fails (at compile time) if the find_flow_cost()
// is not properly handling bundled properties
BOOST_AUTO_TEST_CASE(using_bundled_properties_with_find_max_flow_test)
{
  Graph g;
  traits::vertex_descriptor s,t;

  boost::property_map<Graph,double edge_t::* >::type capacity = boost::get(&edge_t::capacity, g);
  boost::property_map<Graph,float edge_t::* >::type cost = boost::get(&edge_t::cost, g);
  boost::property_map<Graph,float edge_t::* >::type residual_capacity = boost::get(&edge_t::residual_capacity, g);
  boost::property_map<Graph,traits::edge_descriptor edge_t::* >::type rev = boost::get(&edge_t::reversed_edge, g);
  boost::property_map<Graph,traits::edge_descriptor node_t::* >::type pred = boost::get(&node_t::predecessor, g);
  boost::property_map<Graph,boost::graph::vertex_index_t>::type vertex_indices = boost::get(boost::graph::vertex_index, g);
  boost::property_map<Graph,int node_t::* >::type dist = boost::get(&node_t::dist, g);
  boost::property_map<Graph,int node_t::* >::type dist_prev = boost::get(&node_t::dist_prev, g);


  boost::graph::SampleGraph::getSampleGraph(g,s,t,capacity,residual_capacity,cost,rev);

  boost::graph::successive_shortest_path_nonnegative_weights(g,s,t,
      capacity,residual_capacity,cost,rev,vertex_indices,
      pred,dist,dist_prev);

  // The "bundled properties" version (producing errors)
  int flow_cost = boost::graph::find_flow_cost(g,capacity,residual_capacity,cost);
  BOOST_CHECK_EQUAL(flow_cost, 29);
}

// Unit test written in order to fails (at compile time) if the find_flow_cost()
// is not properly handling bundled properties
BOOST_AUTO_TEST_CASE(using_named_params_and_bundled_properties_with_find_max_flow_test)
{
  Graph g;
  traits::vertex_descriptor s,t;

  boost::property_map<Graph,double edge_t::* >::type capacity = boost::get(&edge_t::capacity, g);
  boost::property_map<Graph,float edge_t::* >::type cost = boost::get(&edge_t::cost, g);
  boost::property_map<Graph,float edge_t::* >::type residual_capacity = boost::get(&edge_t::residual_capacity, g);
  boost::property_map<Graph,traits::edge_descriptor edge_t::* >::type rev = boost::get(&edge_t::reversed_edge, g);
  boost::property_map<Graph,traits::edge_descriptor node_t::* >::type pred = boost::get(&node_t::predecessor, g);
  boost::property_map<Graph,boost::graph::vertex_index_t>::type vertex_indices = boost::get(boost::graph::vertex_index, g);
  boost::property_map<Graph,int node_t::* >::type dist = boost::get(&node_t::dist, g);
  boost::property_map<Graph,int node_t::* >::type dist_prev = boost::get(&node_t::dist_prev, g);

  boost::graph::SampleGraph::getSampleGraph(g,s,t,capacity,residual_capacity,cost,rev);

  boost::graph::successive_shortest_path_nonnegative_weights(g,s,t,
    capacity,residual_capacity,cost,rev,vertex_indices,
    pred,dist,dist_prev);

  // The  "named parameters" version (with "bundled properties"; producing errors)
  int flow_cost = boost::graph::find_flow_cost(g,
    boost::graph::capacity_map(capacity)
    .residual_capacity_map(residual_capacity)
    .weight_map(cost));
  BOOST_CHECK_EQUAL(flow_cost, 29);
}
