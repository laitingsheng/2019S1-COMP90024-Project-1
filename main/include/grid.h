#ifndef _GRID_H_
#define _GRID_H_

#include <map>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>

struct grid final
{
    using axis = std::map<double, char>;
    using valids = std::unordered_set<std::string>;

    explicit grid();

    explicit grid(char const * filename);

    char get_horizontal(double pos) const;

    char get_vertical(double pos) const;

    bool validate(char vertical, char horizontal) const;

    grid(grid const &) = delete;
    grid(grid &&) = delete;
    grid & operator=(grid const &) = delete;
    grid & operator=(grid &&) = delete;
private:
    friend struct grid_tester;

    static std::regex const cell_rgx;

    axis _horizontal, _vertical;
    valids _valid;
};

#endif
