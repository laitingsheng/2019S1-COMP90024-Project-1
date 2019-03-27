#define BOOST_TEST_MODULE Processor Unit Test

#include <boost/test/included/unit_test.hpp>

#include "processor.hpp"

template<typename P>
struct processor_test final
{
    explicit processor_test(P const & p) : p(p) {}

    void test_regex()
    {
    }

    void test_preprocess()
    {
    }

    void test_process()
    {
    }
private:
    processor const & p;
};

BOOST_AUTO_TEST_SUITE(Processor)

BOOST_AUTO_TEST_CASE(RegEx)
{
}

BOOST_AUTO_TEST_CASE(Preprocess)
{
}

BOOST_AUTO_TEST_CASE(Process)
{
}

BOOST_AUTO_TEST_SUITE_END()
