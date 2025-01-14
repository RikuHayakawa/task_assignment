#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <string>

namespace timer
{
    class Timer
    {
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
        bool running;

    public:
        Timer();
        void start();
        void stop();
        double elapsedMilliseconds() const;
        double elapsedSeconds() const;
    };
}

#endif
