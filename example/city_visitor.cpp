//
//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//

#include <boost/config.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/graph_utility.hpp> // for boost::make_list


/*
  Example of using a visitor with the depth first search 
    and breadth first search algorithm

  Sacramento ---- Reno ---- Salt Lake City
     |
  San Francisco
     |
  San Jose ---- Fresno
     |
  Los Angeles ---- Las Vegas ---- Phoenix
     |
  San Diego  


  The visitor has three main functions: 
  
  discover_vertex(u,g) is invoked when the algorithm first arrives at the
    vertex u. This will happen in the depth first or breadth first
    order depending on which algorithm you use.

  examine_edge(e,g) is invoked when the algorithm first checks an edge to see
    whether it has already been there. Whether using BFS or DFS, all
    the edges of vertex u are examined immediately after the call to
    visit(u).

  finish_vertex(u,g) is called when after all the vertices reachable from vertex
    u have already been visited.    

 */

struct city_arrival : public boost::base_visitor<city_arrival>
{
  city_arrival(std::string* n) : names(n) { }
  using event_filter = boost::on_discover_vertex;
  template <class Vertex, class Graph>
  inline void operator()(Vertex u, Graph&) {
    std::cout << std::endl << "arriving at " << names[u] << std::endl
         << "  neighboring cities are: ";
  }
  std::string* names;
};

struct neighbor_cities : public boost::base_visitor<neighbor_cities>
{
  neighbor_cities(std::string* n) : names(n) { }
  using event_filter = boost::on_examine_edge;
  template <class Edge, class Graph>
  inline void operator()(Edge e, Graph& g) {
    std::cout << names[ boost::target(e, g) ] << ", ";
  }
  std::string* names;
};

struct finish_city : public boost::base_visitor<finish_city>
{
  finish_city(std::string* n) : names(n) { }
  using event_filter = boost::on_finish_vertex;
  template <class Vertex, class Graph>
  inline void operator()(Vertex u, Graph&) {
    std::cout << std::endl << "finished with " << names[u] << std::endl;
  }
  std::string* names;
};

int main(int, char*[]) 
{

  enum { SanJose, SanFran, LA, SanDiego, Fresno, LasVegas, Reno,
         Sacramento, SaltLake, Phoenix, N };

  std::string names[] = { "San Jose", "San Francisco", "Los Angeles", "San Diego", 
                     "Fresno", "Las Vegas", "Reno", "Sacramento",
                     "Salt Lake City", "Phoenix" };

  using E = std::pair<int,int>;
  E edge_array[] = { E(Sacramento, Reno), E(Sacramento, SanFran),
                     E(Reno, SaltLake),
                     E(SanFran, SanJose),
                     E(SanJose, Fresno), E(SanJose, LA),
                     E(LA, LasVegas), E(LA, SanDiego),
                     E(LasVegas, Phoenix) };

  /* Create the graph type we want. */
  using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  // VC++ has trouble with the edge iterator constructor
  Graph G(N);
  for (std::size_t j = 0; j < sizeof(edge_array)/sizeof(E); ++j)
    boost::add_edge(edge_array[j].first, edge_array[j].second, G);
#else
  Graph G(edge_array, edge_array + sizeof(edge_array)/sizeof(E), N);
#endif

  std::cout << "*** Depth First ***" << std::endl;
  boost::depth_first_search
    (G, 
     boost::visitor(boost::make_dfs_visitor(boost::make_list(city_arrival(names),
                                               neighbor_cities(names),
                                               finish_city(names)))));
  std::cout << std::endl;

  /* Get the source vertex */
  auto s = vertex(SanJose,G);

  std::cout << "*** Breadth First ***" << std::endl;
  boost::breadth_first_search
    (G, s, boost::visitor(boost::make_bfs_visitor(boost::make_list(city_arrival(names), 
                                                     neighbor_cities(names), 
                                                     finish_city(names)))));
  
  return 0;
}
