#ifndef _SINGLE_THREAD_PROCESSOR_H_
#define _SINGLE_THREAD_PROCESSOR_H_

#include "processor.h"

struct processor_sn_st final : public processor
{
    explicit processor_sn_st(char const *, grid const &);

    void preprocess() override;

    result_type operator()() const override;
};

#endif
