/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

// --
// NOTE
// This file isolates inclusion of <Aspect_DisplayConnection.hxx> which is problematic on X11/Linux
// <X.h> #defines constants like "None" which causes name clash with
// GuiDocument::ViewTrihedronMode::None
// --

#include <functional>

#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>

namespace Mayo
{

using FunctionCreateGraphicsDriver = std::function<Handle_Graphic3d_GraphicDriver()>;

FunctionCreateGraphicsDriver &getFunctionCreateGraphicsDriver();

void setFunctionCreateGraphicsDriver(FunctionCreateGraphicsDriver fn);

Handle_Graphic3d_GraphicDriver graphicsCreateDriver();

} // namespace Mayo
