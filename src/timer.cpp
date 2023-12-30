#include "libclean/timer.hpp"

int Timer::time = 0;

Timer::Timer() 
{};

void Timer::addTime() {
    time += 1;
};

int Timer::getTime() {
    return time;
};

