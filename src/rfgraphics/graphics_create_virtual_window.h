/****************************************************************************
+** Copyright (c) 2022, Fougue Ltd. <http://www.fougue.pro>
+** All rights reserved.
+** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
+****************************************************************************/

// --
// NOTE
// This file isolates inclusion of <Aspect_DisplayConnection.hxx> which is problematic on X11/Linux
// <X.h> #defines constants like "None" which causes name clash with
// GuiDocument::ViewTrihedronMode::None
// --

#include "system_type.h"

#ifdef RF_OS_WINDOWS
#include <windows.h>
#endif

#include <Aspect_DisplayConnection.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <Standard_Handle.hxx>

namespace Mayo
{

Handle(Aspect_Window)
    graphicsCreateVirtualWindow([[maybe_unused]] const Handle(Graphic3d_GraphicDriver) & gfxDriver,
                                int wndWidth, int wndHeight);

} // namespace Mayo
