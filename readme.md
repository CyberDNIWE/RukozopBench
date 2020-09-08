[![CyberDNIWE](https://avatars3.githubusercontent.com/u/19151176?s=120&v=4 "Fork me @github")](https://github.com/CyberDNIWE/)
# RukozopBench
[RukozopBench](https://github.com/CyberDNIWE/RukozopBench) is a simple quick-and-dirty **one-header-only** benchmarking tool to quickly and wrongly measure performance in units of time (instead of CPU cycles, like normal people).

## Installation
* Checkout repo
* Copy paste `RukozopBench.h` into your include path
* Profit 
## Running example
* Open `CMakeLists.txt` in your favourite IDE
* Build & Run
* Be amazed! (but not really)

## Usage examples
```c++
//#include <stdio.h>
#include <RukozopBench.h>

// The function we'd like to measure
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
    //Any of std::chrono time units work
    printTime("My awsome function", measureExecTimeAvg<std::chrono::milliseconds>(measureMe, 2));
    //printTime("My awsome function", measureExecTimeAvg<std::chrono::microseconds>(measureMe, 2));
    //printTime("My awsome function", measureExecTimeAvg<std::chrono::nanoseconds>(measureMe, 2));
    //It is also possible to make custom time units, see "UnitOfTimeGeneral" region in RukozopBench.h

    //getchar();
}
```

## Extending with custom units of time
* Specialize `UnitOfTime` template on your custom unit of time
* All units of time are weighted against `nanosecond`
  * `operator()` returns divider by which measured time (in nanoseconds) is divided
```c++

template<>
struct UnitOfTime<my::awsome::time::unit>
{
	long double operator()() const
	{
		return 69;
	}
	std::string getSuffix() const
	{
		return "SixtyNinesOfNs";
	}
};
```


## Authors
* [CyberDNIWE](https://github.com/CyberDNIWE/)  | <battleseal@hotmail.com>


---
###### "Maybe you should try getting a job..." - J.C. Denton, 2052