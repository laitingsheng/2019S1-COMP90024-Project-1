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
    friend struct processor_tester;
};

std::regex processor::coord_rgx(R"cr("coordinates":\[(.+?),(.+?)\])cr");
std::regex processor::hash_tags_rgx(R"htr("hashtags":\[(\{.+?\})(,\1)*?\])htr");
std::regex processor::hash_tag_rgx(R"htr("text":"(.*?)")htr");

#endif
