/*
 * Author: Tingsheng (Tinson) Lai 781319
 */

// @formatter:off

#ifndef READ_GRID
#define READ_GRID 0
#endif

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

#include "read_grid.hpp"

using namespace std;
using namespace chrono;
using namespace filesystem;

namespace mpi = boost::mpi;

using boost::algorithm::to_lower;
using boost::property_tree::ptree;
using boost::property_tree::json_parser::read_json;
// @formatter:off

int main(int argc, char * argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "2 parameter is expected, %d was provided", argc - 1);
        return -1;
    }

    mpi::environment env(argc, argv, mpi::threading::multiple);
    mpi::communicator world;

    auto const [horizontal, vertical, valid] = read_grid<READ_GRID>(argv[1]);

    path const input_file_path(argv[2]);

    return 0;

    regex const coord_rgx(R"("coordinates":\[(-?\d*(?:\.\d*)?),(-?\d*(?:\.\d*)?)\])"),
                hash_tags_rgx(R"("hashtags":\[(\{(?:"indices":\[\d+,\d+\],)?"text":"\w+"(?:,"indices":\[(?:\d+(?:,\d+)*)?\])?\}(?:,\{(?:"indices":\[\d+,\d+\],)?"text":"\w+"(?:,"indices":\[(?:\d+(?:,\d+)*)?\])?\})*)\])");
    // @formatter:on

    // record time used
    auto start = system_clock::now();

    // process the file in parallel
    auto const num_procs = omp_get_num_procs();
    auto const read_block_size = file_size(input_file_path) / num_procs;
    vector<streamsize> read_starts(num_procs);
    unordered_map<string, pair<unsigned long, list<string>>> records;
    #pragma omp parallel
    {
        auto const tn = omp_get_thread_num();

        ifstream twit_file(argv[1]);
        auto const read_start = read_block_size * tn;
        twit_file.seekg(read_start);
        if (tn > 0)
            // set the offset to the start of next line
            twit_file.ignore(numeric_limits<streamsize>::max(), '\n');
        read_starts[tn] = twit_file.tellg();
        #pragma omp barrier

        while (true)
        {
            // attempt to read the file, or terminate current thread if the file reaches the end
            string post_info;
            auto finished = false;
            if (tn < num_procs - 1)
            {
                if (twit_file.tellg() == read_starts[tn + 1])
                    finished = true;
                else
                    getline(twit_file, post_info);
            }
            else
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

            auto &[count, hash_tags_str] = records[move(coord_desc)];
            ++count;

            smatch tags_search;
            if (!regex_search(post_info, tags_search, hash_tags_rgx))
                continue;
            hash_tags_str.emplace_back(tags_search[1].str());
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
