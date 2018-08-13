#include <chrono>

class FrameTimer
{
    using clock = std::chrono::high_resolution_clock;
    using time_point = std::chrono::high_resolution_clock::time_point;

public:
    FrameTimer(std::size_t refresh_period);
    ~FrameTimer();

    void start();
    void end();

private:
    std::chrono::milliseconds refresh_period_;
    time_point start_time_;
};
