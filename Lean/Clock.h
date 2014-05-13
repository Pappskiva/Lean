////////////////////////////////////////////////////////////////////////////////
// Filename: Clock.h
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CLOCK_H
#define CLOCK_H

#include <time.h>

class Clock
{
protected:
public:
	Clock();
	Clock(const Clock&);
	~Clock();
	int GetTime();
	void PausClock();
	void UnPausClock();
	void RestartClock();

private:
	int timePerLevel;
	time_t startTime;
	int timeGoneBy;
};

#endif