#ifndef _MULTI_THREAD_PROCESSOR_H_
#define _MULTI_THREAD_PROCESSOR_H_

#include "processor_mt.h"

struct processor_sn_mt final : public processor_mt
{
    explicit processor_sn_mt(char const *, grid const &);

    void preprocess() override;
};

#endif
