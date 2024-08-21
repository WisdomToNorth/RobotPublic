#include "qtutils.h"

#include <QFile>
#include <QFileInfo>
#include <QGuiApplication>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QPointF>
#include <QScreen>
#include <QString>

namespace rfgui
{
QPoint QtUtils::getMouseEventGlobalPoint(QMouseEvent *event)
{
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    QPointF startPos = event->globalPosition();
#else
    QPointF startPos = event->globalPos();
#endif
    return QPoint(int(startPos.x()), int(startPos.y()));
}

unsigned int QtUtils::getScreenWidth()
{
    if (QGuiApplication::primaryScreen())
    {
        return static_cast<unsigned int>(QGuiApplication::primaryScreen()->geometry().width());
    }
    else
    {
        return 0;
    }
}

unsigned int QtUtils::getScreenHeight()
{
    if (QGuiApplication::primaryScreen())
    {
        return static_cast<unsigned int>(QGuiApplication::primaryScreen()->geometry().height());
    }
    else
    {
        return 0;
    }
}

std::string QtUtils::queryGlVersionString()
{
    QOpenGLContext glContext;
    if (!glContext.create())
        return {};

    QOffscreenSurface surface;
    surface.create();
    if (!glContext.makeCurrent(&surface))
        return {};

    auto glVersion = glContext.functions()->glGetString(GL_VERSION);
    if (!glVersion)
        return {};

    return reinterpret_cast<const char *>(glVersion);
}
} // namespace rfgui
