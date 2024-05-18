#ifndef TIMER_H
#define TIMER_H

#include <raylib.h>
#include <stdbool.h>

typedef struct Timer {
    double start_time;
    double life_time;
} Timer;

Timer createTimer(double life_time);
bool timeout(Timer* timer);

#endif