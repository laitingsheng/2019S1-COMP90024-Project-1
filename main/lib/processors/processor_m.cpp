#include <boost/sort/sort.hpp>

#include "processors/processor_m.h"

processor::result_type processor_m::operator()() const
{
    result_type re(record.size());
    auto it = re.begin();
    // @formatter:off
    for (auto & [k, v] : record)
    // @formatter:on
    {
        // @formatter:off
        auto & [ik, iv] = v;
        // @formatter:on
        std::vector<cell_tag_info> tmp(iv.size());
        auto tit = tmp.begin();
        // @formatter:off
        for (auto &[iik, iiv] : iv)
        // @formatter:on
        {
            *tit = {iik, iiv};
            ++tit;
        }
        boost::sort::block_indirect_sort(tmp.begin(), tmp.end(), less_cell_tag_info);
        if (tmp.size() > 5)
            tmp.erase(tmp.begin() + 5, tmp.end());
        *it = {k, ik, std::move(tmp)};
        ++it;
    }
    boost::sort::block_indirect_sort(re.begin(), re.end(), less_cell_info);
    return re;
}

processor_m::processor_m(char const * filename, grid const & g) : processor(filename, g) {}
