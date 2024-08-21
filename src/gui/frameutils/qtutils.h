#pragma once

#ifndef QT_UTILS_H
#define QT_UTILS_H

#include <QMouseEvent>
#include <QPoint>

namespace rfgui
{
class QtUtils
{
public:
    static QPoint getMouseEventGlobalPoint(QMouseEvent *event);
    static unsigned int getScreenWidth();
    static unsigned int getScreenHeight();
    static std::string queryGlVersionString();
};
} // namespace rfgui
#endif
