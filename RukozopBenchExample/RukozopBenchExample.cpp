#include <stdio.h>
#include <RukozopBench.h>


//Usage: see RukozopBench.h

void measureMe()
{
	unsigned int time = 1;
	std::cout << __func__ <<": I'm going to sleep for " << time << "seconds" << std::endl;
	#ifdef WIN32
	_sleep(time * 1000);
	#else
	sleep(time * 1000);
	#endif
	std::cout << __func__ << ": I woke up!" << std::endl;
}

using namespace rukozop_bench;
int main(int argc, char* argv[])
{
	printTime("My awsome function", measureExecTimeAvg<std::chrono::milliseconds>(measureMe, 2));

	getchar();

}

