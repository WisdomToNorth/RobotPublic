
#pragma once
#ifndef FRAME_CONTROLER_H
#define FRAME_CONTROLER_H

#include "rfTimer.h"

namespace rfbase
{
class FrameControler
{
public:
    FrameControler(int frame_speed);
    ~FrameControler();

    FrameControler(const FrameControler &) = delete;

private:
    const int frame_speed_;
    rfbase::FrTimer timer;
};
}; // namespace rfbase
#endif
