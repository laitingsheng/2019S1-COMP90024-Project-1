#include <fstream>

#include "grid.h"

grid::grid() :
// @formatter:off
    _horizontal {
        {180, 0},
        {145.45, '5'},
        {145.3, '5'},
        {145.15, '4'},
        {145, '3'},
        {144.85, '2'},
        {144.7, '1'}
    },
    _vertical {
        {90, 0},
        {-37.5, 'A'},
        {-37.65, 'B'},
        {-37.8, 'C'},
        {-37.95, 'D'},
        {-38.1, 'D'}
    },
    _valid {"A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4", "C1", "C2", "C3", "C4", "C5", "D3", "D4", "D5"}
    // @formatter:on
{}

grid::grid(char const * filename) : _horizontal {{180, 0}}, _vertical {{90, 0}}
{
    std::ifstream grid_file(filename);
    std::string feature;
    std::smatch feature_matches;
    while (!grid_file.eof())
    {
        std::getline(grid_file, feature);
        if (std::regex_search(feature, feature_matches, cell_rgx))
        {
            char v = feature_matches[1].str()[0];
            char h = feature_matches[2].str()[0];
            _valid.insert({v, h});
            // @formatter:off
            auto xmin = std::stod(feature_matches[3].str()),
                 xmax = std::stod(feature_matches[4].str()),
                 ymin = std::stod(feature_matches[5].str()),
                 ymax = std::stod(feature_matches[6].str());
            // @formatter:on
            _horizontal[xmin] = _horizontal[xmax] = h;
            _vertical[ymin] = _vertical[ymax] = v;
        }
    }
}

char grid::get_horizontal(double pos) const
{
    auto hlbit = _horizontal.lower_bound(pos);
    auto hlbv = hlbit->second;
    if (!hlbv)
        return 0;
    if (pos < hlbit->first)
    {
        if (hlbit == _horizontal.begin())
            return 0;
        hlbv = (--hlbit)->second;
    }
    return hlbv;
}

char grid::get_vertical(double pos) const
{
    auto vlbit = _vertical.lower_bound(pos);
    auto vlbv = vlbit->second;
    if (!vlbv || pos < vlbit->first && vlbit == _vertical.begin())
        return 0;
    return vlbv;
}

bool grid::validate(char vertical, char horizontal) const
{
    return static_cast<bool>(_valid.count({vertical, horizontal}));
}

std::regex const grid::cell_rgx(R"cr("id": "(.)(.)", "xmin": (.*?), "xmax": (.*?), "ymin": (.*?), "ymax": (.*?),)cr");
