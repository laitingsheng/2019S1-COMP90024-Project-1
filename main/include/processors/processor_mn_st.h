#ifndef COMP90024_PROJECT_1_PROCESSOR_MN_ST_H
#define COMP90024_PROJECT_1_PROCESSOR_MN_ST_H


#include "processor.h"

struct processor_mn_st final : public processor
{
    void preprocess() override;

    result_type operator()() const override;
};

#endif
