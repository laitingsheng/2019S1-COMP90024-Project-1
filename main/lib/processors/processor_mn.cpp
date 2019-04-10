#include <omp.h>

#include <boost/serialization/unordered_map.hpp>
#include <boost/sort/sort.hpp>

#include "processors/processor_mn.h"

// @formatter:off
processor_mn::processor_mn(int argc, char * argv[], char const * filename, grid const & g) :
    rank(world.rank()),
    num_nodes(world.size()),
    processor_m(filename, g),
    world(),
    env(argc, argv, boost::mpi::threading::multiple)
{}
// @formatter:on

void processor_mn::preprocess()
{
    auto curr = file.data();
    auto total_proc = num_nodes * num_proc, actual_thread_start = rank * num_proc;
    auto block_size = file.size() / total_proc;
    decltype(curr) starts[num_proc], ends[num_proc];
    #pragma omp parallel for
    for (int i = 0; i < num_proc; ++i)
    {
        auto & start = starts[i] = curr + block_size * (actual_thread_start + i);
        while (*start++ != '\n');
        if (i > 0)
        {
            ends[i - 1] = start;
            if (i == num_proc - 1)
                if (rank < num_nodes - 1)
                {
                    long dist;
                    world.recv(rank + 1, 0, dist);
                    ends[i] = curr + dist;
                }
                else
                    ends[i] = curr + file.size();
        }
        else
            if (rank > 0)
            {
                long dist = start - curr;
                world.send(rank - 1, 0, dist);
            }
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
        record_type tmp[num_nodes];
        boost::mpi::all_gather(world, record, tmp);
        pc = world.size() >> 1;
        while (pc > 1)
        {
            #pragma omp parallel for num_threads(pc)
            for (int i = 0; i < pc; ++i)
                merge_records(tmp[i], std::move(tmp[i + pc]));
            pc >>= 1;
        }
        merge_records(tmp[0], std::move(tmp[1]));
        record = std::move(tmp[0]);
    }
}
