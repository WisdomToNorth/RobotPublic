/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#include "graphics_utils.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

#include <AIS_InteractiveContext.hxx>
#include <AIS_InteractiveObject.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <Aspect_Grid.hxx>
#include <Aspect_Window.hxx>
#include <Bnd_Box.hxx>
#include <ElSLib.hxx>
#include <Graphic3d_ClipPlane.hxx>
#include <Image_PixMap.hxx>
#include <ProjLib.hxx>
#include <SelectMgr_SelectionManager.hxx>
#include <Standard_Version.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>

#include "system_type.h"
#include "tkernel_utils.h"

namespace Mayo
{

namespace Internal
{

static void AisContext_setObjectVisible(AIS_InteractiveContext *ptrContext,
                                        const OccHandle<AIS_InteractiveObject> &object, bool on)
{
    if (ptrContext && object)
    {
        if (on)
            ptrContext->Display(object, false /*dontUpdateViewer*/);
        else
            ptrContext->Erase(object, false /*dontUpdateViewer*/);
    }
}

} // namespace Internal

void GraphicsUtils::V3dView_fitAll(const OccHandle<V3d_View> &view)
{
    view->ZFitAll();
    view->FitAll(0.01, false /*dontUpdateView*/);
}

bool GraphicsUtils::V3dView_hasClipPlane(const OccHandle<V3d_View> &view,
                                         const OccHandle<Graphic3d_ClipPlane> &plane)
{
    const OccHandle<Graphic3d_SequenceOfHClipPlane> &seqClipPlane = view->ClipPlanes();
    if (seqClipPlane.IsNull() || seqClipPlane->Size() == 0)
        return false;

    for (Graphic3d_SequenceOfHClipPlane::Iterator it(*seqClipPlane); it.More(); it.Next())
    {
        const OccHandle<Graphic3d_ClipPlane> &candidate = it.Value();
        if (candidate.get() == plane.get())
            return true;
    }

    return false;
}

gp_Pnt GraphicsUtils::V3dView_to3dPosition(const OccHandle<V3d_View> &view, double x, double y)
{
    double xEye, yEye, zEye, xAt, yAt, zAt;
    view->Eye(xEye, yEye, zEye);
    view->At(xAt, yAt, zAt);
    const gp_Pnt pntEye(xEye, yEye, zEye);
    const gp_Pnt pntAt(xAt, yAt, zAt);

    const gp_Vec vecEye(pntEye, pntAt);
    const bool vecEyeNotNull = vecEye.SquareMagnitude() > gp::Resolution();
    const gp_Dir dirEye(vecEyeNotNull ? vecEye : gp_Vec{0, 0, 1});

    const gp_Pln planeView(pntAt, dirEye);
    double px, py, pz;
    const int ix = static_cast<int>(std::lround(x));
    const int iy = static_cast<int>(std::lround(y));
    view->Convert(ix, iy, px, py, pz);
    const gp_Pnt pntConverted(px, py, pz);
    const gp_Pnt2d pntConvertedOnPlane = ProjLib::Project(planeView, pntConverted);
    return ElSLib::Value(pntConvertedOnPlane.X(), pntConvertedOnPlane.Y(), planeView);
}

bool GraphicsUtils::V3dViewer_isGridActive(const OccHandle<V3d_Viewer> &viewer)
{
    return viewer->IsGridActive();
}

OccHandle<Aspect_Grid> GraphicsUtils::V3dViewer_grid(const OccHandle<V3d_Viewer> &viewer)
{
    return viewer->Grid(false /*dontCreate*/);
}

GraphicsUtils::AspectGridColors
GraphicsUtils::V3dViewer_gridColors(const OccHandle<V3d_Viewer> &viewer)
{
    AspectGridColors colors;
    OccHandle<Aspect_Grid> gridAspect = V3dViewer_grid(viewer);
    if (gridAspect)
        gridAspect->Colors(colors.base, colors.tenth);

    return colors;
}

void GraphicsUtils::V3dViewer_setGridColors(const OccHandle<V3d_Viewer> &viewer,
                                            const AspectGridColors &colors)
{
    OccHandle<Aspect_Grid> gridAspect = V3dViewer_grid(viewer);
    if (gridAspect)
        gridAspect->SetColors(colors.base, colors.tenth);
}

void GraphicsUtils::AisContext_eraseObject(const OccHandle<AIS_InteractiveContext> &context,
                                           const OccHandle<AIS_InteractiveObject> &object)
{
    if (!object.IsNull() && !context.IsNull())
    {
        context->Erase(object, false /*dontUpdateViewer*/);
        context->Remove(object, false /*dontUpdateViewer*/);
        context->ClearPrs(object, 0, false /*dontUpdateViewer*/);
        context->SelectionManager()->Remove(object);
    }
}

void GraphicsUtils::AisContext_setObjectVisible(const OccHandle<AIS_InteractiveContext> &context,
                                                const OccHandle<AIS_InteractiveObject> &object,
                                                bool on)
{
    Internal::AisContext_setObjectVisible(context.get(), object, on);
}

AIS_InteractiveContext *
GraphicsUtils::AisObject_contextPtr(const OccHandle<AIS_InteractiveObject> &object)
{
    if (!object)
        return nullptr;

    return object->InteractiveContext();
}

bool GraphicsUtils::AisObject_isVisible(const OccHandle<AIS_InteractiveObject> &object)
{
    const AIS_InteractiveContext *ptrContext = AisObject_contextPtr(object);
    return ptrContext ? ptrContext->IsDisplayed(object) : false;
}

void GraphicsUtils::AisObject_setVisible(const OccHandle<AIS_InteractiveObject> &object, bool on)
{
    Internal::AisContext_setObjectVisible(AisObject_contextPtr(object), object, on);
}

Bnd_Box GraphicsUtils::AisObject_boundingBox(const OccHandle<AIS_InteractiveObject> &object)
{
    Bnd_Box box;
    if (object.IsNull())
        return box;

    // Ensure bounding box is calculated
    for (OccHandle<PrsMgr_Presentation> prs : object->Presentations())
    {
        if (prs->Mode() == object->DisplayMode() && !prs->CStructure()->BoundingBox().IsValid())
            prs->CalculateBoundBox();
    }

    object->BoundingBox(box);
    return box;
}

int GraphicsUtils::AspectWindow_width(const OccHandle<Aspect_Window> &wnd)
{
    if (wnd.IsNull())
        return 0;

    int w, h;
    wnd->Size(w, h);
    return w;
}

int GraphicsUtils::AspectWindow_height(const OccHandle<Aspect_Window> &wnd)
{
    if (wnd.IsNull())
        return 0;

    int w, h;
    wnd->Size(w, h);
    return h;
}

OccHandle<Aspect_DisplayConnection> GraphicsUtils::AspectDisplayConnection_create()
{
#if (!defined(MAYO_OS_WINDOWS) && (!defined(MAYO_OS_MAC) || defined(MACOSX_USE_GLX)))
    return new Aspect_DisplayConnection(std::getenv("DISPLAY"));
#else
    return new Aspect_DisplayConnection;
#endif
}

void GraphicsUtils::Gfx3dClipPlane_setCappingHatch(const OccHandle<Graphic3d_ClipPlane> &plane,
                                                   Aspect_HatchStyle hatch)
{
    if (hatch == Aspect_HS_SOLID)
        plane->SetCappingHatchOff();
    else
        plane->SetCappingHatchOn();

    plane->SetCappingHatch(hatch);
}
static double planePosition(const gp_Pln &plane)
{
    const gp_Vec vecLoc(plane.Location().XYZ());
    const gp_Vec vecNormal(plane.Axis().Direction());
    return vecLoc.Dot(vecNormal);
}

static bool isReversedStandardDir(const gp_Dir &n)
{
    for (int i = 0; i < 3; ++i)
    {
        const double c = n.XYZ().GetData()[i];
        if (c < 0 && (std::abs(c) - 1) < Precision::Confusion())
            return true;
    }

    return false;
}

void GraphicsUtils::Gfx3dClipPlane_setNormal(const OccHandle<Graphic3d_ClipPlane> &plane,
                                             const gp_Dir &n)
{
    const double planePos = planePosition(plane->ToPlane());
    const gp_Vec placement(planePos * gp_Vec(n));
    plane->SetEquation(gp_Pln(placement.XYZ(), n));
}
void GraphicsUtils::Gfx3dClipPlane_setPosition(const OccHandle<Graphic3d_ClipPlane> &plane,
                                               double pos)
{
    const gp_Dir &n = plane->ToPlane().Axis().Direction();
    if (isReversedStandardDir(n))
        pos = -pos;

    const gp_Vec placement(pos * gp_Vec(n));
    plane->SetEquation(gp_Pln(placement.XYZ(), n));
}

bool GraphicsUtils::ImagePixmap_flipY(Image_PixMap &pixmap)
{
    return Image_PixMap::FlipY(pixmap);
}

} // namespace Mayo
