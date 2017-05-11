// Copyright (C) 2006-2009 Dmitry Bufistov and Andrey Parfenov

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <cassert>
#include <ctime>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/random.hpp>
#include <boost/graph/howard_cycle_ratio.hpp>
#include "range_pair.hpp"

/**
 * @author Dmitry Bufistov
 * @author Andrey Parfenov
 */

using grap_real_t = boost::adjacency_list<
    boost::listS, boost::listS, boost::directedS,
    boost::property<boost::vertex_index_t, int>,
    boost::property<
        boost::edge_weight_t, double, boost::property<boost::edge_weight2_t, double>
    >
>;

template <typename TG>
void gen_rand_graph(TG &g, size_t nV, size_t nE)
{
    g.clear();
    boost::mt19937 rng;
    rng.seed(uint32_t(time(nullptr)));
    boost::generate_random_graph(g, nV, nE, rng, true, true);
    boost::uniform_real<> ur(-1,10);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<>>   ew1rg(rng, ur);
    boost::randomize_property<boost::edge_weight_t>(g, ew1rg);
    boost::uniform_int<size_t> uint(1,5);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<size_t>>      ew2rg(rng, uint);
    boost::randomize_property<boost::edge_weight2_t>(g, ew2rg);
}

int main(int argc, char* argv[])
{
    const double epsilon = 0.0000001;
    using ccReal_t = std::vector<boost::graph_traits<grap_real_t>::edge_descriptor>;
    ccReal_t cc; ///critical cycle

    grap_real_t tgr;
    auto vim = boost::get(boost::vertex_index, tgr);
    auto ew1 = boost::get(boost::edge_weight, tgr);
    auto ew2 = boost::get(boost::edge_weight2, tgr);

    gen_rand_graph(tgr, 1000, 30000);
    std::cout << "Vertices number: " << num_vertices(tgr) << std::endl;
    std::cout << "Edges number: " << num_edges(tgr) << std::endl;
    int i = 0;
    for(const auto& vertex : make_range_pair(boost::vertices(tgr))) {
        vim[vertex] = i++; ///Initialize vertex index property
    }
    auto max_cr = maximum_cycle_ratio(tgr, vim, ew1, ew2);
    std::cout << "Maximum cycle ratio is " << max_cr << std::endl;
    auto min_cr = minimum_cycle_ratio(tgr, vim, ew1, ew2, &cc);
    std::cout << "Minimum cycle ratio is " << min_cr << std::endl;
    std::pair<double, double> cr(.0,.0);
    std::cout << "Critical cycle:\n";
    for (const auto& edge : cc)
    {
        cr.first += ew1[edge];
        cr.second += ew2[edge];
        std::cout << "(" << vim[boost::source(edge, tgr)] << "," <<
            vim[boost::target(edge, tgr)] << ") ";
    }
    std::cout << std::endl;
    assert(std::abs(cr.first / cr.second - min_cr) < epsilon);
    return EXIT_SUCCESS;
}

