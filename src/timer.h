#ifndef __TIMER_H__
#define __TIMER_H__
//reference on measure frame time in millisecond
//https://stackoverflow.com/questions/28530798/how-to-make-a-basic-fps-counter

#include <chrono>

double clock_to_milliseconds(clock_t tick)
{
	return (tick / (double)CLOCKS_PER_SEC) * 1000.f;
}

float average_frame_time(float time, int frame)
{
	return time / frame;
}

#endif