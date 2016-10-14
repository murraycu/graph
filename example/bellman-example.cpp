//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bellman_ford_shortest_paths.hpp>
#include "range_pair.hpp"

using namespace boost::graph;

template <typename Graph, typename ParentMap> 
struct edge_writer
{
  edge_writer(const Graph & g, const ParentMap & p)
  : m_g(g), m_parent(p)
  {
  }

  template <typename Edge>
    void operator() (std::ostream & out, const Edge & e) const
  {
    out << "[label=\"" << boost::get(edge_weight, m_g, e) << "\"";
    auto u = source(e, m_g), v = target(e, m_g);
    if (m_parent[v] == u)
        out << ", color=\"black\"";
    else
        out << ", color=\"grey\"";
      out << "]";
  }
  const Graph & m_g;
  ParentMap m_parent;
};
template <typename Graph, typename Parent>
edge_writer<Graph, Parent>
make_edge_writer(const Graph & g, const Parent & p)
{
  return edge_writer<Graph, Parent> (g, p);
}

struct EdgeProperties {
  int weight;
};

int
main()
{
  enum { u, v, x, y, z, N };
  char name[] = { 'u', 'v', 'x', 'y', 'z' };
  using E = std::pair<int, int>;
  const int n_edges = 10;
  E edge_array[] = { E(u, y), E(u, x), E(u, v), E(v, u),
      E(x, y), E(x, v), E(y, v), E(y, z), E(z, u), E(z,x) };
  int weight[n_edges] = { -4, 8, 5, -2, 9, -3, 7, 2, 6, 7 };

  using Graph = adjacency_list < vecS, vecS, directedS,
    boost::no_property, EdgeProperties>;
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  // VC++ can't handle the iterator constructor
  Graph g(N);
  for (std::size_t j = 0; j < n_edges; ++j)
    add_edge(edge_array[j].first, edge_array[j].second, g);
#else
  Graph g(edge_array, edge_array + n_edges, N);
#endif
  graph_traits<Graph>::edge_iterator ei, ei_end;
  auto weight_pmap = boost::get(&EdgeProperties::weight, g);
  int i = 0;
  for (const auto& edge : make_range_pair(edges(g))) {
    weight_pmap[edge] = weight[i];
    ++i;
  }

  std::vector<int> distance(N, (std::numeric_limits<short>::max)());
  std::vector<std::size_t> parent(N);
  for (i = 0; i < N; ++i)
    parent[i] = i;
  distance[z] = 0;

#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  bool r = bellman_ford_shortest_paths
    (g, int(N), weight_pmap, &parent[0], &distance[0], 
     closed_plus<int>(), std::less<int>(), default_bellman_visitor());
#else
  bool r = bellman_ford_shortest_paths
    (g, int (N), weight_map(weight_pmap).distance_map(&distance[0]).
     predecessor_map(&parent[0]));
#endif

  if (r)
    for (i = 0; i < N; ++i)
      std::cout << name[i] << ": " << std::setw(3) << distance[i]
        << " " << name[parent[i]] << std::endl;
  else
    std::cout << "negative cycle" << std::endl;

  std::ofstream dot_file("figs/bellman-eg.dot");
  dot_file << "digraph D {\n"
    << "  rankdir=LR\n"
    << "  size=\"5,3\"\n"
    << "  ratio=\"fill\"\n"
    << "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";

  {
  for(const auto& e : make_range_pair(edges(g))) {
      auto u = source(e, g), v = target(e, g);
      // VC++ doesn't like the 3-argument get function, so here
      // we workaround by using 2-nested boost::get()'s.
      dot_file << name[u] << " -> " << name[v]
        << "[label=\"" << boost::get(boost::get(&EdgeProperties::weight, g), e) << "\"";
      if (parent[v] == u)
        dot_file << ", color=\"black\"";
      else
        dot_file << ", color=\"grey\"";
      dot_file << "]";
    }
  }
  dot_file << "}";
  return EXIT_SUCCESS;
}
