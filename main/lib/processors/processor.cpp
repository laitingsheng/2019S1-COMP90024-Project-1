#include <boost/algorithm/string.hpp>

#include "processors/processor.h"

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

bool processor::less_tag_info(tag_info const & l, tag_info const & r)
{
    auto ls = l.second, rs = r.second;
    if (ls != rs)
        return ls > rs;
    return l.first < r.first;
}

bool processor::less_cell_total_info(cell_total_info const & l, cell_total_info const & r)
{
    auto lf = l.first, rf = r.first;
    auto lfs = lf.second, rfs = rf.second;
    if (lfs != rfs)
        return lfs > rfs;
    return lf.first < rf.first;
}

processor::processor(char const * filename, grid const & g) : file(filename), g(g) {}

void processor::process_block(char const * start, char const * end, record_type & record) const
{
    record.resize(g.count());
    while (start < end)
    {
        auto prev = start;
        while (*start++ != '\n');
        auto size = start - prev;
        std::string line(prev, size);
        process_line(line, record);
    }
}

void processor::process_line(std::string const & line, record_type & record) const
{
    std::smatch coord_match;
    if (!std::regex_search(line, coord_match, coord_rgx))
        return;

    double h, v;
    if (coord_match.str(1) == "geo")
    {
        v = std::stod(coord_match.str(2));
        h = std::stod(coord_match.str(3));
    }
    else
    {
        h = std::stod(coord_match.str(2));
        v = std::stod(coord_match.str(3));
    }

    auto pos = g.encode(v, h);
    if (pos == -1)
        return;

    // @formatter:off
    auto & [count, hash_tags_counts] = record[pos];
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

processor::cell_total_info processor::record_to_total_info(unsigned int pos) const
{
    cell_total_info re;
    // @formatter:off
    auto & [c, m] = record[pos];
    auto & [rc, rv] = re;
    // @formatter:on
    rc = {pos, c};
    if (c == 0)
        return re;
    rv.resize(m.size());
    auto it = rv.begin();
    for (auto & p : m)
    {
        *it = p;
        ++it;
    }
    std::sort(rv.begin(), rv.end(), less_tag_info);

    int vc = 0;
    unsigned long lc = 0;
    unsigned long j = 0;
    while (j < rv.size())
    {
        auto c = rv[j].second;
        if (c == 0)
        {
            lc = c;
            break;
        }
        if (c != lc)
        {
            ++vc;
            lc = c;
            if (vc > 5)
                break;
        }
        ++j;
    }
    if (lc != 0 && vc > 5)
        rv.resize(j);
    return re;
}
