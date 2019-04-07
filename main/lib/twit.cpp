#include "twit.h"

// @formatter:off
std::regex const twit_parser::line_rgx(
    R"lr(\{.*?(,"(value)":(?:.*"coordinates":\[(-?\d+(?:.\d+)),(-?\d+(?:.\d+))\].*))?,("(doc)":\{.*\})\})lr",
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

twit_info twit_parser::parse(std::string const &)
{
}
