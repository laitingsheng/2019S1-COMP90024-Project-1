#ifndef _MULTI_NODE_PROCESSOR_HPP_
#define _MULTI_NODE_PROCESSOR_HPP_

#include <mpi.h>
#include <omp.h>

#include <algorithm>
#include <fstream>
#include <limits>

#include <boost/mpi.hpp>

#include "processor.hpp"

struct multi_node_processor final : public processor
{
    // @formatter:off
    explicit multi_node_processor(
        boost::mpi::environment const & env,
        boost::mpi::communicator const & world,
        char const * filename,
        grid const & g
    ) : processor(filename, g), world(world), env(env)
    // @formatter:on
    {}

    processor & preprocess() override
    {
        // auto const num_procs_per_node = omp_get_num_procs(), total_procs = num_procs_per_node * world.size();
        // auto const read_block_size = st.st_size / num_procs_per_node;
        // std::vector<std::streamsize> read_starts(num_procs_per_node);
        // std::vector<record_type> records(num_procs_per_node);
        //
        // #pragma omp parallel
        // {
        //     auto const tn = omp_get_thread_num();
        //
        //     std::ifstream fs(filename);
        //     fs.seekg(read_block_size * tn);
        //     if (tn > 0)
        //         // set the offset to the start of next line
        //         fs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        //     read_starts[tn] = fs.tellg();
        //     #pragma omp barrier
        //
        //     while (true)
        //     {
        //         std::string buff;
        //         auto finished = false;
        //         if (tn < num_procs_per_node - 1)
        //         {
        //             if (fs.tellg() == read_starts[tn + 1])
        //                 finished = true;
        //             else
        //                 getline(fs, buff);
        //         }
        //         else
        //         {
        //             if (fs.eof())
        //                 finished = true;
        //             else
        //                 getline(fs, buff);
        //         }
        //         if (finished)
        //             break;
        //
        //         process_line(buff, records[tn]);
        //     }
        // };
        //
        // auto pc = num_procs_per_node >> 1;
        // while (pc > 1)
        // {
        //     #pragma omp parallel for num_threads(pc)
        //     for (int i = 0; i < pc; ++i)
        //         merge_records(records[i], std::move(records[i + pc]));
        //     pc >>= 1;
        // }
        // merge_records(records[0], std::move(records[1]));
        //
        // world.barrier();
        //
        // boost::mpi::gather(world, records[0], records, 0);
        //
        // world.barrier();
        //
        // auto nc = world.size() >> 1;
        // while (nc > 1)
        // {
        //     #pragma omp parallel for num_threads(nc)
        //     for (int i = 0; i < nc; ++i)
        //         merge_records(records[i], std::move(records[i + nc]));
        //     nc >>= 1;
        // }
        // merge_records(records[0], std::move(records[1]));
        //
        // record = std::move(records[0]);

        return *this;
    }

    result_type operator()() const override
    {
        return {};
    }
private:
    boost::mpi::environment const & env;
    boost::mpi::communicator const & world;
};

#endif
