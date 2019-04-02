/*
 * Author: Tingsheng (Tinson) Lai 781319
 */

// @formatter:off

#include <cstdio>

#include "include/grid.h"
#include "include/processors/multi_thread_processor.h"
#include "include/timer.h"

int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Minimum of 1 parameter is expected, %d was provided", argc - 1);
        return -1;
    }

    timer t;
    auto g = argc == 3 ? grid(argv[2]) : grid();
    t.print_duration("Grid read: ");

    multi_thread_processor p(argv[1], g);
    t.restart();
    p.preprocess();
    t.print_duration("Data preprocessed: ");

    t.restart();
    auto output = p();
    t.print_duration("Data processed: ");

    for (auto const & [k, c, _] : output)
        printf("%s: %lu\n", k.c_str(), c);
    for (auto const & [k, _, htp] : output)
    {
        printf("%s: (", k.c_str());
        auto it = htp.begin();
        for (; it != htp.end() - 1; ++it)
        {
            auto & [t, c] = *it;
            printf("(#%s, %lu)", t.c_str(), c);
        }
        auto & [t, c] = *it;
        printf("(#%s, %lu))\n", t.c_str(), c);
    }

    return 0;
}
