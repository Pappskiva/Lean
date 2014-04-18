//=======================================================================================
// Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#ifndef APPLICATIONTIMER_H
#define APPLICATIONTIMER_H


class ApplicationTimer
{
public:
	ApplicationTimer();

	float getGameTime()const;  // in seconds
	float getDeltaTime()const; // in seconds

	void reset(); // Call before message loop.
	void start(); // Call when unpaused.
	void stop();  // Call when paused.
	void tick();  // Call every frame.

private:
	double mSecondsPerCount;
	double mDeltaTime;

	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mStopped;
};

#endif // APPLICATIONTIMER_H