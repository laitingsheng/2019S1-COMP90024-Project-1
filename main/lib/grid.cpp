#include <fstream>

#include <boost/geometry/index/rtree.hpp>

#include "grid.h"

grid::grid() :
// @formatter:off
    _rev_map {"A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4", "C1", "C2", "C3", "C4", "C5", "D3", "D4", "D5"},
    _map {
        {"A1", 0},
        {"A2", 1},
        {"A3", 2},
        {"A4", 3},
        {"B1", 4},
        {"B2", 5},
        {"B3", 6},
        {"B4", 7},
        {"C1", 8},
        {"C2", 9},
        {"C3", 10},
        {"C4", 11},
        {"C5", 12},
        {"D3", 13},
        {"D4", 14},
        {"D5", 15}
    },
    _region {
        {90, {0, {}}},
        {-37.5, {'A', {{180, 0}, {145.3, '4'}, {145.15, '4'}, {145, '3'}, {144.85, '2'}, {144.7, '1'}}}},
        {-37.65, {'B', {{180, 0}, {145.3, '4'}, {145.15, '4'}, {145, '3'}, {144.85, '2'}, {144.7, '1'}}}},
        {-37.8, {'C', {{180, 0}, {145.45, '5'}, {145.3, '5'}, {145.15, '4'}, {145, '3'}, {144.85, '2'}, {144.7, '1'}}}},
        {-37.95, {'D', {{180, 0}, {145.45, '5'}, {145.3, '5'}, {145.15, '4'}, {145, '3'}}}},
        {-38.1, {'D', {{180, 0}, {145.45, '5'}, {145.3, '5'}, {145.15, '4'}, {145, '3'}}}}
    }
// @formatter:on
{}

unsigned long grid::count() const
{
    return _rev_map.size();
}

grid::grid(char const * filename) : _region {{90, {0, {}}}}
{
    std::ifstream grid_file(filename);
    std::string feature;
    std::smatch feature_matches;
    unsigned int i = 0;
    char last_v = 0;
    double last_ymin, last_ymax;
    std::map<double, char> last_h;
    while (!grid_file.eof())
    {
        std::getline(grid_file, feature);
        if (std::regex_search(feature, feature_matches, cell_rgx))
        {
            char v = feature_matches[1].str()[0];
            char h = feature_matches[2].str()[0];
            _map[{v, h}] = i++;
            // @formatter:off
            auto xmin = std::stod(feature_matches[3].str()),
                 xmax = std::stod(feature_matches[4].str()),
                 ymin = std::stod(feature_matches[5].str()),
                 ymax = std::stod(feature_matches[6].str());
            // @formatter:on
            if (last_v != v)
            {
                if (last_v != 0)
                    _region[last_ymin] = _region[last_ymax] = {last_v, last_h};
                last_v = v;
                last_ymin = ymin;
                last_ymax = ymax;
                last_h.clear();
                last_h[180] = 0;
            }
            last_h[xmin] = last_h[xmax] = h;
        }
    }
    _region[last_ymin] = {last_v, last_h};
    _region[last_ymax] = {last_v, last_h};

    _rev_map.resize(_map.size());
    // @formatter:off
    for (auto & [k, v] : _map)
    // @formatter:on
        _rev_map[v] = k;
}

std::string const & grid::decode(unsigned coded_coord) const
{
    return _rev_map.at(coded_coord);
}

unsigned int grid::encode(double vertical, double horizontal) const
{
    auto vlbit = _region.lower_bound(vertical);
    auto & [vlbv, _horizontal] = vlbit->second;
    if (!vlbv || vertical < vlbit->first && vlbit == _region.begin())
        return -1;

    auto hlbit = _horizontal.lower_bound(horizontal);
    auto hlbv = hlbit->second;
    if (!hlbv)
        return -1;
    if (horizontal < hlbit->first)
    {
        if (hlbit == _horizontal.begin())
            return -1;
        hlbv = (--hlbit)->second;
    }

    std::string coord_decs {vlbv, hlbv};
    if (!_map.count(coord_decs))
        return -1;
    return _map.at(coord_decs);
}

// @formatter:off
std::regex const grid::cell_rgx(
    R"cr("id": "(.)(.)", "xmin": (.*?), "xmax": (.*?), "ymin": (.*?), "ymax": (.*?),)cr",
    std::regex::ECMAScript | std::regex::optimize | std::regex::collate
);
// @formatter:on
