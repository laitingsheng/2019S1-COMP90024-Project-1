/*
 * Author: Tingsheng (Tinson) Lai 781319
 */

// @formatter:off
#include <omp.h>
#include <mpi.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <list>
#include <map>
#include <regex>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <boost/algorithm/string.hpp>
#include <boost/mpi.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/sort/sort.hpp>

using namespace std;
using namespace chrono;
using namespace filesystem;

namespace mpi = boost::mpi;

using boost::algorithm::to_lower;
// @formatter:off

int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "1 parameter is expected, %d was provided", argc - 1);
        return -1;
    }

    mpi::environment env(argc, argv);
    mpi::communicator world;

    path const input_file_path(argv[1]);
    if (!is_regular_file(input_file_path))
    {
        fprintf(stderr, "The given file path %s is not a regular file\n", argv[1]);
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
        {90, 0},
        {-37.5, 'A'},
        {-37.65, 'B'},
        {-37.8, 'C'},
        {-37.95, 'D'},
        {-38.1, 'D'}
    };
    unordered_set<string> const invalid {"A5", "B5", "D1", "D2"};
    regex const coord_rgx(R"("coordinates":\[(-?\d*(?:\.\d*)?),(-?\d*(?:\.\d*)?)\])"),
                hash_tags_rgx(R"("hashtags":\[(\{(?:"indices":\[\d+,\d+\],)?"text":"\w+"(?:,"indices":\[(?:\d+(?:,\d+)*)?\])?\}(?:,\{(?:"indices":\[\d+,\d+\],)?"text":"\w+"(?:,"indices":\[(?:\d+(?:,\d+)*)?\])?\})*)\])");
    // @formatter:on

    // ignore the first line of the data file
    auto start = system_clock::now();
    unordered_map<string, pair<unsigned long, list<string>>> records;

    // process the file in parallel
    auto num_procs = omp_get_num_procs();
    auto read_block_size = file_size(input_file_path) / num_procs;
    vector<unsigned long> file_starts(num_procs);
    #pragma omp parallel
    {
        auto tn = omp_get_thread_num();

        ifstream twit_file(argv[1]);
        auto file_start = read_block_size * tn;
        twit_file.seekg(file_start);
        file_starts[tn] = file_start;

        #pragma omp barrier

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

            // smatch coord_search;
            // if (!regex_search(post_info, coord_search, coord_rgx))
            //     continue;
            //
            // auto longitude = stod(coord_search[1].str());
            // auto hlbit = horizontal.lower_bound(longitude);
            // auto hlbv = hlbit->second;
            // if (!hlbv)
            //     continue;
            // if (longitude < hlbit->first)
            // {
            //     if (hlbit == horizontal.begin())
            //         continue;
            //     hlbv = (--hlbit)->second;
            // }
            // auto hc = hlbv;
            //
            // auto latitude = stod(coord_search[2].str());
            // auto vlbit = vertical.lower_bound(latitude);
            // auto vlbv = vlbit->second;
            // if (!vlbv)
            //     continue;
            // if (latitude < vlbit->first && vlbit == vertical.begin())
            //     continue;
            // auto vc = vlbv;
            //
            // string coord_desc {vc, hc};
            // if (invalid.count(coord_desc))
            //     continue;

            // auto &[count, hash_tags_str] = records[move(coord_desc)];
            // ++count;

            // smatch tags_search;
            // if (!regex_search(post_info, tags_search, hash_tags_rgx))
            //     continue;
            // hash_tags_str.emplace_back(tags_search[1].str());
        }
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
