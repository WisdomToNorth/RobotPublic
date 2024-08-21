
#include "rfanimecontroler.h"

#include <thread>

namespace rfbase
{
FrameControler::FrameControler(int frame_speed)
    : frame_speed_(frame_speed)
    , timer(rfbase::FrTimer())
{
}

FrameControler::~FrameControler()
{
    const int frame_speed = 1000 / frame_speed_;
    const int sleep_time = frame_speed - static_cast<int>(timer.timeFromBegin());
    if (sleep_time > 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
    }
}
} // namespace rfbase
