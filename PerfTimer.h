#pragma once

#include <Windows.h>

class PerfTimer
{
private:
	LARGE_INTEGER _freq;
	LARGE_INTEGER _startTime;
	LARGE_INTEGER _endTime;
public:
	explicit PerfTimer()
	{
		QueryPerformanceFrequency(&_freq);
	}

	void start()
	{
		QueryPerformanceCounter(&_startTime);
	}

	void stop(double *milisec)
	{
		QueryPerformanceCounter(&_endTime);
		double micro;
		micro = static_cast<double>(_endTime.QuadPart - _startTime.QuadPart);
		micro *= 1000;
		micro /= static_cast<double>(_freq.QuadPart);
		if (milisec)
			*milisec = micro;
	}
};