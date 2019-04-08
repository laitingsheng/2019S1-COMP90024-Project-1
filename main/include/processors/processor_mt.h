#ifndef _MULTI_THREAD_PROCESSOR_H_
#define _MULTI_THREAD_PROCESSOR_H_

#include "processor_m.h"

struct processor_mt final : public processor_m
{
    explicit processor_mt(char const *, grid const &);

    void preprocess() override;
private:
    static int const num_proc;
};

#endif
