#include "utility/time.h"


// Time
int64 kl::time::get()
{
	LARGE_INTEGER time = {};
	QueryPerformanceCounter(&time);
	return time.QuadPart;
}
int64 kl::time::frequency()
{
	LARGE_INTEGER frequency = {};
	QueryPerformanceFrequency(&frequency);
	return frequency.QuadPart;
}
double kl::time::calculate(int64 start, int64 end)
{
	static const double recFrequency = 1.0 / kl::time::frequency();
	return (end - start) * recFrequency;
}
double kl::time::interval()
{
	static int64 startTime = kl::time::get();
	const int64 endTime = kl::time::get();
	const double elapsedTime = kl::time::calculate(startTime, endTime);
	startTime = endTime;
	return elapsedTime;
}
void kl::time::wait(double seconds)
{
	const int64 startTime = kl::time::get();
	int64 endTime = kl::time::get();
	while (kl::time::calculate(startTime, endTime) < seconds)
	{
		endTime = kl::time::get();
	}
}
bool kl::time::sleep(double seconds)
{
	HANDLE timer = CreateWaitableTimerA(nullptr, true, nullptr);
	if (!timer)
	{
		return false;
	}

	static const int64 frequency = kl::time::frequency();
	const int64 toSleep = -int64(seconds * frequency);
	if (!SetWaitableTimer(timer, (LARGE_INTEGER*) &toSleep, 0, nullptr, nullptr, false))
	{
		CloseHandle(timer);
		return false;
	}

	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
	return true;
}

std::ostream& operator<<(std::ostream& stream, const kl::time::date& date)
{
	stream << date.year <<
		"." << date.month <<
		"." << date.day <<
		"-" << date.hour <<
		":" << date.minute <<
		":" << date.second;
	return stream;
}

// Timer
kl::timer::timer()
{
	newInterval();
	newElapsed();
}

void kl::timer::newInterval()
{
	m_IntervalStart = kl::time::get();
}

double kl::timer::interval()
{
	return kl::time::calculate(m_IntervalStart, kl::time::get());
}

void kl::timer::newElapsed()
{
	m_ElapsedStart = kl::time::get();
}
double kl::timer::elapsed() const
{
	return kl::time::calculate(m_ElapsedStart, kl::time::get());
}
