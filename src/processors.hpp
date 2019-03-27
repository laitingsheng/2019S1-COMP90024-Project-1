#ifndef _PROCESSOR_HPP_
#define _PROCESSOR_HPP_

#include <mpi.h>
#include <omp.h>
#include <sys/stat.h>

#include <algorithm>
#include <fstream>
#include <limits>
#include <regex>
#include <unordered_map>
#include <utility>

#include "grid.hpp"

struct processor
{
    using cell_tag_info = std::pair<std::string, unsigned long>;
    using cell_info = std::tuple<std::string, unsigned long, std::vector<cell_tag_info>>;
    using result_type = std::vector<cell_info>;

    processor(processor const &) = delete;
    processor(processor &&) = delete;
    processor & operator=(processor const &) = delete;
    processor & operator=(processor &&) = delete;

    virtual result_type operator()() const = 0;

    virtual processor & preprocess() = 0;
protected:
    // @formatter:off
    using record_type = std::unordered_map<
        std::string,
        std::pair<
            unsigned long,
            std::unordered_map<std::string, unsigned long>
        >
    >;
    // @formatter:on

    static std::regex coord_rgx, hash_tags_rgx, hash_tag_rgx;
    std::string filename;
    struct stat st;
    grid const & g;
    record_type record;

    explicit processor(char const * filename, grid const & g) : g(g), filename(filename)
    {
        stat(filename, &st);
    }

    static bool less_cell_tag_info(cell_tag_info const & l, cell_tag_info const & r)
    {
        auto ls = l.second, rs = r.second;
        if (ls != rs)
            return ls > rs;
        return l.first < r.first;
    }

    static bool less_cell_info(cell_info const & l, cell_info const & r)
    {
        auto ls = std::get<1>(l), rs = std::get<1>(r);
        if (ls != rs)
            return ls > rs;
        return std::get<0>(l) < std::get<0>(r);
    }
private:
    template<typename>
    friend struct processor_tester;
};

std::regex processor::coord_rgx(R"cr("coordinates":\[(.+?),(.+?)\])cr");
std::regex processor::hash_tags_rgx(R"htr("hashtags":\[(\{.+?\})(,\1)*?\])htr");
std::regex processor::hash_tag_rgx(R"htr("text":"(.*?)")htr");

struct single_thread_processor final : public processor
{
    single_thread_processor(char const * filename, grid const & g) : processor(filename, g) {}

    processor & preprocess() override
    {
        std::ifstream twit_file(filename);
        while (!twit_file.eof())
        {
            std::string buff;
            std::getline(twit_file, buff);

            std::smatch coord_search;
            if (!std::regex_search(buff, coord_search, coord_rgx))
                continue;

            auto h = g.get_horizontal(std::stod(coord_search[1].str()));
            if (!h)
                continue;

            auto v = g.get_vertical(std::stod(coord_search[2].str()));
            if (!v)
                continue;

            if (!g.validate(v, h))
                continue;

            // @formatter:off
            auto & [count, hash_tags_counts] = record[{v, h}];
            // @formatter:on
            ++count;

            std::smatch hash_tags_search;
            if (!std::regex_search(buff, hash_tags_search, hash_tags_rgx))
                continue;

            std::smatch hash_tag_search;
            std::string hash_tags_str = hash_tags_search[0].str();
            if (!std::regex_search(hash_tags_str, hash_tag_search, hash_tag_rgx))
                continue;

            for (auto it = hash_tag_search.begin() + 1; it < hash_tag_search.end(); it += 2)
                ++hash_tags_counts[it->str()];
        }

        return *this;
    }

    result_type operator()() const override
    {
        result_type re(record.size());
        auto it = re.begin();
        for (auto & [k, v] : record)
        {
            auto & [ik, iv] = v;
            std::vector<cell_tag_info> tmp(iv.size());
            auto tit = tmp.begin();
            for (auto & [iik, iiv] : iv)
            {
                *tit = {iik, iiv};
                ++tit;
            }
            std::sort(tmp.begin(), tmp.end(), less_cell_tag_info);
            if (tmp.size() > 5)
                tmp.erase(tmp.begin() + 5, tmp.end());
            *it = {k, ik, std::move(tmp)};
        }
        std::sort(re.begin(), re.end(), less_cell_info);
        return re;
    }
};

struct multi_thread_processor final : public processor
{
    processor & preprocess() override
    {
        auto num_procs = omp_get_num_procs();
        std::vector<std::streamsize> read_starts(num_procs);
        std::vector<result_type> results(num_procs);
        auto read_block_size = st.st_size / num_procs;

        #pragma omp parallel
        {
            auto const tn = omp_get_thread_num();

            std::ifstream fs(filename);
            auto const read_start = read_block_size * tn;
            if (tn > 0)
                // set the offset to the start of next line
                fs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            read_starts[tn] = fs.tellg();
            #pragma omp barrier

            while (true)
            {
                std::string buff;
                auto finished = false;
                break;
            }
        };

        return *this;
    }

    result_type operator()() const override
    {
        return {};
    }
};

struct multi_node_processor final : public processor
{
    processor & preprocess() override
    {
        return *this;
    }

    result_type operator()() const override
    {
        return {};
    }
};

#endif
