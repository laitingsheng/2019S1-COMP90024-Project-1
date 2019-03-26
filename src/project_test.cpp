#define BOOST_TEST_MODULE Project Unit Test
#include <boost/test/included/unit_test.hpp>

#include "preprocess.hpp"

BOOST_AUTO_TEST_SUITE(PreprocessTest)

BOOST_AUTO_TEST_CASE(test)
{
    auto const [h1, v1, va1] = preprocess<true>("melbGrid.json");
    auto const [h2, v2, va2] = preprocess<false>("melbGrid.json");

    BOOST_TEST(va1 == va2);
    BOOST_TEST(h1 == h2);
    BOOST_TEST(v1 == v2);
}

BOOST_AUTO_TEST_SUITE_END()
