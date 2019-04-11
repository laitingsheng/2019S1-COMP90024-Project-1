#ifndef COMP90024_PROJECT_1_GRID_H
#define COMP90024_PROJECT_1_GRID_H

#include <map>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct grid final
{
    explicit grid();

    explicit grid(char const * filename);

    std::string const & decode(unsigned coded_coord) const;

    unsigned int encode(double vertical, double horizontal) const;\

    grid(grid const &) = delete;

    grid(grid &&) = delete;

    grid & operator=(grid const &) = delete;

    grid & operator=(grid &&) = delete;

private:
    friend struct grid_tester;

    static std::regex const cell_rgx;

    std::map<double, char> _horizontal, _vertical;
    std::unordered_map<std::string, unsigned int> _map;
    std::vector<std::string> _rev_map;
};


#endif
