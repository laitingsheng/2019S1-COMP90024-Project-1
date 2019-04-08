#include "twit.h"

// @formatter:off
std::regex const twit_parser::line_rgx(
    R"lr(\{.*?(,"(value)":(?:.*"coordinates":\[(.*?),(.*?)\].*)),("(doc)":\{.*\})\})lr",
    std::regex::ECMAScript|std::regex::optimize|std::regex::collate
);
std::regex const twit_parser::hash_tag_rgx(
    R"htr(\s(#[[:alpha:]_]\w*))htr",
    std::regex::ECMAScript|std::regex::optimize|std::regex::collate
);
// @formatter:on

twit_parser const & twit_parser::get_parser() {
    return parser;
}

// @formatter:off
bool twit_parser::parse(
    std::string const & line,
    double & longitude,
    double & latitude,
    std::unordered_set<std::string> & hash_tags
)
// @formatter:on
{
    std::smatch whole_match;
    if (!std::regex_search(line, whole_match, line_rgx))
        return false;

    if (whole_match[2].str() == "value")
    {}
    else
    {}
}
