#ifndef _MULTI_NODE_PROCESSOR_H_
#define _MULTI_NODE_PROCESSOR_H_

#include <boost/mpi.hpp>

#include "processor.h"

struct multi_node_processor final : public processor
{
    // @formatter:off
    explicit multi_node_processor(
        boost::mpi::environment const & env,
        boost::mpi::communicator const & world,
        char const * filename,
        grid const & g
    );
    // @formatter:on

    void preprocess() override;

    result_type operator()() const override;

private:
    boost::mpi::environment const & env;
    boost::mpi::communicator const & world;
};

#endif
