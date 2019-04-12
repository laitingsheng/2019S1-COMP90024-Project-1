#ifndef COMP90024_PROJECT_1_PROCESSOR_SN_MT_H
#define COMP90024_PROJECT_1_PROCESSOR_SN_MT_H

#include "processor.h"

struct processor_sn_mt final : public processor
{
    explicit processor_sn_mt(char const *, grid const &);

    void preprocess() override;

    result_type operator()() const override;
private:
    static int const num_proc;
};

#endif
