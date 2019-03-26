#ifndef _PROCESS_HPP_
#define _PROCESS_HPP_

#include <sys/stat.h>

#include <regex>

struct processor
{
    using result_type = std::vector<std::pair<std::string, std::vector<std::string>>>;

    virtual result_type operator()() = 0;
protected:
    static std::regex coord_rgx, hash_tags_rgx, hash_tag_rgx;
    std::string filename;
    struct stat st;

    explicit processor(char const * filename) : filename(filename)
    {
        stat(filename, &st);
    }
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
