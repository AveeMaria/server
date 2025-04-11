#include "../include/Timer.hpp"

Timer::Timer()
{
	start_time = SDL_GetTicks();
	seconds = 60 * 1000;
	finished = false;
}

Timer::Timer(uint32_t sec)
{
	start_time = SDL_GetTicks();
	seconds = sec * 1000;//kr je v ms
	finished = false;
}

std::string Timer::getFancyTime() const {
	if (finished) {		
		return "00:00";
	}

	uint32_t time = (seconds - (start_time + curr_time)) / 1000;

	std::stringstream ss;
	ss << time / 60 << ":";

	//ne deletat (da so tud minute prov formated)
	//time / 60 < 10 ? ss << "0" << time / 60 : ss << time / 60; ss << ":";

	time % 60 < 10 ? ss << "0" << time % 60 : ss << time % 60;

	return ss.str();
}

void Timer::updateTimer() {
	curr_time = SDL_GetTicks();
	
	if (curr_time + start_time >= seconds) {
		finished = true;
	}
}

void Timer::restart()
{
	start_time = SDL_GetTicks();
	finished = false;
	std::cout << "timer restarted\n";
}
