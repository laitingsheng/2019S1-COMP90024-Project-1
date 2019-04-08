#ifndef _MULTI_THREAD_PROCESSOR_H_
#define _MULTI_THREAD_PROCESSOR_H_

#include "processor.h"

struct multi_thread_processor final : public processor
{
    explicit multi_thread_processor(char const * filename, grid const & g);

    void preprocess() override;

    result_type operator()() const override;
};

#endif
