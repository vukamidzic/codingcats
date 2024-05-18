#include "timer.h"
#include <stdbool.h>

Timer createTimer(double life_time) {
    Timer timer = {0};
    timer.start_time = GetTime();
    timer.life_time = life_time;

    return timer;
}

bool timeout(Timer* timer) {
    double currTime = GetTime();

    if (currTime - timer->start_time >= timer->life_time) {
        timer->start_time = currTime;
        return true;
    }

    return false;
}