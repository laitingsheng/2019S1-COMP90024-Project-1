#ifndef _MULTI_THREAD_PROCESSOR_HPP_
#define _MULTI_THREAD_PROCESSOR_HPP_

#include <omp.h>

#include <boost/sort/sort.hpp>

#include "processor.hpp"

struct multi_thread_processor final : public processor
{
    explicit multi_thread_processor(char const * filename, grid const & g) : processor(filename, g) {}

    processor & preprocess() override
    {
        auto num_proc = omp_get_num_procs();
        auto curr = file.data();
        auto block_size = file.size() / num_proc;
        decltype(curr) starts[num_proc], ends[num_proc];
        starts[0] = curr;
        ends[num_proc - 1] = curr + file.size();
        #pragma omp parallel for simd num_threads(num_proc - 1)
        for (decltype(num_proc) i = 1; i < num_proc; ++i)
        {
            auto & start = starts[i] = curr + i * block_size;
            while (*start++ != '\n');
            ends[i - 1] = start;
        }

        record_type records[num_proc];
        #pragma omp parallel for
        for (decltype(num_proc) i = 0; i < num_proc; ++i)
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

        return *this;
    }

    result_type operator()() const override
    {
        result_type re(record.size());
        auto it = re.begin();
        for (auto & [k, v] : record)
        {
            auto & [ik, iv] = v;
            std::vector<cell_tag_info> tmp(iv.size());
            auto tit = tmp.begin();
            for (auto & [iik, iiv] : iv)
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
};

#endif
