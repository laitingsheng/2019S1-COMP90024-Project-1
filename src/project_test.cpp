#define BOOST_TEST_MODULE Project Unit Test
#include <boost/test/included/unit_test.hpp>

#include "read_grid.hpp"

BOOST_AUTO_TEST_SUITE(Preprocess)

BOOST_AUTO_TEST_CASE(ReadGrid)
{
    auto const [h1, v1, va1] = read_grid<true>("melbGrid.json");
    auto const [h2, v2, va2] = read_grid<false>("melbGrid.json");

    BOOST_REQUIRE(va1 == va2);
    BOOST_REQUIRE(h1 == h2);
    BOOST_REQUIRE(v1 == v2);
}

BOOST_AUTO_TEST_CASE(ParseTwit)
{
    BOOST_TEST(true);
}

BOOST_AUTO_TEST_SUITE_END()
