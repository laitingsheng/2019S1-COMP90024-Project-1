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
        std::vector<std::mutex> locks(num_procs);
        std::vector<std::ifstream> fss(num_procs);
        std::vector<std::streamsize> read_ends(num_procs);
        std::vector<bool> completions(num_procs);
        std::vector<record_type> records(num_procs);

        auto last_proc = num_procs - 1;
        read_ends[last_proc] = fss[last_proc].seekg(0, std::ios_base::end).tellg();

        #pragma omp parallel
        {
            auto const tn = omp_get_thread_num();

            auto & fs = fss[tn];
            fs.open(filename);
            fs.seekg(read_block_size * tn);
            if (tn > 0)
            {
                // set the offset to the start of next line
                fs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                read_ends[tn - 1] = fs.tellg();
            }
            #pragma omp barrier

            auto & lock = locks[tn];
            while (true)
            {
                lock.lock();
                auto completion = completions[tn];
                if (completion)
                {
                    lock.unlock();
                    break;
                }
                if (fs.tellg() == read_ends[tn])
                {
                    completion = true;
                    lock.unlock();
                    break;
                }
                std::string buff;
                getline(fs, buff);
                lock.unlock();

                process_line(buff, records[tn]);
            }

            // steal work
            auto finished = false;
            while (!finished)
            {
                finished = true;
                for (int i = 0; i < num_procs; ++i)
                    if (i != tn)
                    {
                        auto & lock = locks[i];
                        auto & fs = fss[i];
                        if (!lock.try_lock())
                        {
                            finished = false;
                            continue;
                        }
                        auto completion = completions[i];
                        if (completion)
                        {
                            lock.unlock();
                            break;
                        }
                        if (fs.tellg() == read_ends[tn])
                        {
                            completion = true;
                            lock.unlock();
                            break;
                        }
                        std::string buff;
                        getline(fs, buff);
                        lock.unlock();

                        finished = false;
                        process_line(buff, records[tn]);
                    }
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
