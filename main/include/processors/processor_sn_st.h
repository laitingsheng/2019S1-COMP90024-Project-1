#ifndef COMP90024_PROJECT_1_PROCESSOR_SN_ST_H
#define COMP90024_PROJECT_1_PROCESSOR_SN_ST_H

#include "processor.h"

struct processor_sn_st final : public processor
{
    explicit processor_sn_st(char const *, grid const &);

    void preprocess() override;

    result_type operator()() const override;
};

#endif
