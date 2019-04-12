#include "processors/processor_mn_st.h"

// @formatter:off
processor_mn_st::processor_mn_st(
    char const * filename,
    grid const & g,
    boost::mpi::environment const & env,
    boost::mpi::communicator const & world
) : _rank(world.rank()), _size(world.size()), world(world), env(env), processor(filename, g) {}
// @formatter:on

void processor_mn_st::preprocess()
{
}

processor::result_type processor_mn_st::operator()() const
{
    return {};
}
