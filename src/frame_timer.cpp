#include "frame_timer.h"

#include <thread>

namespace chrono = std::chrono;
using namespace std::chrono_literals;

FrameTimer::FrameTimer(std::size_t refresh_period) :
    refresh_period_{refresh_period},
    start_time_{clock::now()}
{
}

FrameTimer::~FrameTimer()
{
}

void FrameTimer::start()
{
    start_time_ = clock::now();
}

void FrameTimer::end()
{
    auto end_time = clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(end_time - start_time_);

    auto sleep_period = refresh_period_ - elapsed;

    if (sleep_period > 0ms)
    {
        std::this_thread::sleep_for(sleep_period);
    }
}
