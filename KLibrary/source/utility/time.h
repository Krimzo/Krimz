#pragma once

#include <windows.h>

#include "math/math.h"


namespace kl
{
	namespace time
	{
		int64 get();
		int64 frequency();
		double calculate(int64 start, int64 end);

		double interval();
		void wait(double seconds);
		bool sleep(double seconds);

		struct date
		{
			int year;
			int month;
			int day;
			int hour;
			int minute;
			int second;

			date()
			{
				int64 time = std::time(nullptr);
				std::tm now = {};
				localtime_s(&now, &time);

				year = now.tm_year + 1900;
				month = now.tm_mon + 1;
				day = now.tm_mday;
				hour = now.tm_hour;
				minute = now.tm_min;
				second = now.tm_sec;
			}

			friend std::ostream& operator<<(std::ostream& stream, const date& date);
		};
	};

	class timer
	{
	private:
		int64 m_IntervalStart;
		int64 m_ElapsedStart;

	public:
		timer();

		void newInterval();
		double interval();

		void newElapsed();
		double elapsed() const;
	};
}
