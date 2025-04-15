#include "../include/Timer.hpp"

Timer::Timer() : start_time(std::chrono::steady_clock::now()),
duration(60 * 1000), finished(false) {
}

//mnozi sec da pridejo milisekunde
Timer::Timer(uint32_t sec) : start_time(std::chrono::steady_clock::now()), 
duration(sec * 1000), finished(false) {
}

std::string Timer::getFancyTime() const {
    if (finished) {
        return "00:00";
    }

    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time);

    auto remaining = duration - elapsed;
    if (remaining.count() < 0) {
        remaining = std::chrono::milliseconds(0);
    }

    auto total_seconds = std::chrono::duration_cast<std::chrono::seconds>(remaining).count();
    int minutes = static_cast<int>(total_seconds / 60);
    int seconds_remaining = static_cast<int>(total_seconds % 60);

    std::string minutes_str = (minutes < 10) ? "0" + std::to_string(minutes) : std::to_string(minutes);
    std::string seconds_str = (seconds_remaining < 10) ? "0" + std::to_string(seconds_remaining) : std::to_string(seconds_remaining);

    return minutes_str + ":" + seconds_str;
}

void Timer::updateTimer() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time);

    if (elapsed >= duration) {
        finished = true;
    }
}

void Timer::restart() {
    start_time = std::chrono::steady_clock::now();
    finished = false;
    std::cout << "timer restarted\n";
}

uint32_t Timer::getSeconds() const {
    return static_cast<uint32_t>(duration.count() / 1000);
}