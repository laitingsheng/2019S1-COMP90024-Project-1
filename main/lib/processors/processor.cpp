#include <boost/algorithm/string.hpp>

#include "processors/processor.h"

void processor::merge_records(record_type & to, record_type && from)
{
    // @formatter:off
    for (auto & [k, v] : from)
    // @formatter:on
        if (to.count(k))
        {
            // @formatter:off
            auto & [tk, tv] = to[k];
            auto & [ik, iv] = v;
            // @formatter:on
            tk += ik;
            // @formatter:off
            for (auto & [iik, iiv] : iv)
            // @formatter:on
                tv[iik] += iiv;
        }
        else
            to[k] = std::move(v);
}

bool processor::less_cell_tag_info(cell_tag_info const & l, cell_tag_info const & r)
{
    auto ls = l.second, rs = r.second;
    if (ls != rs)
        return ls > rs;
    return l.first < r.first;
}

bool processor::less_cell_info(cell_info const & l, cell_info const & r)
{
    auto ls = std::get<1>(l), rs = std::get<1>(r);
    if (ls != rs)
        return ls > rs;
    return std::get<0>(l) < std::get<0>(r);
}

processor::processor(char const * filename, grid const & g) : file(filename), g(g) {}

void processor::process_line(std::string const & line, record_type & record) const
{
    std::smatch coord_match;
    if (!std::regex_search(line, coord_match, coord_rgx))
        return;

    char h, v;
    if (coord_match.str(1) == "geo")
    {
        if (!(v = g.get_vertical(std::stod(coord_match.str(2)))))
            return;
        if (!(h = g.get_horizontal(std::stod(coord_match.str(3)))))
            return;
    }
    else
    {
        if (!(h = g.get_horizontal(std::stod(coord_match.str(2)))))
            return;
        if (!(v = g.get_vertical(std::stod(coord_match.str(3)))))
            return;
    }

    if (!g.validate(v, h))
        return;

    // @formatter:off
    auto & [count, hash_tags_counts] = record[{v, h}];
    // @formatter:on
    ++count;

    std::smatch hash_tags_search;
    if (!std::regex_search(line, hash_tags_search, hash_tags_rgx))
        return;

    auto hts = line.begin() + hash_tags_search.position(), hte = hts + hash_tags_search.length();
    auto start = std::sregex_token_iterator(hts, hte, hash_tag_rgx, 1);
    auto const end = std::sregex_token_iterator();
    while (start != end)
    {
        auto tmp = start->str();
        boost::algorithm::to_lower(tmp);
        ++hash_tags_counts[std::move(tmp)];
        ++start;
    }
}

void processor::process_block(char const * start, char const * end, record_type & record) const
{
    while (start < end)
    {
        auto prev = start;
        while (*start++ != '\n');
        auto size = start - prev;
        std::string line(prev, size);
        process_line(line, record);
    }
}

// @formatter:off
std::regex const processor::coord_rgx(
    R"vcr(\"(geometry|coordinates|geo)\":\{.*?\"coordinates\":\[(.*?),(.*?)\].*?\})vcr",
    std::regex::ECMAScript | std::regex::optimize | std::regex::collate
);
std::regex const processor::hash_tags_rgx(
    R"htr(\"hashtags\":\[(?:\,?\{.*?\"text\":\".*?\".*?\})+\])htr",
    std::regex::ECMAScript | std::regex::optimize | std::regex::collate
);
std::regex const processor::hash_tag_rgx(
    R"htr(\"text\":\"(.*?)\")htr",
    std::regex::ECMAScript | std::regex::optimize | std::regex::collate
);
// @formatter:on
