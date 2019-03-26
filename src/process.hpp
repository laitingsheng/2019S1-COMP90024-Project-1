#ifndef _PROCESS_HPP_
#define _PROCESS_HPP_

#include <sys/stat.h>

#include <fstream>
#include <regex>
#include <unordered_map>
#include <utility>

struct processor
{
    using result_type = std::vector<std::pair<std::string, std::vector<std::string>>>;

    virtual result_type operator()() = 0;
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
    record_type record;

    explicit processor(char const * filename) : filename(filename)
    {
        stat(filename, &st);
    }
private:
    virtual void preprocess() = 0;
};

std::regex processor::coord_rgx(R"cr("coordinates":\[(.+?),(.+?)\])cr");
std::regex processor::hash_tags_rgx(R"htr("hashtags":\[(.+?)(,\1)*?\])htr");
std::regex processor::hash_tag_rgx(R"htr()htr");

struct single_thread_processor final : public processor
{
    explicit single_thread_processor(char const * filename) : processor(filename) {}

    result_type operator()() override
    {
        return {};
    };
private:
    void preprocess() override
    {
        std::ifstream twit_file(filename);
        while (!twit_file.eof())
        {
            std::string buff;
            std::getline(twit_file, buff);

            std::smatch coord_search;
            if (!std::regex_search(buff, coord_search, coord_rgx))
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

            std::string coord_desc {vc, hc};
            if (!valid.count(coord_desc))
                continue;
        }
    }
};

struct multi_thread_processor final : public processor
{
    explicit multi_thread_processor(char const * filename) : processor(filename) {}
};

struct multi_node_processor final : public processor
{
    explicit multi_node_processor(char const * filename) : processor(filename) {}
};

#endif
