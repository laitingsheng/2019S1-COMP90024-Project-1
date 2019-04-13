#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/vector.hpp>

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
    auto curr = file.data();
    auto block_size = file.size() / _size;
    decltype(curr) start = curr + block_size * _rank;
    while (*start++ != '\n');
    decltype(curr) end = curr + file.size();
    if (_rank > 0)
    {
        long dist = start - curr;
        world.send(_rank - 1, 0, dist);
    }
    if (_rank < _size - 1)
    {
        long dist;
        world.recv(_rank + 1, 0, dist);
        end = curr + dist;
    }
    process_block(start, end, record);

    record_type tmp[_size];
    boost::mpi::all_gather(world, record, tmp);

    auto pc = _size >> 1;
    while (pc > 0)
    {
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
    record = std::move(tmp[0]);
}

processor::result_type processor_mn_st::operator()() const
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
        std::sort(rv.begin(), rv.end(), less_tag_info);

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

    std::vector<result_type> tmps;
    boost::mpi::gather(world, tmp, tmps, 0);

    if (_rank == 0)
    {
        result_type re(g.count());
        int i = 0;
        for (auto & rs : tmps)
            for (auto & r : rs)
                re[i++] = std::move(r);
        std::sort(re.begin(), re.end(), less_cell_total_info);
        return re;
    }

    return {};
}
