#pragma once

#include <chrono>
#include <sstream>
#include <iostream>
#include <string>

class Timer {
public:
    Timer();
    Timer(uint32_t sec);

    std::string getFancyTime() const;

    void updateTimer();

    void restart();

    uint32_t getSeconds() const;

    bool finished;
private:
    std::chrono::steady_clock::time_point start_time;

    std::chrono::milliseconds duration;
};

