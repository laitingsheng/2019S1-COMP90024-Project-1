#include <mpi.h>
#include <omp.h>

#include <algorithm>
#include <fstream>
#include <limits>

#include "processor.hpp"

struct multi_node_processor final : public processor
{
    processor & preprocess() override
    {
        return *this;
    }

    processor::result_type operator()() const override
    {
        return {};
    }
};
