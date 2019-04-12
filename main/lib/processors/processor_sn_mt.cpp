#include <omp.h>

#include <boost/sort/sort.hpp>

#include "processors/processor_sn_mt.h"

processor_sn_mt::processor_sn_mt(char const * filename, grid const & g) : processor(filename, g) {}

void processor_sn_mt::preprocess()
{
    auto curr = file.data();
    auto block_size = file.size() / num_proc;
    decltype(curr) starts[num_proc], ends[num_proc];
    #pragma omp parallel for
    for (int i = 0; i < num_proc; ++i)
    {
        auto & start = starts[i] = curr + i * block_size;
        while (*start++ != '\n');
        if (i > 0)
            ends[i - 1] = start;
        else
            ends[num_proc - 1] = curr + file.size();
    }

    record_type records[num_proc];
    #pragma omp parallel for
    for (int i = 0; i < num_proc; ++i)
        process_block(starts[i], ends[i], records[i]);

    auto pc = num_proc >> 1;
    while (pc > 1)
    {
        #pragma omp parallel for collapse(2) schedule(dynamic)
        for (int dest = 0; dest < pc; ++dest)
            for (unsigned int pos = 0; pos < g.count(); ++pos)
            {
                int src = dest + pc;
                // @formatter:off
                auto & [sc, sm] = records[src][pos];
                // @formatter:on
                if (sc != 0)
                {
                    // @formatter:off
                    auto & [dc, dm] = records[dest][pos];
                    // @formatter:on
                    if (dm.empty())
                        dm = std::move(sm);
                    else
                        // @formatter:off
                        for (auto & [k, v] : sm)
                        // @formatter::on
                            dm[k] += v;
                    dc += sc;
                }
            }
        pc >>= 1;
    }
    auto & dest = records[0], src = records[1];
    #pragma omp parallel for schedule(dynamic)
    for (unsigned int pos = 0; pos < g.count(); ++pos)
    {
        // @formatter:off
        auto & [sc, sm] = src[pos];
        // @formatter:on
        if (sc != 0)
        {
            // @formatter:off
            auto & [dc, dm] = dest[pos];
            // @formatter::on
            if (dm.empty())
                dm = std::move(sm);
            else
                // @formatter:off
                for (auto & [k, v] : sm)
                // @formatter::on
                    dm[k] += v;
            dc += sc;
        }
    }
    record = std::move(dest);
}

processor::result_type processor_sn_mt::operator()() const
{
    result_type re(g.count());
    #pragma omp parallel for schedule(dynamic)
    for (unsigned long i = 0; i < g.count(); ++i)
        re[i] = record_to_total_info(i);
    boost::sort::block_indirect_sort(re.begin(), re.end(), less_cell_total_info);
    return re;
}

int const processor_sn_mt::num_proc = omp_get_num_procs();
