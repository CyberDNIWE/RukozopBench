[![CyberDNIWE](https://i.imgur.com/tvIphsZ.png "Fork me @github")](https://github.com/CyberDNIWE/)
# RukozopBench
[RukozopBench](https://github.com/CyberDNIWE/RukozopBench) is a simple quick-and-dirty **cross-platform** **one-header-only** benchmarking tool to quickly and wrongly measure performance in units of time (instead of CPU cycles, like normal people).

## Installation
* Checkout repo
* Copy paste `RukozopBench.h` into your include path
* Profit 
## Running example
* Open `CMakeLists.txt` in your favourite IDE
* Build & Run
* Be amazed! (but not really)

## Usage examples
###### from `RukozopBenchExample.cpp`
```c++
#include <RukozopBench.h>
//For all you -nix people out there
#ifndef WIN32
#include <unistd.h>
#endif

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
```

## Extending with custom units of time
* Specialize `UnitOfTime` template on your custom unit of time
* All units of time are weighted against `nanosecond`
  * `operator()` returns divider by which measured time (in nanoseconds) is divided
```c++

template<>
struct rukozop_bench::UnitOfTime<my::awsome::time::unit>
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