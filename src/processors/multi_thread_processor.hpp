#ifndef _MULTI_THREAD_PROCESSOR_HPP_
#define _MULTI_THREAD_PROCESSOR_HPP_

#include <omp.h>

#include <algorithm>
#include <fstream>
#include <limits>
#include <thread>

#include <boost/sort/sort.hpp>

#include "processor.hpp"

struct multi_thread_processor final : public processor
{
    explicit multi_thread_processor(char const * filename, grid const & g) : processor(filename, g) {}

    processor & preprocess() override
    {
        auto const num_procs = omp_get_num_procs();
        auto const read_block_size = st.st_size / num_procs;
        std::vector<std::streamsize> read_starts(num_procs);
        std::vector<record_type> records(num_procs);

        #pragma omp parallel
        {
            auto const tn = omp_get_thread_num();

            std::ifstream fs(filename);
            fs.seekg(read_block_size * tn);
            if (tn > 0)
                // set the offset to the start of next line
                fs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            read_starts[tn] = fs.tellg();
            #pragma omp barrier

            while (true)
            {
                std::string buff;
                auto finished = false;
                if (tn < num_procs - 1)
                {
                    if (fs.tellg() == read_starts[tn + 1])
                        finished = true;
                    else
                        getline(fs, buff);
                }
                else
                {
                    if (fs.eof())
                        finished = true;
                    else
                        getline(fs, buff);
                }
                if (finished)
                    break;

                process_line(buff, records[tn]);
            }
        }

        auto pc = num_procs >> 1;
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
