#pragma once

#define USE_WIN_HPT

#ifdef USE_WIN_HPT
	#include <Windows.h>
#else
	#include <chrono>
#endif

#ifdef USE_WIN_HPT
class PerfTimer
{
private:
	LARGE_INTEGER _freq;
	LARGE_INTEGER _startTime;
	LARGE_INTEGER _endTime;

public:
	PerfTimer()
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
		double micro = static_cast<double>(_endTime.QuadPart - _startTime.QuadPart);
		micro *= 1000; // freq is num of ticks per second, mul by 1000 to get milisec
		micro /= static_cast<double>(_freq.QuadPart);
		if (milisec)
			*milisec = micro;
	}
};

#else

// note that on VS2013/VS2012 implementation of high_res_timer is
// not high res actually...
class PerfTimer
{
private:
	std::chrono::high_resolution_clock::time_point _start;
public:
	void start()
	{
		_start = std::chrono::high_resolution_clock::now();
	}

	void stop(double *milisec)
	{
		auto end = std::chrono::high_resolution_clock::now();
		double micro = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - _start).count());
		micro *= 1000; // mul to get microsec, better accuracy
		if (milisec)
			*milisec = micro;
	}
};

#endif