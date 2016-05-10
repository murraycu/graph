Boost Graph Library
===================

A generic interface for traversing graphs, using C++ templates.

The full documentation is available on [boost.org][1].

## Support, bugs and feature requests ##

Bugs and feature requests can be reported through the [Trac issue tracker][2]
(see [open issues][3] and [closed issues][4]). [Here][5] is why Trac is still
in use.

You can submit your changes through a [pull request][6]. One of the maintainers
will take a look (remember that it can take some time).

There is no mailing-list specific to Boost Graph, although you can use the
general-purpose Boost [mailing-list][7] using the tag `[graph]`.


## Development ##

Clone the whole boost project, which includes the individual Boost projects as
submodules ([see boost+git doc][8]):

    git clone https://github.com/boostorg/boost
    cd boost
    git submodule update --init

The Boost Graph Library is located in `libs/graph/`. 

Boost Graph Library is mostly made of headers but also contains some compiled
components. Here are the build commands:
    
    ./bootstrap.sh            <- compile b2 
    ./b2 headers              <- just installs headers
    ./b2                      <- build compiled components

**Note:** The Boost Graph Library cannot currently be built outside of Boost 
itself.

### Running tests ###
First, make sure you are in `libs/graph/test`. 
You can either run all the 300+ tests listed in `Jamfile.v2` or run a single 
test:

    ../../../b2                        <- run all tests
    ../../../b2 cycle_canceling_test   <- single test

You can also check the [regression tests reports][9].

[1]: http://www.boost.org/doc/libs/release/libs/graph/doc/index.html "Official BGL documentation"
[2]: https://svn.boost.org/trac/boost/query?component=graph&desc=1&order=id "All issues on Trac"
[3]: https://svn.boost.org/trac/boost/query?status=!closed&component=graph&desc=1&order=id "Open issues on Trac"
[4]: https://svn.boost.org/trac/boost/query?status=closed&component=graph&col=id&col=summary&col=status&col=owner&col=type&col=milestone&col=version&desc=1&order=id "Closed issues on Trac"
[5]: http://lists.boost.org/Archives/boost/2015/04/221780.php "Why still using Trac"
[6]: https://github.com/boostorg/graph/pulls
[7]: https://groups.google.com/forum/#!forum/boost-devel-archive "Google Groups archive for Boost Dev mailing-list"
[8]: https://svn.boost.org/trac/boost/wiki/TryModBoost#InstallingModularBoost "Using git and submodules with Boost"
[9]: http://beta.boost.org/development/tests/develop/developer/graph.html "Regression tests reports"
