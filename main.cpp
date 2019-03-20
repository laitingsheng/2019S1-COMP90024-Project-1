#include <cstdlib>

#include "omp.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::property_tree::ptree;
using boost::property_tree::json_parser::read_json;

int main(int argc, char * argv[])
{
    if (argc < 3)
    {
        printf("2 parameter is expected, %d was provided", argc - 1);
        return -1;
    }

    ptree grid;
    read_json(argv[1], grid);
    return 0;
}
