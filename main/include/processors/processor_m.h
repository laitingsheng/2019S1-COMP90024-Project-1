#ifndef _PROCESSOR_M_H_
#define _PROCESSOR_M_H_

#include "processor.h"

struct processor_m : public processor
{
    result_type operator()() const override;

protected:
    explicit processor_m(char const *, grid const &);
};

#endif
