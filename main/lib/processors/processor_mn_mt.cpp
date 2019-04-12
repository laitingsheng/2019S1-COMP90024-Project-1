#include "processors/processor_mn_mt.h"

// @formatter:off
processor_mn_mt::processor_mn_mt(
    char const * filename,
    grid const & g,
    boost::mpi::environment const & env,
    boost::mpi::communicator const & world
) : _rank(world.rank()), _size(world.size()), world(world), env(env), processor(filename, g) {}
// @formatter:on

void processor_mn_mt::preprocess()
{
}

processor::result_type processor_mn_mt::operator()() const
{
    return {};
}
