#include <cstdio>

#include "../include/timer.h"

void timer::restart()
{
    start = std::chrono::system_clock::now();
}

void timer::print_duration(char const * prefix)
{
    auto d = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
    double dd = d.count();
    printf("%s%.3lfs\n", prefix, dd / 1000);
}
