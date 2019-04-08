#include "processors/single_thread_processor.h"

single_thread_processor::single_thread_processor(char const * filename, grid const & g) : processor(filename, g) {}

void single_thread_processor::preprocess()
{
    auto curr = file.data();
    process_block(curr, curr + file.size(), record);
}

processor::result_type single_thread_processor::operator()() const
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
        for (auto & [iik, iiv] : iv)
        // @formatter:on
        {
            *tit = {iik, iiv};
            ++tit;
        }
        std::sort(tmp.begin(), tmp.end(), less_cell_tag_info);
        if (tmp.size() > 5)
            tmp.erase(tmp.begin() + 5, tmp.end());
        *it = {k, ik, std::move(tmp)};
        ++it;
    }
    std::sort(re.begin(), re.end(), less_cell_info);
    return re;
}
