#ifndef _MULTI_NODE_PROCESSOR_H_
#define _MULTI_NODE_PROCESSOR_H_

#include <boost/mpi.hpp>

#include "processor_m.h"

struct processor_mn final : public processor_m
{
    // @formatter:off
    explicit processor_mn(int, char * [], char const *, grid const &);
    // @formatter:on

    void preprocess() override;

private:
    boost::mpi::environment env;
    boost::mpi::communicator world;

    int const num_proc;
};

#endif
