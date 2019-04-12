#ifndef COMP90024_PROJECT_1_PROCESSOR_H
#define COMP90024_PROJECT_1_PROCESSOR_H

#include <regex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/iostreams/device/mapped_file.hpp>

#include "../grid.h"

struct processor
{
    using cell_info = std::pair<unsigned int, unsigned long>;
    using tag_info = std::pair<std::string, unsigned long>;
    using cell_total_info = std::pair<cell_info, std::vector<tag_info>>;
    using result_type = std::vector<cell_total_info>;

    virtual ~processor() = default;

    processor(processor const &) = delete;

    processor(processor &&) = delete;

    processor & operator=(processor const &) = delete;

    processor & operator=(processor &&) = delete;

    virtual void preprocess() = 0;

    virtual result_type operator()() const = 0;

protected:
    using record_type = std::vector<std::pair<unsigned long,std::unordered_map<std::string, unsigned long>>>;

    static std::regex const coord_rgx, hash_tags_rgx, hash_tag_rgx;

    static bool less_tag_info(tag_info const & l, tag_info const & r);

    static bool less_cell_total_info(cell_total_info const & l, cell_total_info const & r);

    grid const & g;
    boost::iostreams::mapped_file_source file;
    record_type record;

    explicit processor(char const * filename, grid const & g);

    virtual void process_line(std::string const & line, record_type & record) const final;

    virtual void process_block(char const * start, char const * end, record_type & record) const final;

private:
    friend struct processor_tester;
};


#endif
