#include "rfviewernew.h"

#include <QAction>
#include <QApplication>
#include <QCursor>
#include <QGuiApplication>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QVBoxLayout>
#include <QVersionNumber>

#include <AIS_ViewCube.hxx>
#include <Aspect_Handle.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <Prs3d_DatumAspect.hxx>
#include <Prs3d_PlaneAspect.hxx>

#include "graphics_create_driver.h"
#include "graphics_utils.h"

namespace Mayo
{
namespace internal
{

QCursor getCursor(RfViewerNew::CursorType type)
{
    switch (type)
    {
    case RfViewerNew::CursorType::def:
    {
        int cursor_size = 12; // adjust color as needed
        QPixmap pixmap(cursor_size * 2, cursor_size * 2);
        pixmap.fill(Qt::transparent);

        QPainter painter(&pixmap);
        QPen pen(Qt::black); // adjust color as needed
        pen.setWidth(2);     // adjust width as needed
        painter.setPen(pen);
        int center = pixmap.width() * 0.5; // assuming the pixmap is square
        painter.drawLine(center - cursor_size, center, center + cursor_size, center);
        painter.drawLine(center, center - cursor_size, center, center + cursor_size);

        return QCursor(pixmap);
    }
    case RfViewerNew::CursorType::hand: return QCursor(Qt::PointingHandCursor);
    case RfViewerNew::CursorType::pan: return QCursor(Qt::SizeAllCursor);
    case RfViewerNew::CursorType::glob: return QCursor(Qt::CrossCursor);
    case RfViewerNew::CursorType::zoom:
    {
        return QCursor(Qt::SizeHorCursor);
    }
    case RfViewerNew::CursorType::rot: return QCursor(Qt::SizeFDiagCursor);
    case RfViewerNew::CursorType::wait: return QCursor(Qt::CursorShape::ForbiddenCursor);
    default: return QCursor(Qt::ArrowCursor);
    }
}
}; // namespace internal

void RfViewerNew::setUserCursor(CursorType type)
{
    auto cursor = internal::getCursor(type);
    switch (type)
    {
    case CursorType::def: this->setCursor(cursor); break;
    case CursorType::hand: this->setCursor(cursor); break;
    case CursorType::pan: this->setCursor(cursor); break;
    case CursorType::glob: this->setCursor(cursor); break;
    case CursorType::zoom: this->setCursor(cursor); break;
    case CursorType::rot: this->setCursor(cursor); break;
    case CursorType::wait: this->setCursor(cursor); break;
    default: break;
    }
}

[[maybe_unused]] static std::string queryGlVersionString()
{
    QOpenGLContext glContext;
    if (!glContext.create())
        return {};

    QOffscreenSurface surface;
    surface.create();
    if (!glContext.makeCurrent(&surface))
        return {};

    const GLubyte *glVersion = glContext.functions()->glGetString(GL_VERSION);
    if (!glVersion)
        return {};

    return reinterpret_cast<const char *>(glVersion);
}

// Helper to parse a string containing a semantic version eg "4.6.5 CodeNamed"
// Note: only major and minor versions are detected
[[maybe_unused]] static QVersionNumber parseSemanticVersionString(std::string_view strVersion)
{
    if (strVersion.empty())
        return {};

    const char *ptrVersionStart = strVersion.data();
    const char *ptrVersionEnd = ptrVersionStart + strVersion.size();
    const int versionMajor = std::atoi(ptrVersionStart);
    int versionMinor = 0;
    auto ptrDot = std::find(ptrVersionStart, ptrVersionEnd, '.');
    if (ptrDot != ptrVersionEnd)
        versionMinor = std::atoi(ptrDot + 1);

    return QVersionNumber(versionMajor, versionMinor);
}

RfViewerNew::RfViewerNew(QWidget *parent)
    : QWidget(parent)
    , v3dView_(gfxScene_.createV3dView())
    , camera_animation_(new V3dViewCameraAnimation())

{
    if (!qobject_cast<QGuiApplication *>(QCoreApplication::instance()))
    {
        // QOpenGL requires QGuiApplication
        return;
    }

    {
        // m_v3dView->SetShadingModel(Graphic3d_TypeOfShadingModel_Pbr);
        // 3D view - Enable anti-aliasing with MSAA
        v3dView_->ChangeRenderingParams().IsAntialiasingEnabled = true;
        v3dView_->ChangeRenderingParams().NbMsaaSamples = 4;
        v3dView_->ChangeRenderingParams().CollectedStats =
            Graphic3d_RenderingParams::PerfCounters_Extended;
        v3dView_->ChangeRenderingParams().StatsPosition = new Graphic3d_TransformPers(
            Graphic3d_TMF_2d, Aspect_TOTP_RIGHT_UPPER, Graphic3d_Vec2i(20, 20));
        Quantity_Color top_color =
            Quantity_Color(101 / 255.0, 101 / 255.0, 122 / 255.0, Quantity_TOC_RGB);
        Quantity_Color bottom_color =
            Quantity_Color(191 / 255.0, 193 / 255.0, 204 / 255.0, Quantity_TOC_RGB);
        const Aspect_GradientFillMethod fillstype = Aspect_GFM_VER;
        // 3D view - Set gradient background
        v3dView_->SetBgGradientColors(top_color, bottom_color, fillstype);
        camera_animation_->setView(v3dView_);
    }

    {
        const std::string strGlVersion = queryGlVersionString();
        const QVersionNumber glVersion = parseSemanticVersionString(strGlVersion);
        if (!glVersion.isNull() && glVersion.majorVersion() >= 2)
        {
            setFunctionCreateGraphicsDriver(&QOpenGLWidgetOccView::createCompatibleGraphicsDriver);
            IWidgetOccView::setCreator(&QOpenGLWidgetOccView::create);
        }
        else
        {
            setFunctionCreateGraphicsDriver(&QWidgetOccView::createCompatibleGraphicsDriver);
            IWidgetOccView::setCreator(&QWidgetOccView::create);
        }
        occ_view_ = IWidgetOccView::create(v3dView_, this);
        auto layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(occ_view_->widget());
    }

    {
    }

    setBackgroundRole(QPalette::NoRole);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_PaintOnScreen);
    setMouseTracking(true);

    setViewTrihedronMode(ViewTrihedronMode::AisViewCube);
    setViewTrihedronCorner(Aspect_TOTP_LEFT_UPPER);
    setUserCursor(CursorType::def);
}

RfViewerNew::~RfViewerNew()
{
    delete camera_animation_;
}

bool RfViewerNew::processAction(const GraphicsOwnerPtr &gfxOwner)
{
    if (!gfxOwner)
        return false;

    auto viewCubeOwner = OccHandle<AIS_ViewCubeOwner>::DownCast(gfxOwner);
    if (viewCubeOwner)
    {
        v3dView_->SetProj(viewCubeOwner->MainOrientation());
        // gfxScene_.select();
        GraphicsUtils::V3dView_fitAll(v3dView_);
        return true;
    }

    return false;
}

RfViewerNew::ViewTrihedronMode RfViewerNew::viewTrihedronMode() const
{
    return view_trihedron_mode_;
}

Aspect_TypeOfTriedronPosition RfViewerNew::viewTrihedronCorner() const
{
    return view_trihedron_corner_;
}

void RfViewerNew::v3dViewTrihedronDisplay(Aspect_TypeOfTriedronPosition corner)
{
    const double scale = 0.075 * 2; // TODO: 2: m_devicePixelRatio;
    v3dView_->TriedronDisplay(corner, Quantity_NOC_GRAY50, scale, V3d_ZBUFFER);
}

void RfViewerNew::setViewTrihedronMode(ViewTrihedronMode mode)
{
    if (mode == view_trihedron_mode_)
        return;

    switch (mode)
    {
    case ViewTrihedronMode::None:
    {
        v3dView_->TriedronErase();
        gfxScene_.setObjectVisible(viewcube_, false);
        break;
    }
    case ViewTrihedronMode::V3dViewZBuffer:
    {
        this->v3dViewTrihedronDisplay(view_trihedron_corner_);
        gfxScene_.setObjectVisible(viewcube_, false);
        break;
    }
    case ViewTrihedronMode::AisViewCube:
    {
        if (viewcube_.IsNull())
        {
            auto ais_viewcube = new AIS_ViewCube;
            ais_viewcube->SetBoxColor(Quantity_NOC_GRAY2);
            ais_viewcube->SetSize(55);
            ais_viewcube->SetFontHeight(12);
            ais_viewcube->SetAxesLabels("X", "Y", "Z");
            ais_viewcube->SetBoxTransparency(0.75);
            ais_viewcube->SetTransformPersistence(new Graphic3d_TransformPers(
                Graphic3d_TMF_TriedronPers, view_trihedron_corner_, Graphic3d_Vec2i(85, 85)));

            gfxScene_.addObject(ais_viewcube);
            const auto &datumAspect = ais_viewcube->Attributes()->DatumAspect();
            datumAspect->ShadingAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_RED2);
            datumAspect->ShadingAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_GREEN2);
            datumAspect->ShadingAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_BLUE2);

            viewcube_ = ais_viewcube;
        }

        v3dView_->TriedronErase();
        gfxScene_.setObjectVisible(viewcube_, true);
        break;
    }
    }

    view_trihedron_mode_ = mode;
}

void RfViewerNew::setViewTrihedronCorner(Aspect_TypeOfTriedronPosition corner)
{
    if (corner == view_trihedron_corner_)
    {
        return;
    }

    switch (view_trihedron_mode_)
    {
    case ViewTrihedronMode::None:
    {
        break;
    }
    case ViewTrihedronMode::V3dViewZBuffer:
    {
        this->v3dViewTrihedronDisplay(corner);
        break;
    }
    case ViewTrihedronMode::AisViewCube:
    {
        if (viewcube_)
            viewcube_->TransformPersistence()->SetCorner2d(corner);

        break;
    }
    } // endswitch

    view_trihedron_corner_ = corner;
}

QPaintEngine *RfViewerNew::paintEngine() const
{
    return 0;
}

void RfViewerNew::paintEvent(QPaintEvent *){};

void RfViewerNew::setName(const std::string &name)
{
    name_ = name;
}

std::string RfViewerNew::getName() const
{
    return name_;
}

bool RfViewerNew::hoveringViewCube()
{
    auto obj = gfxScene_.currentHighlightedOwner();
    if (!obj.IsNull())
    {
        return !obj->IsKind(STANDARD_TYPE(AIS_ViewCube));
    }
    return false;
}

ViewerEvent RfViewerNew::mouseEventToViewInfo(QMouseEvent *event)
{
    auto eventToVTypes = [](QEvent *event)
    {
        VTypes result = 0;
        if (event->type() == QEvent::MouseMove)
        {
            result |= VType::MouseMove;
        }
        else if (event->type() == QEvent::Wheel)
        {
            result |= VType::MouseWheel;
        }
        else if (event->type() == QEvent::MouseButtonPress ||
                 event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (event->type() == QEvent::MouseButtonPress)
            {
                const Qt::MouseButtons &qbuttons = mouseEvent->buttons();
                if (qbuttons == Qt::MouseButton::NoButton)
                {
                    result |= VType::None;
                    return result;
                }
                if (qbuttons & Qt::MouseButton::LeftButton)
                {
                    result |= VType::LButtonDown;
                }
                if (qbuttons & Qt::MouseButton::RightButton)
                {
                    result |= VType::RButtonDown;
                }
                if (qbuttons & Qt::MouseButton::MiddleButton)
                {
                    result |= VType::MButtonDown;
                }
            }
            else if (event->type() == QEvent::MouseButtonRelease)
            {
                const Qt::MouseButton &qbutton = mouseEvent->button();
                if (qbutton == Qt::MouseButton::NoButton)
                {
                    result |= VType::None;
                    return result;
                }
                if (qbutton == Qt::MouseButton::LeftButton)
                {
                    result |= VType::LButtonUp;
                }
                if (qbutton == Qt::MouseButton::RightButton)
                {
                    result |= VType::RButtonUp;
                }
                if (qbutton == Qt::MouseButton::MiddleButton)
                {
                    result |= VType::MButtonUp;
                }
            }
        }
        return result;
    };

    double x, y, z;

    v3dView_->Convert(event->pos().x(), event->pos().y(), x, y, z);

    return ViewerEvent{.type_flags = eventToVTypes(event),
                       .view_pos_x = x,
                       .view_pos_y = y,
                       .view_pos_z = z,
                       .mouse_pos_x = event->pos().x(),
                       .mouse_pos_y = event->pos().y(),
                       .picked_entity{gfxScene_.currentHighlightedOwner()}};
}

void RfViewerNew::mousePressEvent(QMouseEvent *event)
{
    auto info = mouseEventToViewInfo(event);
    if (event->buttons() == Qt::MouseButton::LeftButton)
    {
        gfxScene_.highlightAt(info.mouse_pos_x, info.mouse_pos_y, v3dView_);
        gfxScene_.setSelectionMode(GraphicsScene::SelectionMode::Single); // TODO:
        if (!processAction(gfxScene_.currentHighlightedOwner()) && viewerEventCb)
        {
            gfxScene_.select();
            viewerEventCb(info);
        }
        occ_view_->redraw();
    }
    else if (event->buttons() == Qt::RightButton)
    {
        gfxScene_.highlightAt(info.mouse_pos_x, info.mouse_pos_y, v3dView_);
        gfxScene_.redraw();
        if (hoveringViewCube() && viewerEventCb)
        {
            viewerEventCb(info);
        }
    }
    else
    {
        if (viewerEventCb)
        {
            viewerEventCb(info);
        }
        v3dView_->Update();
    }
}

void RfViewerNew::mouseReleaseEvent(QMouseEvent *event)
{
    if (viewerEventCb)
    {
        const auto &info = mouseEventToViewInfo(event);
        viewerEventCb(info);
    }
};

void RfViewerNew::mouseMoveEvent(QMouseEvent *event)
{
    const auto &info = mouseEventToViewInfo(event);
    if (event->buttons() == Qt::MouseButton::NoButton)
    {
        gfxScene_.highlightAt(info.mouse_pos_x, info.mouse_pos_y, v3dView_);
        occ_view_->redraw();
    }

    if (viewerEventCb)
    {
        viewerEventCb(info);
    }
}

void RfViewerNew::wheelEvent(QWheelEvent *event)
{
    ViewerEvent info{.type_flags = VType::MouseWheel,
                     .mouse_pos_x = static_cast<int>(event->position().x()),
                     .mouse_pos_y = static_cast<int>(event->position().y()),
                     .wheel_delta_x = event->angleDelta().x(),
                     .wheel_delta_y = event->angleDelta().y()};
    if (viewerEventCb)
    {
        viewerEventCb(info);
    }
}

void RfViewerNew::zoomAtPoint(double x, double y, double zoom_factor_x, double zoome_factory_y)
{
    v3dView_->StartZoomAtPoint(x, y);
    v3dView_->ZoomAtPoint(0, 0, zoom_factor_x * zoom_scale_control_,
                          zoome_factory_y * zoom_scale_control_);
}

void RfViewerNew::startRotation(double ori_x, double ori_y)
{
    v3dView_->StartRotation(ori_x, ori_y);
}

void RfViewerNew::rotAtPoint(double x, double y)
{
    v3dView_->Rotation(x, y);
}

void RfViewerNew::pan(double dx, double dy)
{
    v3dView_->Pan(dx, dy, 1.0, true);
}

void RfViewerNew::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::ControlModifier)
    {
        selection_scheme_ = AIS_SelectionScheme::AIS_SelectionScheme_Add;
    }
    else
    {
        selection_scheme_ = AIS_SelectionScheme::AIS_SelectionScheme_Replace;
    }

    return QWidget::keyPressEvent(event);
}

void RfViewerNew::setDisplayMode(const Handle(AIS_InteractiveObject) obj, int display_mode)
{
    gfxScene_.setObjectDisplayMode(obj, display_mode);
}

void RfViewerNew::setSelectFlag(int select_flag)
{
    select_flag_ = select_flag;
    gfxScene_.foreachDisplayedObject([this](const Handle(AIS_InteractiveObject) & obj)
                                     { gfxScene_.setObjSelectionMode(obj, select_flag_); });
}

void RfViewerNew::updateSelectMode(const Handle(AIS_InteractiveObject) obj)
{
    gfxScene_.setObjSelectionMode(obj, select_flag_);
}

int RfViewerNew::getDynamicFlag() const
{
    return select_flag_;
}

void RfViewerNew::addModel(Handle(AIS_InteractiveObject) _model)
{
    gfxScene_.setObjectVisible(_model, true);
}

void RfViewerNew::hideModel(Handle(AIS_InteractiveObject) _model)
{
    gfxScene_.setObjectVisible(_model, false);
}

void RfViewerNew::clearSelected()
{
    gfxScene_.clearSelection();
}

void RfViewerNew::reCalcObjSelection(const Handle(AIS_InteractiveObject) obj)
{
    gfxScene_.recomputeObjectPresentation(obj);
}

bool RfViewerNew::isDisplayed(const Handle(AIS_InteractiveObject) obj)
{
    return gfxScene_.isObjectVisible(obj);
}

void RfViewerNew::setObjHighlight(const Handle(AIS_InteractiveObject) obj, bool selected)
{
    gfxScene_.highlightObject(obj, selected);
}

void RfViewerNew::updateViewer(KUpdate cmd)
{
    switch (cmd)
    {
    case RfViewerNew::KUpdate::Redraw:
    {
        occ_view_->redraw();
        break;
    }
    case RfViewerNew::KUpdate::Update:
    {
        occ_view_->redraw();
        this->update();
        break;
    }
    case RfViewerNew::KUpdate::Refresh:
    {
        v3dView_->MustBeResized();
        occ_view_->redraw();
        this->update();

        QResizeEvent event(this->size(), this->size());
        QApplication::sendEvent(this, &event);
        break;
    }
    default: break;
    }
}
} // namespace Mayo
