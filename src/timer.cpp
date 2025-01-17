#include "timer.h"
#include <iostream>

namespace timer
{
    Timer::Timer() : running(false) {}

    void Timer::start()
    {
        std::cout << "Timer started." << std::endl;
        startTime = std::chrono::high_resolution_clock::now();
        running = true;
    }

    void Timer::stop()
    {
        std::cout << "Timer stopped." << std::endl;
        if (running)
        {
            endTime = std::chrono::high_resolution_clock::now();
            running = false;
        }
        else
        {
            std::cerr << "Timer is not running. Call start() before stop()." << std::endl;
        }
    }

    double Timer::elapsedMilliseconds() const
    {
        if (running)
        {
            std::cerr << "Timer is still running. Call stop() before measuring elapsed time." << std::endl;
            return 0.0;
        }
        return std::chrono::duration<double, std::milli>(endTime - startTime).count();
    }

    double Timer::elapsedSeconds() const
    {
        if (running)
        {
            std::cerr << "Timer is still running. Call stop() before measuring elapsed time." << std::endl;
            return 0.0;
        }
        return std::chrono::duration<double>(endTime - startTime).count();
    }
}