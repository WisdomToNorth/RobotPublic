#include "rfviewer.h"

#include <QAction>
#include <QApplication>
#include <QCursor>
#include <QGuiApplication>
#include <QPainter>

#include <AIS_ViewCube.hxx>
#include <Aspect_Handle.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <Prs3d_DatumAspect.hxx>
#include <Prs3d_PlaneAspect.hxx>

namespace rfapp
{
namespace internal
{

QCursor getCursor(RfViewer::CursorType type)
{
    switch (type)
    {
    case RfViewer::CursorType::def:
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
    case RfViewer::CursorType::hand: return QCursor(Qt::PointingHandCursor);
    case RfViewer::CursorType::pan: return QCursor(Qt::SizeAllCursor);
    case RfViewer::CursorType::glob: return QCursor(Qt::CrossCursor);
    case RfViewer::CursorType::zoom:
    {
        return QCursor(Qt::SizeHorCursor);
    }
    case RfViewer::CursorType::rot: return QCursor(Qt::SizeFDiagCursor);
    case RfViewer::CursorType::wait: return QCursor(Qt::CursorShape::ForbiddenCursor);
    default: return QCursor(Qt::ArrowCursor);
    }
}
}; // namespace internal

void RfViewer::setUserCursor(CursorType type)
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

RfViewer::RfViewer(QWidget *parent)
    : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus); // 窗口将通过鼠标点击、按tab键等方式获取焦点
    setBackgroundRole(QPalette::NoRole);
    setMouseTracking(true);
    // set focus policy to threat QContextMenuEvent from keyboard

    initContext();
    setUserCursor(CursorType::def);
}

RfViewer::~RfViewer()
{
}

void RfViewer::initContext()
{
    if (context_.IsNull())
    {
        Handle(Aspect_DisplayConnection) m_display_donnection = new Aspect_DisplayConnection();
        if (graphic_driver_.IsNull())
        {
            graphic_driver_ = new OpenGl_GraphicDriver(m_display_donnection);
        }
        WId window_handle = (WId)winId();

#ifdef _WIN32
        Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle)window_handle);
#elif defined(__APPLE__) && !defined(MACOSX_USE_GLX)
        Handle(Cocoa_Window) wind = new Cocoa_Window((NSView *)window_handle);
#else
        Handle(Xw_Window) wind = new Xw_Window(m_display_donnection, (Aspect_Handle)window_handle);
#endif
        // 创建3D查看器
        v_viewer_ = new V3d_Viewer(graphic_driver_);
        v_viewer_->SetDefaultLights();
        v_viewer_->SetLightOn();
        v_viewer_->ActivateGrid(Aspect_GT_Rectangular, Aspect_GDM_Points);
        v_viewer_->SetGridEcho(false);

        view_ = v_viewer_->CreateView();
        view_->SetWindow(wind);
        if (!wind->IsMapped())
        {
            wind->Map();
        }

        view_->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);

        Quantity_Color top_color =
            Quantity_Color(101 / 255.0, 101 / 255.0, 122 / 255.0, Quantity_TOC_RGB);
        Quantity_Color bottom_color =
            Quantity_Color(191 / 255.0, 193 / 255.0, 204 / 255.0, Quantity_TOC_RGB);
        view_->SetBgGradientColors(top_color, bottom_color, Aspect_GFM_VER);

        context_ = new AIS_InteractiveContext(v_viewer_);
        initHilightAttributes();
        initContextAttributes();

        // Set Qt attribute
        setBackgroundRole(QPalette::NoRole);
        setFocusPolicy(Qt::StrongFocus);
        setAttribute(Qt::WA_NoSystemBackground);
        setAttribute(Qt::WA_PaintOnScreen);
        setMouseTracking(true);
    }
    setViewCube();

    view_->Redraw();
    view_->MustBeResized();
};

void RfViewer::setViewCube()
{
    auto ais_viewcube = new AIS_ViewCube();
    double transp_num = 0.55;
    ais_viewcube->SetBoxColor(Quantity_NOC_GRAY2);
    ais_viewcube->SetSize(55);
    ais_viewcube->SetFontHeight(12);
    ais_viewcube->SetBoxTransparency(transp_num);
    ais_viewcube->SetTransformPersistence(new Graphic3d_TransformPers(
        Graphic3d_TMF_TriedronPers, Aspect_TypeOfTriedronPosition::Aspect_TOTP_RIGHT_LOWER,
        Graphic3d_Vec2i(85, 85)));

    const Handle_Prs3d_DatumAspect datum_color = new Prs3d_DatumAspect();
    datum_color->ShadingAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_GREEN2);
    datum_color->ShadingAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_RED2);
    datum_color->ShadingAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_BLUE2);
    datum_color->ShadingAspect(Prs3d_DP_XAxis)->SetTransparency(transp_num);
    datum_color->ShadingAspect(Prs3d_DP_YAxis)->SetTransparency(transp_num);
    datum_color->ShadingAspect(Prs3d_DP_ZAxis)->SetTransparency(transp_num);
    ais_viewcube->Attributes()->SetDatumAspect(datum_color);

    viewcube_ = ais_viewcube;

    context_->Display(viewcube_, true);
}

QPaintEngine *RfViewer::paintEngine() const
{
    return 0;
}

void RfViewer::initContextAttributes()
{
    context_->SetDisplayMode(AIS_Shaded, Standard_True);
    context_->HighlightStyle(Prs3d_TypeOfHighlight_LocalDynamic)
        ->SetColor(Quantity_NameOfColor::Quantity_NOC_RED);
    context_->HighlightStyle(Prs3d_TypeOfHighlight_LocalDynamic)->SetMethod(Aspect_TOHM_COLOR);
    context_->HighlightStyle(Prs3d_TypeOfHighlight_LocalDynamic)->SetDisplayMode(1);
    context_->HighlightStyle(Prs3d_TypeOfHighlight_LocalDynamic)->SetTransparency(0.5f);
    context_->UpdateCurrentViewer();
}

void RfViewer::initHilightAttributes()
{
    Quantity_Color L_YELLOW =
        Quantity_Color(245 / 255.0, 180 / 255.0, 77 / 255.0, Quantity_TOC_RGB);
    Quantity_Color L_BLUE = Quantity_Color(29 / 255.0, 166 / 255.0, 251 / 255.0, Quantity_TOC_RGB);
    setHilightAttribute(Prs3d_TypeOfHighlight::Prs3d_TypeOfHighlight_Dynamic, 2, L_BLUE);
    setHilightAttribute(Prs3d_TypeOfHighlight::Prs3d_TypeOfHighlight_LocalDynamic, 2, L_YELLOW);
    setHilightAttribute(Prs3d_TypeOfHighlight::Prs3d_TypeOfHighlight_Selected, 2, Quantity_NOC_RED);
    setHilightAttribute(Prs3d_TypeOfHighlight::Prs3d_TypeOfHighlight_LocalSelected, 2,
                        Quantity_NOC_RED);
}

void RfViewer::setHilightAttribute(Prs3d_TypeOfHighlight idx, double lineWidth_aspect,
                                   Quantity_Color theColor)
{
    //        Prs3d_TypeOfHighlight_Selected,       //!< entire object is selected
    //        Prs3d_TypeOfHighlight_Dynamic,        //!< entire object is dynamically highlighted
    //        Prs3d_TypeOfHighlight_LocalSelected,  //!< part of the object is selected
    //        Prs3d_TypeOfHighlight_LocalDynamic,   //!< part of the object is dynamically
    //        highlighted
    const Handle(Prs3d_Drawer) &drawer = context_->HighlightStyle(idx);
    double lineWidth = 5.0;

    drawer->SetMethod(Aspect_TOHM_COLOR);
    drawer->SetDisplayMode(0);
    drawer->SetColor(theColor);

    drawer->SetupOwnShadingAspect();
    drawer->SetupOwnPointAspect();

    drawer->SetLineAspect(new Prs3d_LineAspect(Quantity_NOC_BLACK, Aspect_TOL_SOLID, lineWidth));
    *drawer->LineAspect()->Aspect() = *drawer->Link()->LineAspect()->Aspect();

    drawer->SetWireAspect(new Prs3d_LineAspect(Quantity_NOC_BLACK, Aspect_TOL_SOLID, lineWidth));
    *drawer->WireAspect()->Aspect() = *drawer->Link()->WireAspect()->Aspect();

    drawer->SetPlaneAspect(new Prs3d_PlaneAspect());
    *drawer->PlaneAspect()->EdgesAspect() = *drawer->Link()->PlaneAspect()->EdgesAspect();

    drawer->SetFreeBoundaryAspect(
        new Prs3d_LineAspect(Quantity_NOC_BLACK, Aspect_TOL_SOLID, lineWidth));
    *drawer->FreeBoundaryAspect()->Aspect() = *drawer->Link()->FreeBoundaryAspect()->Aspect();

    drawer->SetUnFreeBoundaryAspect(
        new Prs3d_LineAspect(Quantity_NOC_BLACK, Aspect_TOL_SOLID, lineWidth));
    *drawer->UnFreeBoundaryAspect()->Aspect() = *drawer->Link()->UnFreeBoundaryAspect()->Aspect();
    drawer->SetDatumAspect(new Prs3d_DatumAspect());

    drawer->ShadingAspect()->SetColor(theColor);
    drawer->WireAspect()->SetColor(theColor);
    drawer->LineAspect()->SetColor(theColor);
    drawer->PlaneAspect()->ArrowAspect()->SetColor(theColor);
    drawer->PlaneAspect()->IsoAspect()->SetColor(theColor);
    drawer->PlaneAspect()->EdgesAspect()->SetColor(theColor);
    drawer->FreeBoundaryAspect()->SetColor(theColor);
    drawer->UnFreeBoundaryAspect()->SetColor(theColor);
    drawer->PointAspect()->SetColor(theColor);
    for (Standard_Integer it = 0; it < Prs3d_DP_None; ++it)
    {
        if (Handle(Prs3d_LineAspect) aLineAsp =
                drawer->DatumAspect()->LineAspect((Prs3d_DatumParts)it))
        {
            aLineAsp->SetColor(theColor);
        }
    }

    drawer->WireAspect()->SetWidth(lineWidth_aspect);
    drawer->LineAspect()->SetWidth(lineWidth_aspect);
    drawer->PlaneAspect()->EdgesAspect()->SetWidth(lineWidth_aspect);
    drawer->FreeBoundaryAspect()->SetWidth(lineWidth_aspect);
    drawer->UnFreeBoundaryAspect()->SetWidth(lineWidth_aspect); // Auxilliary lines
    drawer->PointAspect()->SetTypeOfMarker(Aspect_TOM_O_POINT);
    drawer->PointAspect()->SetScale(lineWidth_aspect);

    // the triangulation should be computed using main presentation attributes,
    // and should not be overridden by highlighting
    drawer->SetAutoTriangulation(Standard_False);
}

void RfViewer::paintEvent(QPaintEvent *)
{
    view_->InvalidateImmediate();
};

void RfViewer::setName(const std::string &name)
{
    name_ = name;
}

std::string RfViewer::getName() const
{
    return name_;
}

void RfViewer::resizeEvent(QResizeEvent *)
{
    if (!view_.IsNull())
    {
        view_->MustBeResized();
    }
};

bool RfViewer::checkDetectedValid()
{
    if (!context_->HasDetected())
    {
        return true;
    }
    else
    {
        if (context_->DetectedInteractive()->IsKind(STANDARD_TYPE(AIS_ViewCube)))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}

ViewerEvent RfViewer::mouseEventToViewInfo(QMouseEvent *event)
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
    view_->Convert(event->pos().x(), event->pos().y(), x, y, z);

    return ViewerEvent{.type_flags = eventToVTypes(event),
                       .view_pos_x = x,
                       .view_pos_y = y,
                       .view_pos_z = z,
                       .mouse_pos_x = event->pos().x(),
                       .mouse_pos_y = event->pos().y(),
                       .picked_entity{context_->DetectedOwner()}};
}

void RfViewer::mousePressEvent(QMouseEvent *event)
{
    auto info = mouseEventToViewInfo(event);
    if (event->buttons() == Qt::MouseButton::LeftButton)
    {
        context_->MoveTo(info.mouse_pos_x, info.mouse_pos_y, view_, true);
        context_->ClearSelected(true);
        context_->SelectDetected(selection_scheme_);
        if (checkDetectedValid() && viewerEventCb)
        {
            viewerEventCb(info);
        }
        updateViewer(KUpdate::Update);
    }
    else if (event->buttons() == Qt::RightButton)
    {
        context_->MoveTo(info.mouse_pos_x, info.mouse_pos_y, view_, true);
        if (checkDetectedValid() && viewerEventCb)
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
    }

    update();
    view_->Update();
};

void RfViewer::mouseReleaseEvent(QMouseEvent *event)
{
    if (viewerEventCb)
    {
        const auto &info = mouseEventToViewInfo(event);
        viewerEventCb(info);
    }
};

void RfViewer::mouseMoveEvent(QMouseEvent *event)
{
    const auto &info = mouseEventToViewInfo(event);
    if (event->buttons() == Qt::MouseButton::NoButton)
    {
        context_->MoveTo(info.mouse_pos_x, info.mouse_pos_y, view_, true);
    }

    if (viewerEventCb)
    {
        viewerEventCb(info);
    }
};

void RfViewer::wheelEvent(QWheelEvent *event)
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
};

void RfViewer::zoomAtPoint(double x, double y, double zoom_factor_x, double zoome_factory_y)
{
    view_->StartZoomAtPoint(x, y);
    view_->ZoomAtPoint(0, 0, zoom_factor_x * zoom_scale_control_,
                       zoome_factory_y * zoom_scale_control_);
}

void RfViewer::startRotation(double ori_x, double ori_y)
{
    view_->StartRotation(ori_x, ori_y);
}

void RfViewer::rotAtPoint(double x, double y)
{
    view_->Rotation(x, y);
}

void RfViewer::pan(double dx, double dy)
{
    view_->Pan(dx, dy, 1.0, true);
}

void RfViewer::keyPressEvent(QKeyEvent *event)
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
};

void RfViewer::removeAll(bool redraw)
{
    context_->RemoveAll(false);
    context_->Display(viewcube_, false);
    if (redraw)
    {
        view_->Redraw();
    }
}

void RfViewer::setSelectFlag(int select_flag)
{
    select_flag_ = select_flag;
    AIS_ListOfInteractive objs;
    context_->DisplayedObjects(objs);
    for (AIS_ListOfInteractive::Iterator it(objs); it.More(); it.Next())
    {
        Handle(AIS_InteractiveObject) obj = it.Value();
        if (obj->IsKind(STANDARD_TYPE(AIS_ViewCube)))
        {
            continue;
        }
        updateSelectMode(obj);
    }
}

int RfViewer::getDynamicFlag() const
{
    return select_flag_;
}

void RfViewer::updateSelectMode(Handle(AIS_InteractiveObject) _model)
{
    if (!_model)
    {
        return;
    }

    if (_model->IsKind(STANDARD_TYPE(AIS_Shape)))
    {
        Handle(AIS_Shape) ais_shape = Handle(AIS_Shape)::DownCast(_model);
        context_->Deactivate(ais_shape);
        auto select_concurrency =
            AIS_SelectionModesConcurrency::AIS_SelectionModesConcurrency_Multiple;

        int flag = select_flag_;
        int mode = 0;
        // TODO: this will reset all activate mode, we can optimize it if needed
        do
        {
            if (flag & 1)
            {
                context_->SetSelectionModeActive(ais_shape, mode, true, select_concurrency);
            }
            mode++;
        } while (flag >>= 1);
    }
}

void RfViewer::clearSelected()
{
    context_->ClearSelected(false);
}

void RfViewer::addModel(Handle(AIS_InteractiveObject) _model)
{
    if (_model)
    {
        context_->Display(_model, true);
    }
}

void RfViewer::hideModel(Handle(AIS_InteractiveObject) _model)
{
    if (_model)
    {
        context_->Erase(_model, true);
    }
}

void RfViewer::setObjHighlight(Handle(AIS_InteractiveObject) _model, bool on)
{
    if (on)
    {
        context_->HilightWithColor(_model, context_->HighlightStyle(), false);
    }
    else
    {
        context_->Unhilight(_model, false);
    }
}

void RfViewer::setDisplayMode(const Handle(AIS_InteractiveObject) obj, int display_mode)
{
    if (obj)
    {
        context_->SetDisplayMode(obj, display_mode, true);
    }
}

bool RfViewer::isDisplayed(Handle(AIS_InteractiveObject) _model)
{
    if (_model)
    {
        return context_->IsDisplayed(_model);
    }
    return false;
}

void RfViewer::reCalcObjSelection(Handle(AIS_InteractiveObject) _model)
{
    if (_model)
    {
        context_->Redisplay(_model, true);
        // context_->Update(_model, true);
    }
}

void RfViewer::updateViewer(KUpdate cmd)
{
    switch (cmd)
    {
    case RfViewer::KUpdate::Redraw:
    {
        view_->Redraw();
        context_->UpdateCurrentViewer();
        break;
    }
    case RfViewer::KUpdate::Update:
    {
        context_->UpdateCurrentViewer();
        break;
    }
    case RfViewer::KUpdate::Refresh:
    {
        view_->MustBeResized();
        context_->UpdateCurrentViewer();
        view_->Redraw();
        this->update();

        QResizeEvent event(this->size(), this->size());
        QApplication::sendEvent(this, &event);
        break;
    }
    default: break;
    }
}
} // namespace rfapp
