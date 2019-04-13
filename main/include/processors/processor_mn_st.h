#ifndef COMP90024_PROJECT_1_PROCESSOR_MN_ST_H
#define COMP90024_PROJECT_1_PROCESSOR_MN_ST_H

#include <boost/mpi.hpp>

#include "processor.h"

struct processor_mn_st final : public processor
{
    // @formatter:off
    explicit processor_mn_st(
        char const * filename,
        grid const & g,
        boost::mpi::environment const & env,
        boost::mpi::communicator const & world
    );
    // @formatter:on

    void preprocess() override;

    result_type operator()() const override;

private:
    boost::mpi::environment const & env;
    boost::mpi::communicator const & world;

    int const _size, _rank;
};

#endif
