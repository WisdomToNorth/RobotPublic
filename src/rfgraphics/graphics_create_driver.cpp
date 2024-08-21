#include "graphics_create_driver.h"

namespace Mayo
{

using FunctionCreateGraphicsDriver = std::function<Handle_Graphic3d_GraphicDriver()>;

FunctionCreateGraphicsDriver &getFunctionCreateGraphicsDriver()
{
    static FunctionCreateGraphicsDriver fn = []
    { return new OpenGl_GraphicDriver(new Aspect_DisplayConnection); };
    return fn;
}

void setFunctionCreateGraphicsDriver(FunctionCreateGraphicsDriver fn)
{
    getFunctionCreateGraphicsDriver() = std::move(fn);
}

Handle_Graphic3d_GraphicDriver graphicsCreateDriver()
{
    const auto &fn = getFunctionCreateGraphicsDriver();
    if (fn)
        return fn();

    return {};
}

} // namespace Mayo
