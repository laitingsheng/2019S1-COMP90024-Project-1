#define BOOST_TEST_MODULE Grid Unit Test

#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/test/included/unit_test.hpp>

#include "../include/grid.h"

struct grid_tester final
{
    void test_equality() const
    {
        BOOST_REQUIRE(g1._horizontal == g2._horizontal);
        BOOST_REQUIRE(g1._vertical == g2._vertical);
        BOOST_REQUIRE(g1._map == g2._map);
        BOOST_REQUIRE(g1._rev_map == g2._rev_map);
    }

    void test_encode() const
    {
        // @formatter:off
        for (auto & [k, v] : encode_test_cases)
            for (auto & [cv, ch] : v)
        // @formatter:on
                BOOST_REQUIRE(g1.encode(cv, ch) == k);
    }

private:
    grid const g1, g2 {"melbGrid.json"};

    static std::unordered_map<unsigned int, std::vector<std::pair<double, double>>> const encode_test_cases;
};

struct GridFixture
{
    grid_tester tester;
};

BOOST_FIXTURE_TEST_SUITE(Grid, GridFixture)

BOOST_AUTO_TEST_CASE(Eqaulity)
{
    tester.test_equality();
}

BOOST_AUTO_TEST_CASE(Encode)
{
    tester.test_encode();
}

BOOST_AUTO_TEST_SUITE_END()

// @formatter:off
// std::unordered_map<std::pair<double, double>, unsigned> const grid_tester::encode_test_cases {};
std::unordered_map<unsigned int, std::vector<std::pair<double, double>>> const grid_tester::encode_test_cases {
    {-1, {}},
    {0, {}},
    {1, {}},
    {2, {}},
    {3, {}},
    {4, {}},
    {5, {}},
    {6, {}},
    {7, {
        {-37.65, 145.15}, {-37.65, 145.2}, {-37.65, 145.3},
        {-37.7, 145.15}, {-37.7, 145.2}, {-37.7, 145.3}
    }},
    {8, {
        {-37.8, 144.7}, {-37.8, 144.8},
        {-37.9, 144.7}, {-37.9, 144.8}
    }},
    {9, {
        {-37.8, 144.85}, {-37.8, 144.9},
        {-37.9, 144.85}, {-37.9, 144.9}
    }},
    {10, {
        {-37.8, 145}, {-37.8, 145.1},
        {-37.9, 145}, {-37.9, 145.1}
    }},
    {11, {
        {-37.8, 145.15}, {-37.8, 145.2},
        {-37.9, 145.15}, {-37.9, 145.2}
    }},
    {12, {
        {-37.8, 145.3}, {-37.8, 145.4}, {-37.8, 145.45},
        {-37.9, 145.3}, {-37.9, 145.4}, {-37.9, 145.45}
    }},
    {13, {
        {-37.95, 145}, {-37.95, 145.1},
        {-38, 145}, {-38, 145.1},
        {-38.1, 145}, {-38.1, 145.1}
    }},
    {14, {
        {-37.95, 145.15}, {-37.95, 145.2},
        {-38, 145.15}, {-38, 145.2},
        {-38.1, 145.15}, {-38.1, 145.2}
    }},
    {15, {
        {-37.95, 145.3}, {-37.95, 145.4}, {-37.95, 145.45},
        {-38, 145.3}, {-38, 145.4}, {-38, 145.45},
        {-38.1, 145.3}, {-38.1, 145.4}, {-38.1, 145.45}
    }}
};
// @formatter:on
