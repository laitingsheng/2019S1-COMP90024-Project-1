#ifndef _SINGLE_THREAD_PROCESSOR_H_
#define _SINGLE_THREAD_PROCESSOR_H_

#include "processor.h"

struct single_thread_processor final : public processor
{
    explicit single_thread_processor(char const *, grid const &);

    void preprocess() override;

    result_type operator()() const override;
};

#endif
