#include <fstream>
#include <map>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <omp.h>

#include <boost/algorithm/string.hpp>
#include <boost/property_tree/json_parser.hpp>

using std::getline;
using std::ifstream;
using std::map;
using std::move;
using std::numeric_limits;
using std::pair;
using std::stod;
using std::stoul;
using std::streamsize;
using std::string;
using std::stringstream;
using std::tuple;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using boost::algorithm::to_lower;
using boost::property_tree::ptree;
using boost::property_tree::json_parser::read_json;

struct info_counter : unordered_map<string, pair<unsigned long, unordered_map<string, unsigned long>>>
{
    auto merge(info_counter & source) -> void
    {
        // @formatter:off
        for (auto & [k, rv] : source)
        // @formatter:on
            if (count(k))
            {
                // @formatter:off
                auto & [lc, lhc] = this->operator[](k);
                auto & [rc, rhc] = rv;
                // @formatter:on
                lc += rc;
                // @formatter:off
                for (auto & [h, hc] : rhc)
                // @formatter:on
                    lhc[h] += hc;
            }
            else
                this->operator[](k) = move(rv);
    }
};

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
        {90, 0},
        {-37.5, 'A'},
        {-37.65, 'B'},
        {-37.8, 'C'},
        {-37.95, 'D'},
        {-38.1, 'D'}
    };
    unordered_set<string> const invalid {"A5", "B5", "D1", "D2"};
    // @formatter:on

    // parse the first line of the data file
    ifstream twit_file(argv[1]);
    twit_file.ignore(numeric_limits<streamsize>::max(), '\n');

    // @formatter:off
    vector<info_counter> counters(omp_get_num_procs());
    // @formatter:on

    // process the file in parallel
    #pragma omp parallel
    {
        auto & counter = counters[omp_get_thread_num()];
        while (true)
        {
            // attempt to read the file, or terminate current thread if the file reaches the end
            string post_str;
            auto finished = false;
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
            stringstream _0(post_str);
            read_json(_0, post);

            // get information for the JSON
            // @formatter:off
            auto const & doc = post.get_child("doc"),
                       & coordinates = doc.get_child("coordinates").get_child("coordinates");
            // @formatter:on

            // determine the longitude area
            auto citer = coordinates.begin();
            ptree const & long_ptree = citer->second;
            char hc;
            auto longitude = stod(long_ptree.data());
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
            hc = hlbv;

            // determine the latitude area
            ptree const & lat_ptree = (++citer)->second;
            char vc;
            auto latitude = stod(lat_ptree.data());
            auto vlbit = vertical.lower_bound(latitude);
            auto vlbv = vlbit->second;
            if (!vlbv)
                continue;
            if (latitude < vlbit->first && vlbit == vertical.begin())
                continue;
            vc = vlbv;

            // determine if it is in the valid region
            string coord_desc {vc, hc};
            if (invalid.count(coord_desc))
                continue;

            // increment total count
            // @formatter:off
            auto & [count, hashtags_count] = counter[move(coord_desc)];
            // @formatter:on
            ++count;

            // parse hash tags information
            auto const & hashtags = doc.get_child("entities").get_child("hashtags");
            for (auto hiter = hashtags.begin(); hiter != hashtags.end(); ++hiter)
            {
                string text = hiter->second.get_child("text").data();
                to_lower(text);
                ++hashtags_count[move(text)];
            }
        }
    };

    // logarithmic merging
    auto pc = omp_get_num_procs() >> 1;
    while (pc > 1)
    {
        #pragma omp parallel for num_threads(pc)
        for (int i = 0; i < pc; ++i)
            counters[i].merge(counters[i + pc]);
        pc >>= 1;
    }

    // final merge
    counters[0].merge(counters[1]);
    for (auto const & [k, v] : counters[0])
    {
        auto const & [c, hc] = v;
        printf("%s : %lu :", k.c_str(), c);
        for (auto const & [ik, iv] : hc)
            printf(" (#%s: %lu)", ik.c_str(), iv);
        printf("\n");
    }

    return 0;
}
