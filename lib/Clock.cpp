#include "Clock.hpp"
#include <stdio.h>

void Clock::start()
{
    tpStart_m = std::chrono::steady_clock::now();
}

void Clock::stop()
{
    tpStop_m = std::chrono::steady_clock::now();
}

void Clock::print(const char *title)
{
    auto dotaTime = tpStop_m - tpStart_m;
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(dotaTime);
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(dotaTime);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(dotaTime);
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(dotaTime);

    printf("%s . Time Elapse(m:s:ms:us): %lu:%lu:%lu:%lu\r\n", title,
           minutes.count()%60, seconds.count()%60, milliseconds.count()%1000, microseconds.count()%1000);
}

