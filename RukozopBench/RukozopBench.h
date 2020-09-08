#pragma once

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

//=========================================== Usage example ============================================//
/*
void myAwsomeFunction()
{ //... Stuff you need to measure ...// }

using namespace rukozop_bench;
printTime("My awsome function", measureExecTimeAvg<std::chrono::milliseconds>(myAwsomeFunction, 10000));
*/
//======================================================================================================//

#pragma region rkzp_defines

	#define RKZP_ITER_AMT_DEFAULT 10000
	
	#ifndef RKZP_UNIT_OF_TIME_ERROR_NOT_IMPLEMENTED
	#define RKZP_UNIT_OF_TIME_ERROR_NOT_IMPLEMENTED "Provide your own specialization for time unit scaling :("
	#endif

#pragma endregion

namespace rukozop_bench
{
	class TimeMeasurement
	{
	public:
		TimeMeasurement(long double time, const std::string& unit) :
			time_(time), unit_(unit)
		{}
		long double time_;
		std::string unit_;
	};

	#pragma region UnitOfTimeGeneral
	template<typename TU>
	struct UnitOfTime
	{
		long double operator()() const
		{
			static_assert(false, RKZP_UNIT_OF_TIME_ERROR_NOT_IMPLEMENTED);
			return 0;
		}
		std::string getSuffix() const
		{
			static_assert(false, RKZP_UNIT_OF_TIME_ERROR_NOT_IMPLEMENTED);
			return std::string();
		}
	};
	#pragma endregion
	
	#pragma region UnitsOfTimeSpecializations
	template<>
	struct UnitOfTime<std::chrono::nanoseconds>
	{
		long double operator()() const
		{
			return 1;
		}

		std::string getSuffix() const
		{
			return "ns";
		}
	};

	template<>
	struct UnitOfTime<std::chrono::microseconds>
	{
		long double operator()() const
		{
			return UnitOfTime<std::chrono::nanoseconds>()() * 1000;
		}

		std::string getSuffix() const
		{
			return "mus";
		}
	};

	template<>
	struct UnitOfTime<std::chrono::milliseconds>
	{
		long double operator()() const
		{
			return UnitOfTime<std::chrono::microseconds>()() * 1000;
		}

		std::string getSuffix() const
		{
			return "ms";
		}
	};

	template<>
	struct UnitOfTime<std::chrono::seconds>
	{
		long double operator()() const
		{
			return UnitOfTime<std::chrono::milliseconds>()() * 1000;
		}

		std::string getSuffix() const
		{
			return "s";
		}
	};

	template<>
	struct UnitOfTime<std::chrono::minutes>
	{
		long double operator()() const
		{
			return UnitOfTime<std::chrono::seconds>()() * 60;
		}

		std::string getSuffix() const
		{
			return "min";
		}
	};

	template<>
	struct UnitOfTime<std::chrono::hours>
	{
		long double operator()() const
		{
			return UnitOfTime<std::chrono::minutes>()() * 60;
		}

		std::string getSuffix() const
		{
			return "h";
		}
	};
		
	#pragma endregion
	
	
	class TimeDiff
	{
	public:
		TimeDiff(const std::chrono::time_point<std::chrono::steady_clock>& begin,
			const std::chrono::time_point<std::chrono::steady_clock>& end)
		{
			m_timeSpan = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
		}

		template<typename TU>
		long double getSpan() const
		{
			return getSpanNS() / UnitOfTime<TU>()();
		}

		std::chrono::nanoseconds getTimeSpan() const
		{
			return m_timeSpan;
		}

	private:

		long long int getSpanNS() const
		{
			return m_timeSpan.count();
		}

		std::chrono::nanoseconds m_timeSpan;
	};

	class TimeSpans
	{
	public:
		TimeSpans(unsigned long long cap = 1) : m_timePoints({})
		{
			m_timePoints.reserve(cap);
		}

		void add(TimeDiff& point)
		{
			m_timePoints.push_back(point);
		}

		void add(TimeDiff&& point)
		{
			m_timePoints.push_back(std::move(point));
		}

		template<typename TU>
		long double getAverage() const
		{
			long double total = 0;
			auto size = m_timePoints.size();
			for(const auto& point : m_timePoints)
			{
				total += point.getSpan<TU>();
			}

			return size ? total / size : total;
		}

		template<typename TU>
		std::string getUnitSuffix() const
		{
			return UnitOfTime<TU>().getSuffix();
		}

		template<typename TU>
		TimeMeasurement getTimeMeasurement() const
		{
			return TimeMeasurement(getAverage<TU>(), getUnitSuffix<TU>());
		}

	private:
		std::vector<TimeDiff> m_timePoints;
	};


	#pragma region measurementFuncionsTemplates
	template<typename F>
	TimeDiff measureExecTime(const F& func)
	{
		auto t1 = std::chrono::high_resolution_clock::now();
		func();
		auto t2 = std::chrono::high_resolution_clock::now();

		return TimeDiff(t1, t2);

	}

	template<typename TU = std::chrono::milliseconds, typename F>
	TimeMeasurement measureExecTimeAvg(const F& func, unsigned long long avgOver = RKZP_ITER_AMT_DEFAULT)
	{
		TimeSpans ts = TimeSpans(avgOver);

		for(unsigned long long i = 0; i < avgOver; ++i)
		{
			ts.add(std::move(measureExecTime(func)));
		}

		return ts.getTimeMeasurement<TU>();
	}
	#pragma endregion


	void printTime(const std::string& fname, const TimeMeasurement& measurement)
	{
		auto originalPrecision = std::cout.precision();
		std::cout.precision(12);

		std::cout << std::fixed << fname << " took on average " << measurement.time_ << " " << measurement.unit_ << std::endl;

		std::cout.precision(originalPrecision);
	}
};

#pragma region rkzp_undefs

	#undef RKZP_ITER_AMT_DEFAULT
	#undef RKZP_UNIT_OF_TIME_ERROR_NOT_IMPLEMENTED

#pragma endregion
