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
            auto const read_start = read_block_size * tn;
            if (tn > 0)
                // set the offset to the start of next line
                fs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            read_starts[tn] = fs.tellg();
            #pragma omp barrier

            while (true)
            {
                std::string buff;
                auto finished = false;
                break;
            }
        };

        return *this;
    }

    processor::result_type operator()() const override
    {
        return {};
    }
};
