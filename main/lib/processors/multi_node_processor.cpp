#include <omp.h>

#include <boost/serialization/unordered_map.hpp>
#include <boost/sort/sort.hpp>

#include "processors/multi_node_processor.h"

// @formatter:off
multi_node_processor::multi_node_processor(int argc, char * argv[], char const * filename, grid const & g) :
    num_proc(world.size() * omp_get_num_procs()),
    processor(filename, g),
    world(),
    env(argc, argv, boost::mpi::threading::funneled)
{}
// @formatter:on

void multi_node_processor::preprocess()
{
    auto curr = file.data();
    auto block_size = file.size() / num_proc;
    decltype(curr) starts[num_proc], ends[num_proc];
    starts[0] = curr;
    ends[num_proc - 1] = curr + file.size();
    #pragma omp parallel for
    for (int i = 0; i < num_proc; ++i)
    {
        auto & start = starts[i] = curr + i * block_size;
        while (*start++ != '\n');
        if (i > 0)
            ends[i - 1] = start;
    }

    record_type records[num_proc];
    #pragma omp parallel for
    for (int i = 0; i < num_proc; ++i)
        process_block(starts[i], ends[i], records[i]);

    auto pc = num_proc >> 1;
    while (pc > 1)
    {
        #pragma omp parallel for num_threads(pc)
        for (int i = 0; i < pc; ++i)
            merge_records(records[i], std::move(records[i + pc]));
        pc >>= 1;
    }
    merge_records(records[0], std::move(records[1]));
    record = std::move(records[0]);

    if (world.size() > 1)
    {
        boost::mpi::all_gather(world, record, records);
        pc = world.size() >> 1;
        while (pc > 1)
        {
            #pragma omp parallel for num_threads(pc)
            for (int i = 0; i < pc; ++i)
                merge_records(records[i], std::move(records[i + pc]));
            pc >>= 1;
        }
        merge_records(records[0], std::move(records[1]));
        record = std::move(records[0]);
    }
}

processor::result_type multi_node_processor::operator()() const
{
    return {};
}
