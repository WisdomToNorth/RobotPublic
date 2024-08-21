#pragma once
#ifndef RF_VIEWER_H
#define RF_VIEWER_H

#include <functional>
#include <string>

#include <QMouseEvent>
#include <QWheelEvent>
#include <QWidget>

#ifdef _WIN32
#include <WNT_Window.hxx>
#elif __linux__
#include <Xw_Window.hxx>
#elif __APPLE__
#include <Cocoa_Window.hxx>
#elif defined(__ANDROID__)
#include <Aspect_NeutralWindow.hxx>
#endif

#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <Prs3d_TypeOfHighlight.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>

#include "rfviewevent.h"

namespace rfapp
{
class RfViewer : public QWidget
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
    void setUserCursor(CursorType type);
    RfViewer(QWidget *parent = Q_NULLPTR);
    ~RfViewer() override;

    inline Handle(AIS_InteractiveContext) getContext()
    {
        return context_;
    }

    void updateViewer(KUpdate);
    void removeAll(bool redraw = true);

    void setSelectFlag(int select_flag);
    int getDynamicFlag() const;
    void updateSelectMode(Handle(AIS_InteractiveObject) _model);
    void clearSelected();
    void addModel(Handle(AIS_InteractiveObject) _model);
    void setObjHighlight(Handle(AIS_InteractiveObject) _model, bool on);
    void setDisplayMode(const Handle(AIS_InteractiveObject) obj, int display_mode);
    bool isDisplayed(Handle(AIS_InteractiveObject) _model);
    void reCalcObjSelection(Handle(AIS_InteractiveObject) _model);
    void hideModel(Handle(AIS_InteractiveObject) _model);

    void zoomAtPoint(double x, double y, double zoom_factor_x, double zoome_factory_y);
    void startRotation(double ori_x, double ori_y);
    void rotAtPoint(double x, double y);
    void pan(double dx, double dy);

    void setName(const std::string &name);
    std::string getName() const;

    viewerEventCallBack viewerEventCb = NULL;

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    QPaintEngine *paintEngine() const override;

private:
    void initContext();
    void initHilightAttributes();
    void initContextAttributes();
    void setHilightAttribute(Prs3d_TypeOfHighlight idx, double lineWidth_aspect,
                             Quantity_Color theColor);
    void setViewCube();
    bool checkDetectedValid();

    ViewerEvent mouseEventToViewInfo(QMouseEvent *event);

private:
    Handle(AIS_InteractiveContext) context_ = nullptr;
    Handle(AIS_InteractiveObject) viewcube_ = nullptr;
    Handle(V3d_Viewer) v_viewer_ = nullptr;
    Handle(Graphic3d_GraphicDriver) graphic_driver_ = nullptr;
    Handle(V3d_View) view_ = nullptr;

    AIS_SelectionScheme selection_scheme_ = AIS_SelectionScheme::AIS_SelectionScheme_Replace;
    std::string name_;
    double zoom_scale_control_ = 0.25;

    // AIS_Shape::SelectionType(TopAbs_ShapeEnum::TopAbs_SHAPE);
    int select_flag_ = 0;
};
} // namespace rfapp
#endif
