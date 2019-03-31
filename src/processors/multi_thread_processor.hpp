#ifndef _MULTI_THREAD_PROCESSOR_HPP_
#define _MULTI_THREAD_PROCESSOR_HPP_

#include <omp.h>

#include <algorithm>
#include <fstream>
#include <limits>
#include <thread>

#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/sort/sort.hpp>

#include "processor.hpp"

struct multi_thread_processor final : public processor
{
    explicit multi_thread_processor(char const * filename, grid const & g) : processor(filename, g) {}

    processor & preprocess() override
    {
        boost::iostreams::mapped_file_source file(filename);
        printf("%lu\n", file.size());
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
