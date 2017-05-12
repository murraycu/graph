//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <boost/tokenizer.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include "range_pair.hpp"
#include <map>

template <typename DistanceMap>
class bacon_number_recorder : public boost::default_bfs_visitor
{
public:
  bacon_number_recorder(DistanceMap dist) : d(dist) { }

  template <typename Edge, typename Graph>
  void tree_edge(Edge e, const Graph& g) const
  {
    typename boost::graph_traits<Graph>::vertex_descriptor
      u = boost::source(e, g), v = boost::target(e, g);
      d[v] = d[u] + 1;
  }
private:
    DistanceMap d;
};

// Convenience function
template <typename DistanceMap>
bacon_number_recorder<DistanceMap>
record_bacon_number(DistanceMap d)
{
  return bacon_number_recorder<DistanceMap> (d);
}


int
main()
{
  std::ifstream datafile("./kevin-bacon.dat");
  if (!datafile) {
    std::cerr << "No ./kevin-bacon.dat file" << std::endl;
    return EXIT_FAILURE;
  }

  using Graph = boost::adjacency_list <boost::vecS, boost::vecS, boost::undirectedS,
    boost::property<boost::vertex_name_t, std::string>,
    boost::property<boost::edge_name_t, std::string>>;
  Graph g;

  auto actor_name = boost::get(boost::vertex_name, g);
  auto connecting_movie = boost::get(boost::edge_name, g);

  using Vertex = boost::graph_traits<Graph>::vertex_descriptor;
  using NameVertexMap = std::map<std::string, Vertex>;
  NameVertexMap actors;

  for (std::string line; std::getline(datafile, line);) {
    boost::char_delimiters_separator<char> sep(false, "", ";");
    boost::tokenizer <> line_toks(line, sep);
    auto i = line_toks.begin();
    auto actors_name = *i++;
    Vertex u, v;
    auto [pos, inserted] = actors.insert(std::make_pair(actors_name, Vertex()));
    if (inserted) {
      u = add_vertex(g);
      actor_name[u] = actors_name;
      pos->second = u;
    } else
      u = pos->second;

    auto movie_name = *i++;

    std::tie(pos, inserted) = actors.insert(std::make_pair(*i, Vertex()));
    if (inserted) {
      v = add_vertex(g);
      actor_name[v] = *i;
      pos->second = v;
    } else
      v = pos->second;

    auto [e, insertede] = boost::add_edge(u, v, g);
    if (insertede)
      connecting_movie[e] = movie_name;

  }

  std::vector<int>bacon_number(boost::num_vertices(g));

  Vertex src = actors["Kevin Bacon"];
  bacon_number[src] = 0;

  boost::breadth_first_search(g, src,
                       boost::visitor(record_bacon_number(&bacon_number[0])));

  for (const auto& vertex : make_range_pair(boost::vertices(g))) {
    std::cout << actor_name[vertex] << " has a Bacon number of "
      << bacon_number[vertex] << std::endl;
  }

  return 0;
}
