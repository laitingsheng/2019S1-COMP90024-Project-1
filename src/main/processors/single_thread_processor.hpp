#ifndef _SINGLE_THREAD_PROCESSOR_HPP_
#define _SINGLE_THREAD_PROCESSOR_HPP_

#include <algorithm>
#include <fstream>
#include <limits>

#include "processor.hpp"

struct single_thread_processor final : public processor
{
    explicit single_thread_processor(char const * filename, grid const & g) : processor(filename, g) {}

    processor & preprocess() override
    {
        std::ifstream twit_file(filename);
        while (!twit_file.eof())
        {
            std::string buff;
            std::getline(twit_file, buff);

            process_line(buff, record);
        }

        return *this;
    }

    result_type operator()() const override
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
};

#endif
