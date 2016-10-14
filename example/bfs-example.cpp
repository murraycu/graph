//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/range/irange.hpp>

#include <iostream>

using namespace boost::graph;
template <typename TimeMap> class bfs_time_visitor:public default_bfs_visitor {
  using T = typename boost::property_traits<TimeMap>::value_type;
public:
  bfs_time_visitor(TimeMap tmap, T & t):m_timemap(tmap), m_time(t) { }
  template <typename Vertex, typename Graph>
    void discover_vertex(Vertex u, const Graph & g) const
  {
    boost::put(m_timemap, u, m_time++);
  }
  TimeMap m_timemap;
  T & m_time;
};


int
main()
{
  using namespace boost::graph;
  // Select the graph type we wish to use
  using graph_t = adjacency_list<vecS, vecS, undirectedS>;
  // Set up the vertex IDs and names
  enum { r, s, t, u, v, w, x, y, N };
  const char *name = "rstuvwxy";
  // Specify the edges in the graph
  using E = std::pair<int, int>;
  E edge_array[] = { E(r, s), E(r, v), E(s, w), E(w, r), E(w, t),
    E(w, x), E(x, t), E(t, u), E(x, y), E(u, y)
  };
  // Create the graph object
  const int n_edges = sizeof(edge_array) / sizeof(E);
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  // VC++ has trouble with the edge iterator constructor
  graph_t g(N);
  for (std::size_t j = 0; j < n_edges; ++j)
    add_edge(edge_array[j].first, edge_array[j].second, g);
#else
  using v_size_t = graph_traits<graph_t>::vertices_size_type;
  graph_t g(edge_array, edge_array + n_edges, v_size_t(N));
#endif

  // Typedefs
  using Size = graph_traits<graph_t>::vertices_size_type;

  // a vector to hold the discover time property for each vertex
  std::vector<Size> dtime(num_vertices(g));
  using dtime_pm_type =
    boost::iterator_property_map<std::vector<Size>::iterator,
                          boost::property_map<graph_t, boost::graph::vertex_index_t>::const_type>;
  dtime_pm_type dtime_pm(dtime.begin(), boost::get(vertex_index, g));

  Size time = 0;
  bfs_time_visitor<dtime_pm_type> vis(dtime_pm, time);
  breadth_first_search(g, vertex(s, g), visitor(vis));

  // Use std::sort to order the vertices by their discover time
  std::vector<graph_traits<graph_t>::vertices_size_type > discover_order(N);
  boost::integer_range<int> range(0, N);
  std::copy(range.begin(), range.end(), discover_order.begin());
  std::sort(discover_order.begin(), discover_order.end(),
            indirect_cmp<dtime_pm_type, std::less<Size>>(dtime_pm));

  std::cout << "order of discovery: ";
  for (int i = 0; i < N; ++i)
    std::cout << name[discover_order[i]] << " ";
  std::cout << std::endl;

  return EXIT_SUCCESS;
}
