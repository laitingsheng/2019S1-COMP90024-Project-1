#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_

#include <regex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/iostreams/device/mapped_file.hpp>

#include "../grid.h"

struct processor
{
    using cell_tag_info = std::pair<std::string, unsigned long>;
    using cell_info = std::tuple<std::string, unsigned long, std::vector<cell_tag_info>>;
    using result_type = std::vector<cell_info>;

    processor(processor const &) = delete;

    processor(processor &&) = delete;

    processor & operator=(processor const &) = delete;

    processor & operator=(processor &&) = delete;

    virtual processor & preprocess() = 0;

    virtual result_type operator()() const = 0;

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

    static std::regex const coord_rgx, hash_tags_rgx, hash_tag_rgx;

    static void merge_records(record_type & to, record_type && from);

    static bool less_cell_tag_info(cell_tag_info const & l, cell_tag_info const & r);

    static bool less_cell_info(cell_info const & l, cell_info const & r);

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
