#define BOOST_TEST_MODULE Processor Unit Test

#include <boost/test/included/unit_test.hpp>

#include "processor.hpp"

template<typename P>
struct processor_test final
{
    explicit processor_test(P const & p) : p(p) {}
private:
    processor const & p;
};
