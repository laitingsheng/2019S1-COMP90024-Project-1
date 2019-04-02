#include <boost/algorithm/string.hpp>

#include "../../include/processors/processor.h"

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

    // @formatter:off
    auto & [count, hash_tags_counts] = record[{v, h}];
    // @formatter:on
    ++count;

    std::smatch hash_tags_search;
    if (!std::regex_search(line, hash_tags_search, hash_tags_rgx))
        return;

    std::smatch hash_tag_search;
    auto hash_tags_str = hash_tags_search[0].str();
    if (!std::regex_search(hash_tags_str, hash_tag_search, hash_tag_rgx))
        return;

    for (auto it = hash_tag_search.begin() + 1; it < hash_tag_search.end(); it += 2)
    {
        auto tmp = it->str();
        boost::algorithm::to_lower(tmp);
        ++hash_tags_counts[std::move(tmp)];
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

std::regex processor::coord_rgx(R"cr("coordinates":\[(-?\d*(?:\.\d*)?),(-?\d*(?:\.\d*)?)\])cr");
std::regex processor::hash_tags_rgx(R"htr("hashtags":\[(\{.+?\})(,\1)*?\])htr");
std::regex processor::hash_tag_rgx(R"htr("text":"(.*?)")htr");
