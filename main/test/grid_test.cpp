#define BOOST_TEST_MODULE Grid Unit Test

#include <unordered_map>
#include <vector>

#include <boost/test/included/unit_test.hpp>

#include "../include/grid.h"

struct grid_tester final
{
    void test_equality() const
    {
        BOOST_REQUIRE(g1._horizontal == g2._horizontal);
        BOOST_REQUIRE(g1._vertical == g2._vertical);
        BOOST_REQUIRE(g1._valid == g2._valid);
    }

    void test_getter() const
    {
        // @formatter:off
        for (auto [pos, c] : horizontal_test_cases)
        // @formatter:on
            BOOST_REQUIRE(g1.get_horizontal(pos) == c);
        // @formatter:off
        for (auto [pos, c] : vertical_test_cases)
        // @formatter:on
            BOOST_REQUIRE(g1.get_vertical(pos) == c);
    }

    void test_validator() const
    {
        for (auto const & valid : g1._valid)
            BOOST_REQUIRE(g1.validate(valid[0], valid[1]));
        // @formatter:off
        for (auto [v, h] : invalid_test_cases)
        // @formatter:on
            BOOST_REQUIRE(!g1.validate(v, h));
    }

private:
    grid const g1, g2 {"melbGrid.json"};

    static std::unordered_map<double, char> const horizontal_test_cases;
    static std::unordered_map<double, char> const vertical_test_cases;
    static std::vector<std::pair<char, char>> const invalid_test_cases;
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

BOOST_AUTO_TEST_CASE(Getter)
{
    tester.test_getter();
}

BOOST_AUTO_TEST_CASE(Validator)
{
    tester.test_validator();
}

BOOST_AUTO_TEST_SUITE_END()

// @formatter:off
std::unordered_map<double, char> const grid_tester::horizontal_test_cases {
    {-180, 0},
    {144.6, 0},
    {144.7, '1'},
    {144.8, '1'},
    {144.85, '2'},
    {144.9, '2'},
    {145, '3'},
    {145.1, '3'},
    {145.15, '4'},
    {145.2, '4'},
    {145.3, '5'},
    {145.4, '5'},
    {145.45, '5'},
    {145.5, 0},
    {180, 0}
};
std::unordered_map<double, char> const grid_tester::vertical_test_cases {
    {-90, 0},
    {-38.2, 0},
    {-38.1, 'D'},
    {-38, 'D'},
    {-37.95, 'D'},
    {-37.9, 'C'},
    {-37.8, 'C'},
    {-37.7, 'B'},
    {-37.65, 'B'},
    {-37.6, 'A'},
    {-37.5, 'A'},
    {-37.4, 0},
    {90, 0}
};
std::vector<std::pair<char, char>> const grid_tester::invalid_test_cases {
    {'A', '5'},
    {'B', '5'},
    {'D', '1'},
    {'D', '2'},
    {'A', '0'},
    {'E', '6'}
};
// @formatter:on
