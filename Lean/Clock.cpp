#include "Clock.h"

Clock::Clock()
{
	time(&startTime);
	timeGoneBy = 0;
	timePerLevel = 100;
}

Clock::Clock(const Clock&)
{

}

Clock::~Clock()
{

}

int Clock::GetTime()
{
	time_t nowTime;
	time(&nowTime);
	int sek = difftime(nowTime, startTime);
	return timePerLevel - sek - timeGoneBy;
}

void Clock::PausClock()
{
	time_t nowTime;
	time(&nowTime);
	timeGoneBy = difftime(nowTime, startTime);
}

void Clock::UnPausClock()
{
	time(&startTime);
}

void Clock::RestartClock()
{
	time(&startTime);
	timeGoneBy = 0;
}