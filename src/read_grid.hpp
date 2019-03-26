#ifndef _READ_GRID_HPP_
#define _READ_GRID_HPP_

#include <fstream>
#include <limits>
#include <map>
#include <regex>
#include <string>
#include <unordered_set>
#include <utility>

static auto const cell_rgx = std::regex {
    R"cr("id": "(.)(.)", "xmin": (.*?), "xmax": (.*?), "ymin": (.*?), "ymax": (.*?),)cr"
};

template<bool Read>
// @formatter:off
auto read_grid(std::string const & grid_file_name) -> std::tuple<
    std::map<double, char>,
    std::map<double, char>,
    std::unordered_set<std::string>
>
// @formatter:on
{
    if constexpr (Read)
    {
        std::map<double, char> horizontal {{180, 0}}, vertical {{90, 0}};
        std::unordered_set<std::string> valid;

        std::ifstream grid_file(grid_file_name);
        std::string feature;
        std::smatch feature_matches;
        while (!grid_file.eof())
        {
            std::getline(grid_file, feature);
            if (std::regex_search(feature, feature_matches, cell_rgx))
            {
                char v = feature_matches[1].str()[0];
                char h = feature_matches[2].str()[0];
                valid.insert({v, h});
                // @formatter:off
                auto xmin = std::stod(feature_matches[3].str()),
                     xmax = std::stod(feature_matches[4].str()),
                     ymin = std::stod(feature_matches[5].str()),
                     ymax = std::stod(feature_matches[6].str());
                // @formatter:on
                horizontal[xmin] = horizontal[xmax] = h;
                vertical[ymin] = vertical[ymax] = v;
            }
        }

        // @formatter:off
        return {
            std::move(horizontal),
            std::move(vertical),
            std::move(valid)
        };
        // @formatter:on
    }
    else
    {
        // @formatter:off
        return {
            {
                {180, 0},
                {145.45, '5'},
                {145.3, '5'},
                {145.15, '4'},
                {145, '3'},
                {144.85, '2'},
                {144.7, '1'}
            },
            {
                {90, 0},
                {-37.5, 'A'},
                {-37.65, 'B'},
                {-37.8, 'C'},
                {-37.95, 'D'},
                {-38.1, 'D'}
            },
            {"A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4", "C1", "C2", "C3", "C4", "C5", "D3", "D4", "D5"}
        };
        // @formatter:on
    }
}

#endif
