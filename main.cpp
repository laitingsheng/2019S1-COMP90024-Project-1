/*
 * Author: Tingsheng (Tinson) Lai 781319
 */

#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

#include <omp.h>

#include <boost/algorithm/string.hpp>
#include <boost/mpi.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/sort/sort.hpp>

using namespace std;
using namespace chrono;

namespace mpi = boost::mpi;

using boost::algorithm::to_lower;
using boost::property_tree::ptree;
using boost::sort::block_indirect_sort;
using boost::property_tree::json_parser::read_json;

using hash_tag_pair = pair<string, unsigned long>;
static constexpr auto htp_less(hash_tag_pair const & l, hash_tag_pair const & r) -> bool
{
    auto ls = l.second, rs = r.second;
    if (ls < rs)
        return false;
    if (ls == rs)
        return l.first < r.first;
    return true;
}

using cell_tuple = tuple<string, unsigned long, vector<hash_tag_pair>>;
static constexpr auto ct_less(cell_tuple const & l, cell_tuple const & r) -> bool
{
    auto ls = get<1>(l), rs = get<1>(r);
    if (ls < rs)
        return false;
    if (ls == rs)
        return get<0>(l) < get<0>(r);
    return true;
}

int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        printf("1 parameter is expected, %d was provided", argc - 1);
        return -1;
    }

    mpi::environment env(argc, argv);
    mpi::communicator world;

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
        {90, 0},
        {-37.5, 'A'},
        {-37.65, 'B'},
        {-37.8, 'C'},
        {-37.95, 'D'},
        {-38.1, 'D'}
    };
    unordered_set<string> const invalid {"A5", "B5", "D1", "D2"};
    regex const coord_rgx {R"("coordinates":\[(-?\d*(?:\.\d*)?),(-?\d*(?:\.\d*)?)\])"},
                hash_tag_rgx {R"("hashtags":\[((?:\{.*"text":"[[:alnum:]]+".*\})+)\])"};
    // @formatter:on

    // ignore the first line of the data file
    auto start = system_clock::now();
    ifstream twit_file(argv[1]);
    twit_file.ignore(numeric_limits<streamsize>::max(), '\n');
    unordered_map<string, list<ptree>> records;

    // process the file in parallel
    #pragma omp parallel
    while (true)
    {
        // attempt to read the file, or terminate current thread if the file reaches the end
        string post_info;
        auto finished = false;
        #pragma omp critical
        {
            if (twit_file.eof())
                finished = true;
            else
                getline(twit_file, post_info);
        };
        if (finished)
            break;

        smatch coord_search;
        if (!regex_search(post_info, coord_search, coord_rgx))
            continue;

        auto longitude = stod(coord_search[1].str());
        auto hlbit = horizontal.lower_bound(longitude);
        auto hlbv = hlbit->second;
        if (!hlbv)
            continue;
        if (longitude < hlbit->first)
        {
            if (hlbit == horizontal.begin())
                continue;
            hlbv = (--hlbit)->second;
        }
        auto hc = hlbv;

        auto latitude = stod(coord_search[2].str());
        auto vlbit = vertical.lower_bound(latitude);
        auto vlbv = vlbit->second;
        if (!vlbv)
            continue;
        if (latitude < vlbit->first && vlbit == vertical.begin())
            continue;
        auto vc = vlbv;

        string coord_desc {vc, hc};
        if (invalid.count(coord_desc))
            continue;

        printf("%3d: %s\n", omp_get_thread_num(), coord_desc.c_str());
    }

    // @formatter:off
    printf(
        "%d out of %d: I/O time used: %.3lfs\n", world.rank() + 1, world.size(),
        duration_cast<milliseconds>(system_clock::now() - start).count() * 1.0 / 1000
    );
    // @formatter:on
    start = system_clock::now();

    // process

    // @formatter:off
    printf(
        "%d out of %d: processing time used: %.3lfs\n", world.rank() + 1, world.size(),
        duration_cast<milliseconds>(system_clock::now() - start).count() * 1.0 / 1000
    );
    // @formatter:on

    return 0;
}
