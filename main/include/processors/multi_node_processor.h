#ifndef _MULTI_NODE_PROCESSOR_H_
#define _MULTI_NODE_PROCESSOR_H_

#include <boost/mpi.hpp>

#include "processor.h"

struct multi_node_processor final : public processor
{
    // @formatter:off
    explicit multi_node_processor(int, char * [], char const *, grid const &);
    // @formatter:on

    void preprocess() override;

    result_type operator()() const override;

private:
    boost::mpi::environment env;
    boost::mpi::communicator world;

    int const num_proc;
};

#endif
