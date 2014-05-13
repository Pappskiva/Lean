#include "Clock.h"

Clock::Clock()
{
	time(&startTime);
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
	return timePerLevel - sek;
}