//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

template <typename Graph> void
generic_foo(Graph & g)
{
  // Access descriptor types
  // using Vertex = typename boost::graph_traits<Graph>::vertex_descriptor;
  // using Edge = typename boost::graph_traits<Graph>::edge_descriptor;

  // Access category types
  // using Dir = typename boost::graph_traits<Graph>::directed_category;
  // using Par = typename boost::graph_traits<Graph>::edge_parallel_category;

  // Access iterator types...
  // Access size types...
  // Now do something useful...
}

template <typename Graph> void
generic_bar(Graph & g)
{
  // Declare some vertex and edge descriptor variables
  auto u = vertex(0,g), v = vertex(1,g);
  typename boost::graph_traits<Graph>::edge_descriptor e1, e2;
  // Set u and e1 to valid descriptors...
  v = u;                        // Make v a handle to the same vertex as u.
  e2 = e1;                      // Make e2 a handle to the same edge as e1.
  assert(u == v);               // Do u and v identify the same vertex? Yes
  assert(!(u != v));            // Do u and v identify different vertices? No
  assert(e1 == e2);             // Do e1 and e2 identify the same edge? Yes
  assert(!(e1 != e2));          // Do e1 and e2 identify different edges? No
}

// This version of foo gets called when g is directed
template <typename Graph> void
foo_dispatch(Graph & g, boost::directed_tag)
{
  //... 
}

// This version of foo gets called when g is undirected
template <typename Graph> void
foo_dispatch(Graph & g, boost::undirected_tag)
{
  //...
}

template <typename Graph> void
foo(Graph & g)
{
  using Cat = typename boost::graph_traits<Graph>::directed_category;
  foo_dispatch(g, Cat());
}

template <typename Digraph> void
foo(Digraph & digraph,
    typename boost::graph_traits<Digraph>::vertex_descriptor u,
    typename boost::graph_traits<Digraph>::vertex_descriptor v)
{
  auto e1 = edge(u, v, digraph);
  auto e2 = edge(v, u, digraph);
  assert(e1.first != e2.first);
}
template <typename Undigraph> void
bar(Undigraph & undigraph,
    typename boost::graph_traits<Undigraph>::vertex_descriptor u,
    typename boost::graph_traits<Undigraph>::vertex_descriptor v)
{
  auto e1 = edge(u, v, undigraph);
  auto e2 = edge(v, u, undigraph);
  assert(e1.first == e2.first);
}


int
main()
{

  boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> g(2);
  boost::add_edge(0, 1, g);
  boost::add_edge(1, 0, g);
  generic_foo(g);
  generic_bar(g);
  foo(g);
  foo(g, vertex(0, g), vertex(1, g));

  boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> ug(2);
  boost::add_edge(0, 1, g);
  bar(ug, vertex(0, g), vertex(1, g));

  return 0;
}
