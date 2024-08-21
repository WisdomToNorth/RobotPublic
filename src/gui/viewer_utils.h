#pragma once
#ifndef VIEWER_UTILS_H
#define VIEWER_UTILS_H

#include <AIS_InteractiveObject.hxx>

#include "viewerwrapper.h"
#include "viewnode.h"
namespace rfgui
{
class ViewerUtils
{
public:
    static void updateViewer(int index);
    static void updateCurrentViewer();

    static void displayVTNode(ViewNode *model);
    static void highLightVTNode(ViewNode *model, bool is_high_light);
    static void removeVTNode(ViewNode *model);
    static void updateVTNodeViewObject(ViewNode *model);

    static bool isDisplayed(ViewNode *model);

    static void clearViewerSelected(int viewer_index, bool redraw);
    static void changeObjSelectedMode(int viewer_index, int mode);

    // Temp objects are not able to be selected
    static void addViewObjToViewer(Handle(AIS_InteractiveObject) temp_model, ViewerCore *viewer,
                                   bool redraw);
    static void removeViewObjFromViewer(Handle(AIS_InteractiveObject) temp_model,
                                        ViewerCore *viewer, bool redraw);

private:
    static void updateObject(Handle(AIS_InteractiveObject) temp_model_, int viewer_index);
    static void removeObject(Handle(AIS_InteractiveObject) temp_model_, int viewer_index);
    static void addToViewer(Handle(AIS_InteractiveObject) temp_model_, int viewer_index);

    static void highlightObject(Handle(AIS_InteractiveObject) temp_model_, bool is_highlight,
                                int viewer_index);

    static void changeObjDisplayMode(Handle(AIS_InteractiveObject) _model, int mode,
                                     int viewer_index);

    static void recursivedisplayVTNode(ViewNode *model, int viewer_index);
    static void recursiveRemoveVTNode(ViewNode *model, int viewer_index);
    static void recursiveHighLightVTNode(ViewNode *model, bool is_high_light, int viewer_index);
    static bool recursiveCheckDisplayed(ViewNode *model, int viewer_index);
};
} // namespace rfgui
#endif
