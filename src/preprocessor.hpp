#ifndef _PREPROCESSOR_HPP_
#define _PREPROCESSOR_HPP_

#include <fstream>
#include <limits>
#include <map>
#include <regex>
#include <string>
#include <utility>

static auto const cell_rgx = std::regex {
    R"cr("id": "(.*)", "xmin": (.)(.), "xmax": (.*), "ymin": (.*), "ymax": (.*), "geometry")cr"
};

template<bool Read>
auto preprocess(std::string const & grid_file_name) -> std::pair<std::map<double, char>, std::map<double, char>>
{
    if constexpr (Read)
    {
        std::map<double, char> horizontal, vertical;

        std::ifstream grid_file(grid_file_name);
        while (!grid_file.eof())
            grid_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::string feature;
        std::smatch feature_matches;
        std::getline(grid_file, feature);
        while (std::regex_search(feature, feature_matches, cell_rgx)) {
            char h = feature_matches[1].str()[0];
            char v = feature_matches[2].str()[0];
            std::getline(grid_file, feature);
        }

        return {
            std::move(horizontal),
            std::move(vertical)
        };
    }
    else
    {
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
            }
        };
    }
}

#endif
