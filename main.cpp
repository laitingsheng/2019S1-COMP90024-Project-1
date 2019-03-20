#include <fstream>
#include <map>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

#include "omp.h"

#include <boost/property_tree/json_parser.hpp>

using std::getline;
using std::ifstream;
using std::map;
using std::numeric_limits;
using std::pair;
using std::stod;
using std::stoul;
using std::streamsize;
using std::string;
using std::stringstream;
using std::unordered_map;
using std::unordered_set;

using boost::property_tree::ptree;
using boost::property_tree::json_parser::read_json;

int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        printf("1 parameter is expected, %d was provided", argc - 1);
        return -1;
    }

    // hard-coded grid, to eliminate time for reading the melbGrid.json
    // @formatter:off
    map<double, char> const horizontal {
        {180, 0},
        {145.45, '5'},
        {145.3, '5'},
        {145.15, '4'},
        {145, '3'},
        {144.85, '2'},
        {144.7, '1'}
    };
    map<double, char> const vertical {
        {-37.5, 'A'},
        {-37.65, 'B'},
        {-37.8, 'C'},
        {-37.95, 'D'},
        {-38.1, 'D'},
        {-90, 0}
    };
    unordered_set<string> const invalid {"A5", "B5", "D1", "D2"};
    // @formatter:on

    // parse the first line of the data file
    ifstream twit_file(argv[1]);
    twit_file.ignore(numeric_limits<streamsize>::max(), '\n');

    unordered_map<string, pair<unsigned long, unordered_map<string, unsigned long>>> counters[omp_get_num_threads()];
    // process the file in parallel
    #pragma omp parallel
    {
        auto & counter = counters[omp_get_thread_num()];
        while (true)
        {
            // attempt to read the file, or terminate current thread if the file reaches the end
            string post_str;
            bool finished = false;
            #pragma omp critical
            {
                if (twit_file.eof())
                    finished = true;
                else
                    getline(twit_file, post_str);
            };
            if (finished || post_str.empty() || post_str == "]}")
                break;

            // trim the trailing comma if it appears and parse the string
            auto post_end = post_str.end() - 2;
            if (*post_end == ',')
                post_str.erase(post_end, post_end + 2);
            else
                post_str.pop_back();
            ptree post;
            stringstream _3(post_str);
            read_json(_3, post);

            // get information for the JSON
            // @formatter:off
            auto & doc = post.get_child("doc"),
                 & coordinates = doc.get_child("coordinates").get_child("coordinates"),
                 & hashtags = doc.get_child("entities").get_child("hashtags");
            // @formatter:on

            // parse coordinate
            auto citer = coordinates.begin();
            // @formatter:off
            auto const & [_4, long_ptree] = *citer++;
            auto const & [_5, lat_ptree] = *citer;
            // @formatter:on

            // determine the longitude area
            char hc;
            double longitude = stod(long_ptree.data());
            auto hlbit = horizontal.lower_bound(longitude);
            // @formatter:off
            auto [hlbk, hlbv] = *hlbit;
            // @formatter:on
            if (!hlbv)
                continue;
            if (hlbk == longitude)
                hc = hlbv;
            else
            {
                if (hlbit == horizontal.begin())
                    continue;
                --hlbit;
                // @formatter:off
                auto [_, lbv] = *hlbit;
                // @formatter:on
                hc = lbv;
            }

            // determine the latitude area
            char vc;
            double latitude = stod(lat_ptree.data());
            auto vlbit = vertical.lower_bound(latitude);
            // @formatter:off
            auto [vlbk, vlbv] = *vlbit;
            // @formatter:on
        }
    };

    return 0;
}
