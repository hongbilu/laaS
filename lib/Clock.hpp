#pragma once
#include <chrono>
#include "basic_types.hpp"
#include "raii.hpp"

class Clock
{
public:
    Clock(){}
    Clock(const Clock&) = delete;
    Clock& operator=(const Clock&) = delete;
    void start();
    void stop();
    void print(const char *title);
    u_int32 minute(){return (std::chrono::duration_cast<std::chrono::minutes>(tpStop_m - tpStart_m)).count()%60;}
    u_int32 second(){return (std::chrono::duration_cast<std::chrono::seconds>(tpStop_m - tpStart_m)).count()%60;}
    u_int32 millisecond(){return (std::chrono::duration_cast<std::chrono::milliseconds>(tpStop_m - tpStart_m)).count()%1000;}
    u_int32 usecond(){return (std::chrono::duration_cast<std::chrono::microseconds>(tpStop_m - tpStart_m)).count()%1000;}
    raii make()const noexcept
    {
        return make_raii(*this, &Clock::stop, &Clock::start);
    }

private:
    std::chrono::steady_clock::time_point tpStart_m;
    std::chrono::steady_clock::time_point tpStop_m;
};

