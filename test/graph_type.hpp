//=======================================================================
// Copyright 2002 Indiana University.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <boost/graph/adjacency_list.hpp>
typedef boost::graph::adjacency_list<boost::graph::setS, boost::graph::setS, boost::graph::undirectedS, boost::property<boost::graph::vertex_id_t, std::size_t>, boost::property<boost::graph::edge_id_t, std::size_t> > Graph;
typedef boost::property<boost::graph::vertex_id_t, std::size_t> VertexId;
typedef boost::property<boost::graph::edge_id_t, std::size_t> EdgeID;
