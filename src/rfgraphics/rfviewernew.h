#pragma once
#ifndef RF_VIEWERNEW_H
#define RF_VIEWERNEW_H

#include <functional>
#include <string>

#include <QMouseEvent>
#include <QWheelEvent>
#include <QWidget>

#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <Prs3d_TypeOfHighlight.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>

#include "graphics_scene.h"
#include "rfviewevent.h"
#include "v3d_view_camera_animation.h"
#include "widget_occ_view.h"
#include "widget_occ_view_controller.h"

namespace Mayo
{
using namespace rfapp;

class RfViewerNew : public QWidget
{
private:
    typedef std::function<void(const ViewerEvent &)> viewerEventCallBack;

public:
    enum class KUpdate
    {
        Redraw,
        Update,
        Refresh
    };

    enum class CursorType
    {
        def,
        hand,
        pan,
        glob,
        zoom,
        rot,
        wait
    };

    enum class ViewTrihedronMode
    {
        None,
        V3dViewZBuffer,
        AisViewCube
    };

    void setUserCursor(CursorType type);
    RfViewerNew(QWidget *parent = Q_NULLPTR);
    ~RfViewerNew() override;

    void updateViewer(KUpdate);

    int getDynamicFlag() const;
    void addModel(Handle(AIS_InteractiveObject) _model);
    void hideModel(Handle(AIS_InteractiveObject) _model);

    void setDisplayMode(const Handle(AIS_InteractiveObject) obj, int display_mode);
    void setSelectFlag(int select_flag);
    void updateSelectMode(Handle(AIS_InteractiveObject) _model);
    void clearSelected();
    void reCalcObjSelection(const Handle(AIS_InteractiveObject) obj);
    bool isDisplayed(const Handle(AIS_InteractiveObject) obj);
    void setObjHighlight(const Handle(AIS_InteractiveObject) obj, bool selected);

    void zoomAtPoint(double x, double y, double zoom_factor_x, double zoome_factory_y);
    void startRotation(double ori_x, double ori_y);
    void rotAtPoint(double x, double y);
    void pan(double dx, double dy);

    void setName(const std::string &name);
    std::string getName() const;

    ViewTrihedronMode viewTrihedronMode() const;
    void setViewTrihedronMode(ViewTrihedronMode mode);
    Aspect_TypeOfTriedronPosition viewTrihedronCorner() const;
    void setViewTrihedronCorner(Aspect_TypeOfTriedronPosition corner);
    void v3dViewTrihedronDisplay(Aspect_TypeOfTriedronPosition corner);

    viewerEventCallBack viewerEventCb = NULL;

    bool processAction(const GraphicsOwnerPtr &gfxOwner);

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    QPaintEngine *paintEngine() const override;

private:
    void initContext();
    void initContextAttributes();
    void setHilightAttribute(Prs3d_TypeOfHighlight idx, double lineWidth_aspect,
                             Quantity_Color theColor);
    bool hoveringViewCube();

    ViewerEvent mouseEventToViewInfo(QMouseEvent *event);

private:
    IWidgetOccView *occ_view_ = nullptr;

    GraphicsScene gfxScene_;
    OccHandle<V3d_View> v3dView_;
    V3dViewCameraAnimation *camera_animation_ = nullptr;
    WidgetOccViewController *occ_view_controller_ = nullptr;

    Handle(AIS_InteractiveObject) viewcube_ = nullptr;
    AIS_SelectionScheme selection_scheme_ = AIS_SelectionScheme::AIS_SelectionScheme_Replace;
    std::string name_;
    double zoom_scale_control_ = 0.25;

    Aspect_TypeOfTriedronPosition view_trihedron_corner_ = Aspect_TOTP_RIGHT_LOWER;
    ViewTrihedronMode view_trihedron_mode_ = ViewTrihedronMode::None;

    // AIS_Shape::SelectionType(TopAbs_ShapeEnum::TopAbs_SHAPE);
    int select_flag_ = 0;
};
} // namespace Mayo
#endif
