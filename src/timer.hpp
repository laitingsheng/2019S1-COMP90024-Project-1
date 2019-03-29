#ifndef _TIMER_HPP_
#define _TIMER_HPP_

#include <cstdio>

#include <chrono>
#include <string>

class timer final
{
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
public:
    timer() = default;
    ~timer() = default;

    timer(timer const &) = delete;
    timer(timer &&) = delete;
    timer & operator=(timer const &) = delete;
    timer & operator=(timer &&) = delete;

    void restart()
    {
        start = std::chrono::system_clock::now();
    }

    void print_duration(std::string const & prefix)
    {
        auto d = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
        double dd = d.count();
        printf("%s%.3lfs\n", prefix.c_str(), dd / 1000);
    }
};

#endif
