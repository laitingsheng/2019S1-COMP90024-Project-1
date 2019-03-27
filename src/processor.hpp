#ifndef _PROCESSOR_HPP_
#define _PROCESSOR_HPP_

#include <sys/stat.h>

#include <fstream>
#include <regex>
#include <unordered_map>
#include <utility>

#include "grid.hpp"

struct processor
{
    using result_type = std::vector<std::pair<std::string, std::vector<std::string>>>;

    processor() = delete;
    processor(processor const &) = delete;
    processor(processor &&) = delete;
    processor & operator=(processor const &) = delete;
    processor & operator=(processor &&) = delete;

    virtual result_type operator()() = 0;

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
private:
    template<typename P>
    friend struct processor_tester;
};

std::regex processor::coord_rgx(R"cr("coordinates":\[(.+?),(.+?)\])cr");
std::regex processor::hash_tags_rgx(R"htr("hashtags":\[(\{.+?\})(,\1)*?\])htr");
std::regex processor::hash_tag_rgx(R"htr("text":"(.*?)")htr");

struct single_thread_processor final : public processor
{
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

            auto v = g.get_vertical(std::stod(coord_search[1].str()));
            if (!v)
                continue;

            auto h = g.get_horizontal(std::stod(coord_search[2].str()));
            if (!h)
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
            std::string hash_tags_str = hash_tag_search[0].str();
            if (!std::regex_search(hash_tags_str, hash_tag_search, hash_tag_rgx))
                continue;

            for (auto & match : hash_tag_search)
                ++hash_tags_counts[match.str()];
        }

        return *this;
    }
};

struct multi_thread_processor final : public processor
{
};

struct multi_node_processor final : public processor
{
};

#endif
