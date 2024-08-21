/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#include <QtGlobal>
#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include <QResizeEvent>

#include <Aspect_NeutralWindow.hxx>

#include "occ_handle.h"
#include "occt_window.h"
#include "widget_occ_view.h"
#include "widget_occ_view_impl.h"

namespace Mayo
{

static IWidgetOccView::Creator &getWidgetOccViewCreator()
{
    static IWidgetOccView::Creator fn = [](const OccHandle<V3d_View> &view, QWidget *parent)
    { return new QWidgetOccView(view, parent); };
    return fn;
}

void IWidgetOccView::setCreator(IWidgetOccView::Creator fn)
{
    getWidgetOccViewCreator() = std::move(fn);
}

IWidgetOccView *IWidgetOccView::create(const OccHandle<V3d_View> &view, QWidget *parent)
{
    const auto &fn = getWidgetOccViewCreator();
    return fn(view, parent);
}

static OccHandle<Aspect_NeutralWindow> createNativeWindow([[maybe_unused]] QWidget *widget)
{
    auto window = new Aspect_NeutralWindow;
    // On non-Windows systems Aspect_Drawable is aliased to 'unsigned long' so can't init with
    // nullptr
    Aspect_Drawable nativeWin = 0;
#ifdef Q_OS_WIN
    HDC wglDevCtx = wglGetCurrentDC();
    HWND wglWin = WindowFromDC(wglDevCtx);
    nativeWin = (Aspect_Drawable)wglWin;
#else
    nativeWin = (Aspect_Drawable)widget->winId();
#endif
    window->SetNativeHandle(nativeWin);
    return window;
}

QOpenGLWidgetOccView::QOpenGLWidgetOccView(const OccHandle<V3d_View> &view, QWidget *parent)
    : QOpenGLWidget(parent)
    , IWidgetOccView(view)
{
    this->setMouseTracking(true);
    this->setBackgroundRole(QPalette::NoRole);

    this->setUpdatesEnabled(true);
    this->setUpdateBehavior(QOpenGLWidget::NoPartialUpdate);

    QSurfaceFormat glFormat;
    glFormat.setDepthBufferSize(24);
    glFormat.setStencilBufferSize(8);
    if (QOpenGLWidgetOccView_isCoreProfile())
        glFormat.setVersion(4, 5);

    glFormat.setProfile(QOpenGLWidgetOccView_isCoreProfile() ?
                            QSurfaceFormat::CoreProfile :
                            QSurfaceFormat::CompatibilityProfile);

    this->setFormat(glFormat);
}

void QOpenGLWidgetOccView::redraw()
{
    this->update();
}

QOpenGLWidgetOccView *QOpenGLWidgetOccView::create(const OccHandle<V3d_View> &view, QWidget *parent)
{
    return new QOpenGLWidgetOccView(view, parent);
}

OccHandle<Graphic3d_GraphicDriver> QOpenGLWidgetOccView::createCompatibleGraphicsDriver()
{
    return QOpenGLWidgetOccView_createCompatibleGraphicsDriver();
}

void QOpenGLWidgetOccView::initializeGL()
{
    const QRect wrect = this->rect();
    const Graphic3d_Vec2i viewSize(wrect.right() - wrect.left(), wrect.bottom() - wrect.top());
    QOpenGLWidgetOccView_createOpenGlContext(
        [=, this](Aspect_RenderingContext context)
        {
            auto window = OccHandle<Aspect_NeutralWindow>::DownCast(this->v3dView()->Window());
            if (!window)
                window = createNativeWindow(this);

            window->SetSize(viewSize.x(), viewSize.y());
            this->v3dView()->SetWindow(window, context);
        });
}

void QOpenGLWidgetOccView::paintGL()
{
    if (!this->v3dView()->Window())
        return;

    const OccHandle<Graphic3d_GraphicDriver> &driver = this->v3dView()->Viewer()->Driver();
    if (!QOpenGLWidgetOccView_wrapFrameBuffer(driver))
        return;

    Graphic3d_Vec2i viewSizeOld;
    const Graphic3d_Vec2i viewSizeNew =
        QOpenGLWidgetOccView_getDefaultframeBufferViewportSize(driver);
    auto window = OccHandle<Aspect_NeutralWindow>::DownCast(this->v3dView()->Window());
    window->Size(viewSizeOld.x(), viewSizeOld.y());
    if (viewSizeNew != viewSizeOld)
    {
        window->SetSize(viewSizeNew.x(), viewSizeNew.y());
        this->v3dView()->MustBeResized();
        this->v3dView()->Invalidate();
    }

    // Redraw the viewer
    // this->v3dView()->InvalidateImmediate();
    this->v3dView()->Redraw();
}

QWidgetOccView::QWidgetOccView(const OccHandle<V3d_View> &view, QWidget *parent)
    : QWidget(parent)
    , IWidgetOccView(view)
{
    this->setMouseTracking(true);
    this->setBackgroundRole(QPalette::NoRole);

    // Avoid Qt background clears to improve resizing speed
    this->setAutoFillBackground(false);
    this->setAttribute(Qt::WA_NoSystemBackground);
    this->setAttribute(Qt::WA_PaintOnScreen);
}

// Defined in widget_occ_view.cpp
OccHandle<Graphic3d_GraphicDriver> QWidgetOccView::createCompatibleGraphicsDriver()
{
    return QWidgetOccView_createCompatibleGraphicsDriver();
}

void QWidgetOccView::redraw()
{
    std::cout << "QWidgetOccView::redraw()" << std::endl;
    this->v3dView()->Redraw();
}

QWidgetOccView *QWidgetOccView::create(const OccHandle<V3d_View> &view, QWidget *parent)
{
    return new QWidgetOccView(view, parent);
}

void QWidgetOccView::showEvent(QShowEvent *)
{
    if (this->v3dView()->Window().IsNull())
    {
        auto hWnd = makeOccHandle<OcctWindow>(this);
        this->v3dView()->SetWindow(hWnd);
        if (!hWnd->IsMapped())
            hWnd->Map();

        this->v3dView()->MustBeResized();
    }
}

void QWidgetOccView::paintEvent(QPaintEvent *)
{
    this->v3dView()->Redraw();
}

void QWidgetOccView::resizeEvent(QResizeEvent *event)
{
    if (!event->spontaneous()) // Workaround for infinite window shrink on Ubuntu
    {
        this->v3dView()->MustBeResized();
    }
}

} // namespace Mayo
