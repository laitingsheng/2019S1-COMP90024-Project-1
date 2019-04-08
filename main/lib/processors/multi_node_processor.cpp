#include "processors/multi_node_processor.h"

// @formatter:off
multi_node_processor::multi_node_processor(
    boost::mpi::environment const & env,
    boost::mpi::communicator const & world,
    char const * filename,
    grid const & g
) : processor(filename, g), world(world), env(env) {}
// @formatter:on

void multi_node_processor::preprocess()
{
    auto curr = file.data();
}

processor::result_type multi_node_processor::operator()() const
{
    return {};
}
