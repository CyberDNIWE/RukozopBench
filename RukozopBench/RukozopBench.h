#pragma once

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

//=========================================== Usage example ============================================//
/*
void myAwsomeFunction(int param1, std::string param2)
{ //... Stuff you need to measure ...// }

using namespace rukozop_bench;
printTime("My awsome function", measureExecTimeAvg<rukozop_bench::milliseconds, 10000>(myAwsomeFunction, 69, "Hello, world"));
*/
//======================================================================================================//

#pragma region rkzp_defines

	#define RKZP_ITER_AMT_DEFAULT 10000
	
	#ifndef RKZP_UNIT_OF_TIME_ERROR_NOT_IMPLEMENTED
	#define RKZP_UNIT_OF_TIME_ERROR_NOT_IMPLEMENTED "Provide your own specialization for time unit scaling :("
	#endif

	#ifndef RKZP_ACCURACY_DEFAULT
	#define RKZP_ACCURACY_DEFAULT rukozop_bench::milliseconds
	#endif

#pragma endregion

namespace rukozop_bench
{
	#pragma region typedefs
	
	typedef		std::chrono::nanoseconds	nanoseconds;
	typedef		std::chrono::microseconds	microseconds;
	typedef		std::chrono::milliseconds	milliseconds;
	typedef		std::chrono::seconds		seconds;
	typedef		std::chrono::minutes		minutes;
	typedef		std::chrono::hours			hours;

	#pragma endregion

	
	class TimeMeasurement
	{
	public:
		template<typename TIMECOUNT = long double, typename UNITDESCR = std::string>
		TimeMeasurement(TIMECOUNT time, const UNITDESCR& unit) :
			time_(static_cast<decltype(time_)>(time)), unit_(unit)
		{}

		long double time_;
		std::string unit_;
	};

	#pragma region UnitOfTimeGeneral
	//Forward declare templated type to be specialized later
	template<typename TU>
	struct UnitOfTime;
	#pragma endregion
	
	#pragma region UnitsOfTimeSpecializations
	template<>
	struct UnitOfTime<nanoseconds>
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
	struct UnitOfTime<microseconds>
	{
		long double operator()() const
		{
			return UnitOfTime<nanoseconds>()() * 1000;
		}

		std::string getSuffix() const
		{
			return "mus";
		}
	};

	template<>
	struct UnitOfTime<milliseconds>
	{
		long double operator()() const
		{
			return UnitOfTime<microseconds>()() * 1000;
		}

		std::string getSuffix() const
		{
			return "ms";
		}
	};

	template<>
	struct UnitOfTime<seconds>
	{
		long double operator()() const
		{
			return UnitOfTime<milliseconds>()() * 1000;
		}

		std::string getSuffix() const
		{
			return "s";
		}
	};

	template<>
	struct UnitOfTime<minutes>
	{
		long double operator()() const
		{
			return UnitOfTime<seconds>()() * 60;
		}

		std::string getSuffix() const
		{
			return "min";
		}
	};

	template<>
	struct UnitOfTime<hours>
	{
		long double operator()() const
		{
			return UnitOfTime<minutes>()() * 60;
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
		template<typename TP>
		TimeDiff(const TP& begin, const TP& end)
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
	template<typename F, typename ...ARGS>
	TimeDiff measureExecTime(const F& func, ARGS && ...args)
	{
		auto t1 = std::chrono::high_resolution_clock::now();
		func(std::forward<ARGS>(args)...);
		auto t2 = std::chrono::high_resolution_clock::now();

		return TimeDiff(t1, t2);

	}

	template<typename TU, unsigned long long AVGOVER, typename F, typename ...ARGS>
	TimeMeasurement measureExecTimeAvg(const F& func, ARGS&& ...args)
	{
		TimeSpans ts = TimeSpans(AVGOVER);

		for(unsigned long long i = 0; i < AVGOVER; ++i)
		{
			ts.add(std::move(measureExecTime(func, std::forward<ARGS>(args)...)));
		}

		return ts.getTimeMeasurement<TU>();
	}

	template<unsigned long long AVGOVER, typename F, typename ...ARGS>
	TimeMeasurement measureExecTimeAvg(const F& func, ARGS&& ...args)
	{
		return measureExecTimeAvg<RKZP_ACCURACY_DEFAULT, AVGOVER>(func, std::forward<ARGS>(args)...);
	}

	template<typename TU = RKZP_ACCURACY_DEFAULT, typename F, typename ...ARGS>
	TimeMeasurement measureExecTimeAvg(const F& func, ARGS&& ...args)
	{
		return measureExecTimeAvg<TU, RKZP_ITER_AMT_DEFAULT>(func, std::forward<ARGS>(args)...);
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
	#undef RKZP_ACCURACY_DEFAULT

#pragma endregion
