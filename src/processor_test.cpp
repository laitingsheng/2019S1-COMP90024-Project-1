#define BOOST_TEST_MODULE Processor Unit Test

#include <string>
#include <unordered_map>
#include <vector>

#include <boost/test/included/unit_test.hpp>

#include "grid.hpp"
#include "processors.hpp"

template<typename P>
struct processor_tester final
{
    explicit processor_tester(grid const & g) : p("tinyTwitter.json", g) {}

    void test_preprocess()
    {
        p.preprocess();
        for (auto const & [k, v] : p.record)
        {
            auto const & [ik, iv] = v;
            printf("%s %lu\n", k.c_str(), ik);
            for (auto const & [iik, iiv] : iv)
                printf("    %s %lu\n", iik.c_str(), iiv);
        }
    }

    void test_process()
    {
        auto re = p();
        for (auto & [_1, _2, _3] : re)
        {
            printf("%s %lu\n", _1.c_str(), _2);
            for (auto & [_4, _5] : _3)
                printf("    %s %lu\n", _4.c_str(), _5);
        }
    }
private:
    static processor::record_type const preprocess_tiny_ans;
    static processor::result_type const process_tiny_ans;

    P p;
};

BOOST_AUTO_TEST_SUITE(Processor)

grid g;
processor_tester<single_thread_processor> tester(g);

BOOST_AUTO_TEST_CASE(Preprocess)
{
    tester.test_preprocess();
}

BOOST_AUTO_TEST_CASE(Process)
{
    tester.test_process();
}

BOOST_AUTO_TEST_SUITE_END()
