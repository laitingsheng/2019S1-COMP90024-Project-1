#ifndef _MULTI_NODE_PROCESSOR_H_
#define _MULTI_NODE_PROCESSOR_H_

#include <boost/mpi.hpp>

#include "processor_mt.h"

struct processor_mn_mt final : public processor_mt
{
    explicit processor_mn_mt(int, char * [], char const *, grid const &);

    void preprocess() override;

private:
    boost::mpi::environment const env;
    boost::mpi::communicator const world;

    int const num_nodes, rank;
};

#endif
