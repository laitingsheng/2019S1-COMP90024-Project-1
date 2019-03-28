#ifndef _PROCESSOR_HPP_
#define _PROCESSOR_HPP_

#include <sys/stat.h>

#include <regex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "../grid.hpp"

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

    virtual void process_line(std::string const & line, record_type & record) final
    {
        std::smatch coord_search;
        if (!std::regex_search(line, coord_search, coord_rgx))
            return;

        auto h = g.get_horizontal(std::stod(coord_search[1].str()));
        if (!h)
            return;

        auto v = g.get_vertical(std::stod(coord_search[2].str()));
        if (!v)
            return;

        if (!g.validate(v, h))
            return;

        auto & [count, hash_tags_counts] = record[{v, h}];
        // @formatter:on
        ++count;

        std::smatch hash_tags_search;
        if (!std::regex_search(line, hash_tags_search, hash_tags_rgx))
            return;

        std::smatch hash_tag_search;
        std::string hash_tags_str = hash_tags_search[0].str();
        if (!std::regex_search(hash_tags_str, hash_tag_search, hash_tag_rgx))
            return;

        for (auto it = hash_tag_search.begin() + 1; it < hash_tag_search.end(); it += 2)
        {
            auto tmp = it->str();
            boost::algorithm::to_lower(tmp);
            ++hash_tags_counts[std::move(tmp)];
        }
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

    std::string filename;
    struct stat st;
    grid const & g;
    record_type record;

    explicit processor(char const * filename, grid const & g) : g(g), filename(filename)
    {
        stat(filename, &st);
    }
private:
    friend struct processor_tester;
};

std::regex processor::coord_rgx(R"cr("coordinates":\[(.+?),(.+?)\])cr");
std::regex processor::hash_tags_rgx(R"htr("hashtags":\[(\{.+?\})(,\1)*?\])htr");
std::regex processor::hash_tag_rgx(R"htr("text":"(.*?)")htr");

#endif
