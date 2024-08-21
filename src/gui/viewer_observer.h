#pragma once
#ifndef VIEWER_OBSERVER_H
#define VIEWER_OBSERVER_H

#include "rfapp/observer.h"

#include "viewnode.h"

namespace rfgui
{
class ViewObserver : public rfapp::Observer<ViewNode>
{
public:
    ViewObserver() = default;
    ~ViewObserver() override = default;

    void updateEvent(ViewNode *sub_, rfapp::KMode mode) override;
};
} // namespace rfgui
#endif
