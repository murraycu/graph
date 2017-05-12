//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <boost/concept/assert.hpp>
#include <iostream>
#include <fstream>
#include <stack>
#include <map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/reverse_graph.hpp>
#include "range_pair.hpp"

template <typename OutputIterator> 
class back_edge_recorder : public boost::default_dfs_visitor
{
public:
  back_edge_recorder(OutputIterator out):m_out(out) { }

  template <typename Edge, typename Graph>
  void back_edge(Edge e, const Graph &)
  {
    *m_out++ = e;
  }
private:
  OutputIterator m_out;
};

// object generator function
template <typename OutputIterator>
back_edge_recorder<OutputIterator>
make_back_edge_recorder(OutputIterator out)
{
  return back_edge_recorder<OutputIterator> (out);
}

template <typename Graph, typename Loops> void
find_loops(typename boost::graph_traits<Graph>::vertex_descriptor entry, 
           const Graph & g, 
           Loops & loops)    // A container of sets of vertices
{
  BOOST_CONCEPT_ASSERT(( BidirectionalGraphConcept<Graph> ));
  using Edge = typename boost::graph_traits<Graph>::edge_descriptor;
  std::vector<Edge> back_edges;
  std::vector<boost::default_color_type> color_map(boost::num_vertices(g));
  boost::depth_first_visit(g, entry,
                    make_back_edge_recorder(std::back_inserter(back_edges)),
                    boost::make_iterator_property_map(color_map.begin(),
                                               boost::get(boost::vertex_index, g), color_map[0]));

  for (typename std::vector<Edge>::size_type i = 0; i < back_edges.size(); ++i) {
    typename Loops::value_type x;
    loops.emplace_back(x);
    compute_loop_extent(back_edges[i], g, loops.back());
  }
}

template <typename Graph, typename Set> void
compute_loop_extent(typename boost::graph_traits <
                    Graph >::edge_descriptor back_edge, const Graph & g,
                    Set & loop_set)
{
  BOOST_CONCEPT_ASSERT(( BidirectionalGraphConcept<Graph> ));
  using Vertex = typename boost::graph_traits<Graph>::vertex_descriptor;
  using Color = boost::color_traits<boost::default_color_type>;

  auto loop_tail = boost::source(back_edge, g);
  auto loop_head = boost::target(back_edge, g);

  std::vector<boost::default_color_type>
    reachable_from_head(boost::num_vertices(g), Color::white());
  boost::default_color_type c;
  boost::depth_first_visit(g, loop_head, boost::default_dfs_visitor(),
                    boost::make_iterator_property_map(reachable_from_head.begin(),
                                               boost::get(boost::vertex_index, g), c));

  std::vector<boost::default_color_type> reachable_to_tail(boost::num_vertices(g));
  reverse_graph<Graph> reverse_g(g);
  boost::depth_first_visit(reverse_g, loop_tail, boost::default_dfs_visitor(),
                    boost::make_iterator_property_map(reachable_to_tail.begin(),
                                               boost::get(boost::vertex_index, g), c));

  for (const auto& vertex : make_range_pair(boost::vertices(g)))
    if (reachable_from_head[vertex] != Color::white()
        && reachable_to_tail[vertex] != Color::white())
      loop_set.insert(vertex);
}


int
main(int argc, char *argv[])
{
  if (argc < 3) {
    std::cerr << "usage: loops_dfs <in-file> <out-file>" << std::endl;
    return -1;
  }
  GraphvizDigraph g_in;
  boost::read_graphviz(argv[1], g_in);

  using Graph = boost::adjacency_list < boost::vecS, boost::vecS, boost::bidirectionalS,
    GraphvizVertexProperty,
    GraphvizEdgeProperty, GraphvizGraphProperty >;
  using Vertex = boost::graph_traits<Graph>::vertex_descriptor;

  Graph g;
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  // VC++ has trouble with the get_property() function
  boost::get_priority(g, boost::graph_name) = "loops";
#endif

  copy_graph(g_in, g);

  using set_t = std::set<Vertex>;
  using list_of_sets_t = std::list<set_t>;
  list_of_sets_t loops;
  auto entry = *vertices(g).first;

  find_loops(entry, g, loops);

  auto vattr_map = boost::get(vertex_attribute, g);
  auto eattr_map = boost::get(edge_attribute, g);

  for (const auto& loop : loops) {
    std::vector<bool> in_loop(boost::num_vertices(g), false);
    for (const auto& vertex : loop) {
      vattr_map[vertex]["color"] = "gray";
      in_loop[vertex] = true;
    }
    for (const auto& edge : make_range_pair(boost::edges(g)))
      if (in_loop[boost::source(edge, g)] && in_loop[boost::target(edge, g)])
        eattr_map[edge]["color"] = "gray";
  }

  std::ofstream loops_out(argv[2]);
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  // VC++ has trouble with the get_property() functions
  loops_out << "digraph loops {\n"
            << "size=\"3,3\"\n"
            << "ratio=\"fill\"\n"
            << "shape=\"box\"\n";
  for (const auto& vertex : make_range_pair(boost::vertices(g))) {
    loops_out << vertex << "[";
    for (auto ai = vattr_map[vertex].begin();
         ai != vattr_map[vertex].end(); ++ai) {
      loops_out << ai->first << "=" << ai->second;
      if (next(ai) != vattr_map[vertex].end())
        loops_out << ", ";
    }
    loops_out<< "]";
  }

  for (const auto& edge : make_range_pair(boost::edges(g))) {
    loops_out << boost::source(edge, g) << " -> " << boost::target(edge, g) << "[";
    auto& attr_map = eattr_map[edge];
    for (auto eai = attr_map.begin();
         eai != attr_map.end(); ++eai) {
      loops_out << eai->first << "=" << eai->second;
      if (next(eai) != attr_map.end())
        loops_out << ", ";
    }
    loops_out<< "]";
  }
  loops_out << "}\n";
#else
  boost::get_priority(g, graph_graph_attribute)["size"] = "3,3";
  boost::get_priority(g, graph_graph_attribute)["ratio"] = "fill";
  boost::get_priority(g, graph_vertex_attribute)["shape"] = "box";

  boost::write_graphviz(loops_out, g,
                 boost::make_vertex_attributes_writer(g),
                 make_edge_attributes_writer(g),
                 boost::make_graph_attributes_writer(g));
#endif
  return 0;
}
