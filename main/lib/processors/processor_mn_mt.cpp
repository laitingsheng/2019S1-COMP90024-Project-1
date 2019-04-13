#include <omp.h>

#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/sort/sort.hpp>

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
    auto curr = file.data();
    auto total_proc = _size * num_proc, actual_thread_start = _rank * num_proc;
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
                if (_rank < _size - 1)
                {
                    long dist;
                    world.recv(_rank + 1, 0, dist);
                    ends[i] = curr + dist;
                }
                else
                    ends[i] = curr + file.size();
        }
        else
        if (_rank > 0)
        {
            long dist = start - curr;
            world.send(_rank - 1, 0, dist);
        }
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

    if (world.size() > 1)
    {
        record_type tmp[_size];
        boost::mpi::all_gather(world, record, tmp);

        auto pc = _size >> 1;
        while (pc > 1)
        {
            #pragma omp parallel for collapse(2) schedule(dynamic)
            for (int dest = 0; dest < pc; ++dest)
                for (unsigned int pos = 0; pos < g.count(); ++pos)
                {
                    int src = dest + pc;
                    // @formatter:off
                    auto & [sc, sm] = tmp[src][pos];
                    // @formatter:on
                    if (sc != 0)
                    {
                        // @formatter:off
                        auto & [dc, dm] = tmp[dest][pos];
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
        auto & dest = tmp[0], src = tmp[1];
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
}

processor::result_type processor_mn_mt::operator()() const
{
    auto block_size = g.count() / _size, remain = g.count() % _size;
    if (_rank < remain)
        ++block_size;
    result_type tmp(block_size);
    for (int i = _rank; i < g.count(); i += _size)
    {
        // @formatter:off
        auto & [c, m] = record[i];
        auto & [rc, rv] = tmp[i / _size];
        // @formatter:on
        rc = {i, c};
        if (c == 0)
            continue;
        rv.resize(m.size());
        auto it = rv.begin();
        for (auto & p : m)
        {
            *it = p;
            ++it;
        }
        boost::sort::block_indirect_sort(rv.begin(), rv.end(), less_tag_info);

        int vc = 0;
        unsigned long lc = 0;
        unsigned long j = 0;
        while (j < rv.size())
        {
            auto c = rv[j].second;
            if (c == 0)
            {
                lc = c;
                break;
            }
            if (c != lc)
            {
                ++vc;
                lc = c;
                if (vc > 5)
                    break;
            }
            ++j;
        }
        if (lc != 0 && vc > 5)
            rv.resize(j);
    }

    std::vector<result_type> tmps(_size);
    boost::mpi::gather(world, tmp, tmps, 0);

    if (_rank == 0)
    {
        result_type re(g.count());
        int i = 0;
        for (auto & rs : tmps)
            for (auto & r : rs)
                re[i++] = std::move(r);
        boost::sort::block_indirect_sort(re.begin(), re.end(), less_cell_total_info);
        return re;
    }

    return {};
}

int const processor_mn_mt::num_proc = omp_get_num_procs();
