#include <iostream>

#if defined(MNST) || defined(MNMT)
#include <boost/mpi.hpp>
#endif
#include <boost/timer/timer.hpp>

#include "grid.h"

#if defined(SNST)
#include "processors/processor_sn_st.h"
#elif defined(SNMT)
#include "processors/processor_sn_mt.h"
#elif defined(MNST)
#include "processors/processor_mn_st.h"
#elif defined(MNMT)
#include "processors/processor_mn_mt.h"
#else
#error Invalid Configuration
#endif

int main(int argc, char * argv[])
{
    #if defined(MNST) || defined(MNMT)
    #ifdef MNST
    boost::mpi::environment env(argc, argv, boost::mpi::threading::single);
    #else
    boost::mpi::environment env(argc, argv, boost::mpi::threading::multiple);
    #endif
    boost::mpi::communicator world;
    #endif

    if (argc < 2)
    {
        fprintf(stderr, "Minimum of 1 parameter is expected, %d was provided", argc - 1);
        return -1;
    }

    boost::timer::cpu_timer timer;
    timer.start();
    auto g = argc == 3 ? grid(argv[2]) : grid();
    timer.stop();
    // @formatter:off
    std::cout << timer.format(3, "Grid Reading:\nWall: %w\nUser: %u\nSystem: %s\nTotal: %t\nPercentage: %p%\n\n")
              << std::endl;
    // @formatter:on

    #if defined(SNST)
    processor_sn_st p(argv[1], g);
    #elif defined(SNMT)
    processor_sn_mt p(argv[1], g);
    #elif defined(MNST)
    processor_mn_st p(argv[1], g, env, world);
    #elif defined(MNMT)
    processor_mn_mt p(argv[1], g, env, world);
    #else
    #error Invalid Configuration
    #endif
    timer.start();
    p.preprocess();
    timer.stop();
    // @formatter:off
    std::cout << timer.format(3, "Preprocessing:\nWall: %w\nUser: %u\nSystem: %s\nTotal: %t\nPercentage: %p%\n\n")
              << std::endl;
    // @formatter:on

    timer.start();
    auto output = p();
    timer.stop();
    // @formatter:off
    std::cout << timer.format(3, "Final Processing:\nWall: %w\nUser: %u\nSystem: %s\nTotal: %t\nPercentage: %p%\n\n")
              << std::endl;
    // @formatter:on

    #if defined(MNST) || defined(MNMT)
    if (world.rank() > 0)
        return 0;
    #endif

    processor::print_result(g, output);

    return 0;
}
