#ifndef _MULTI_THREAD_PROCESSOR_HPP_
#define _MULTI_THREAD_PROCESSOR_HPP_

#include <omp.h>

#include <algorithm>
#include <fstream>
#include <limits>

#include "processor.hpp"

struct multi_thread_processor final : public processor
{
    explicit multi_thread_processor(char const * filename, grid const & g) : processor(filename, g) {}

    processor & preprocess() override
    {
        auto num_procs = omp_get_num_procs();
        std::vector<std::streamsize> read_starts(num_procs);
        std::vector<result_type> results(num_procs);
        auto read_block_size = st.st_size / num_procs;

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
                };
                if (finished)
                    break;
            }
        };

        return *this;
    }

    result_type operator()() const override
    {
        return {};
    }
};

#endif
