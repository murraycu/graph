//=======================================================================
// Copyright 2002 Indiana University.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef BOOST_GRAPH_SELECTORS_HPP
#define BOOST_GRAPH_SELECTORS_HPP

#include <type_traits>

namespace boost {

  //===========================================================================
  // Selectors for the Directed template parameter of adjacency_list
  // and adjacency_matrix.

  struct directedS {
    static constexpr bool is_directed = true;
    static constexpr bool is_bidir = false;
  };

  struct undirectedS { 
    static constexpr bool is_directed = false;
    static constexpr bool is_bidir = false;
  };

  struct bidirectionalS { 
    static constexpr bool is_directed = true;
    static constexpr bool is_bidir = true;
  };

} // namespace boost

#endif // BOOST_GRAPH_SELECTORS_HPP
