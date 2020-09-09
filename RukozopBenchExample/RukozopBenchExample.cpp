#include <RukozopBench.h>


// The function we'd like to measure
void measureMe(const std::string& msg, int sleepTimeMS)
{
	#ifdef WIN32
	_sleep(sleepTimeMS);
	#else
	sleep(sleepTimeMS);
	#endif
	//std::cout << __func__ <<" was given message to print: " << msg << " and sleep time of: " << sleepTimeMS << " ms" << std::endl;
}

int main(int argc, char* argv[])
{
	using namespace rukozop_bench;

	std::cout << "We will now test different usage scenarios on essentially singular example (each test may take some time)" << std::endl
		<< "Working..." << std::endl;
	// Default way:
	printTime("My awsome function, tested for RKZP_ITER_AMT_DEFAULT repetitions with RKZP_ACCURACY_DEFAULT accuracy", measureExecTimeAvg(measureMe, "Hello there!", 1));

	// Specifying accuracy manually:
	printTime("My awsome function, tested for RKZP_ITER_AMT_DEFAULT repetitions, with millisecond accuracy", measureExecTimeAvg<rukozop_bench::milliseconds>(measureMe, "Hello there!", 1));

	// Specifying test runs amount manually:
	printTime("My awsome function, tested for 2 repetitions with default accuracy", measureExecTimeAvg<2>(measureMe, "Hello there!", 1));

	// Specifying both accuracy and amount of test runs manually:
	printTime("My awsome function, tested for 2 repetitions, with millisecod accuracy", measureExecTimeAvg<rukozop_bench::milliseconds, 2>(measureMe, "Hello there!", 1));

	// Any of above can also be used with lambdas: (written in multiple lines for clarity)
	auto l = [&](const std::string& a, int b){ return measureMe(a, b); };
	auto timeMeasurement = measureExecTimeAvg(l, "Hello there!", 1);
	printTime("Lambda, RKZP_ITER_AMT_DEFAULT repetitions, RKZP_ACCURACY_DEFAULT accuracy", timeMeasurement);
	

	// We done here!
	std::cout << "Demo completed!" << std::endl;
	std::cin.get();

}

