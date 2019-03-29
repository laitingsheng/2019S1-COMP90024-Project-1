#ifndef _MULTI_NODE_PROCESSOR_HPP_
#define _MULTI_NODE_PROCESSOR_HPP_

#include <mpi.h>
#include <omp.h>

#include <algorithm>
#include <fstream>
#include <limits>

#include <boost/mpi.hpp>

#include "processor.hpp"

struct multi_node_processor final : public processor
{
    // @formatter:off
    explicit multi_node_processor(
        boost::mpi::environment const & env,
        boost::mpi::communicator const & world,
        char const * filename,
        grid const & g
    ) : processor(filename, g), world(world), env(env)
    // @formatter:on
    {}

    processor & preprocess() override
    {
        auto const num_procs = omp_get_num_procs() * world.size();
        return *this;
    }

    result_type operator()() const override
    {
        return {};
    }
private:
    boost::mpi::environment const & env;
    boost::mpi::communicator const & world;
};

#endif
