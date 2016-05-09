Boost Graph Library
===================

A generic interface for traversing graphs, using C++ templates.

The full documentation is available on [boost.org](http://www.boost.org/doc/libs/release/libs/graph/doc/index.html).

## Support, bugs and feature requests ##

Bugs and feature requests can be reported through the [trac issue tracker](https://svn.boost.org/trac/boost/query?component=graph&desc=1&order=id). The GitHub issue tracker is not (yet) used on the Boost Graph Library project.

You can submit your changes through a [pull request](https://github.com/boostorg/graph/pulls). One of the maintainers will take a look (remember that it can take some time).

There is no mailing-list specific to Boost Graph, although you can use the general-purpose Boost [mailing-list](https://groups.google.com/forum/#!forum/boost-devel-archive) using the tag [Graph].


## Development ##

Clone the whole boost project, which includes the individual Boost projects as submodules ([see boost+git doc](https://svn.boost.org/trac/boost/wiki/TryModBoost#InstallingModularBoost)): 

    git clone https://github.com/boostorg/boost
    cd boost
    git submodule update --init

The Boost Graph Library is located in `libs/graph/`. 

To _build_ the headers the headers (as boost is mostly a headers-based library, hence no need to build) with:
    
    ./bootstrap.sh
    ./b2 headers


**Note:** The Boost Graph Library cannot currently be built outside of Boost itself.

### Running tests ###
First, make sure you are in `libs/graph/test`. To run the 300+ tests listed in `Jamfile.v2`, you can do

    ../../../b2

Say you fixed some bugs in [`find_flow_cost.hpp`](https://github.com/boostorg/graph/blob/develop/include/boost/graph/find_flow_cost.hpp). You want to know which tests are relevant to that file. The is no straight-forward way to do that, as the tests are scattered in the `test` folder. You can, as an example, look for the main function in this file:

    grep . -r -e "find_flow_cost("

Say you found out that you only want to run the tests in `cycle_canceling_test.cpp`. You can launch the tests for that specific file using:

    ../../../b2 cxxflags="-std=c++11" cycle_canceling_test


