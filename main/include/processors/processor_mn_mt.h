#ifndef COMP90024_PROJECT_1_PROCESSOR_MN_MT_H
#define COMP90024_PROJECT_1_PROCESSOR_MN_MT_H

#include <boost/mpi.hpp>

#include "processor.h"

struct processor_mn_mt final : public processor
{
    // @formatter:off
    explicit processor_mn_mt(
        char const * filename,
        grid const & g,
        boost::mpi::environment const & env,
        boost::mpi::communicator const & world
    );
    // @formatter:on

    void preprocess() override;

    result_type operator()() const override;

private:
    static int const num_proc;

    boost::mpi::environment const & env;
    boost::mpi::communicator const & world;

    int const _size, _rank;
};

#endif
