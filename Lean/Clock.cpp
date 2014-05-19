#include "Clock.h"

Clock::Clock()
{
	time(&startTime);
	timeGoneBy = 0.0f;
	timePerLevel = 100;
}

Clock::Clock(const Clock&)
{

}

Clock::~Clock()
{

}

int Clock::GetTime(bool pause)
{
	time_t nowTime;
	time(&nowTime);
	int result = timePerLevel - timeGoneBy;
	if (pause)
	{
		int sek = difftime(nowTime, startTime);
		result -= sek;
	}
	if (result < 0)
	{
		result = 0;
	}
	return result;
}

void Clock::PausClock()
{
	time_t nowTime;
	time(&nowTime);
	timeGoneBy += difftime(nowTime, startTime);
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