#ifndef _TIMER_H_
#define _TIMER_H_

#include <chrono>

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

    void restart();

    void print_duration(char const * prefix);
};

#endif
