#ifndef _PROCESSOR_M_H_
#define _PROCESSOR_M_H_

#include "processor.h"

struct processor_mt : public processor
{
    result_type operator()() const override;

protected:
    static int const num_proc;

    explicit processor_mt(char const *, grid const &);
};

#endif
