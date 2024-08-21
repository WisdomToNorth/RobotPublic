/****************************************************************************
** Copyright (c) 2022, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/
#include <functional>

#include <Aspect_DisplayConnection.hxx>
#include <Aspect_NeutralWindow.hxx>
#include <OpenGl_Context.hxx>
#include <OpenGl_FrameBuffer.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <Standard_Version.hxx>

#include "occ_handle.h"

namespace Mayo
{
// Defined in widget_occ_view.cpp
bool QOpenGLWidgetOccView_isCoreProfile();
void QOpenGLWidgetOccView_createOpenGlContext(
    std::function<void(Aspect_RenderingContext)> fnCallback);
OccHandle<Graphic3d_GraphicDriver> QOpenGLWidgetOccView_createCompatibleGraphicsDriver();
bool QOpenGLWidgetOccView_wrapFrameBuffer(const OccHandle<Graphic3d_GraphicDriver> &);
Graphic3d_Vec2i
QOpenGLWidgetOccView_getDefaultframeBufferViewportSize(const OccHandle<Graphic3d_GraphicDriver> &);
OccHandle<Graphic3d_GraphicDriver> QWidgetOccView_createCompatibleGraphicsDriver();

} // namespace Mayo
