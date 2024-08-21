#include "viewer_utils.h"

#include <AIS_Shape.hxx>

#include "app_system.h"

namespace rfgui
{
using namespace rfapp;

void ViewerUtils::updateCurrentViewer()
{
    auto cadview_ = GApp::instance().getCurrentViewer();
    if (cadview_)
    {
        cadview_->updateViewer(ViewerCore::KUpdate::Redraw);
    }
}

void ViewerUtils::updateViewer(int index)
{
    auto cadview_ = GApp::instance().getViewer(index);
    cadview_->updateViewer(ViewerCore::KUpdate::Redraw);
}

void ViewerUtils::removeVTNode(ViewNode *model)
{
    if (!model)
    {
        return;
    }

    recursiveRemoveVTNode(model, model->index_in_viewer_);
    GApp &gapp = GApp::instance();
    auto cadview_ = gapp.getViewer(model->index_in_viewer_);
    cadview_->updateViewer(ViewerCore::KUpdate::Redraw);
}

void ViewerUtils::recursiveRemoveVTNode(ViewNode *model, int viewer_index)
{
    assert(model != nullptr);

    if (model->children_.empty())
    {
        removeObject(model->viewmodel_item_, viewer_index);
    }
    else
    {
        for (const auto &child : model->children_)
        {
            recursiveRemoveVTNode(child, viewer_index);
        }
    }
}

void ViewerUtils::removeObject(Handle(AIS_InteractiveObject) temp_model_, int viewer_index)
{
    auto cadview_ = GApp::instance().getViewer(viewer_index);
    cadview_->hideModel(temp_model_);
}

void ViewerUtils::displayVTNode(ViewNode *model)
{
    if (!model || !model->is_visible_ || model->index_in_viewer_ < 0)
    {
        return;
    }

    recursivedisplayVTNode(model, model->index_in_viewer_);

    auto cadview_ = GApp::instance().getViewer(model->index_in_viewer_);
    cadview_->updateViewer(ViewerCore::KUpdate::Redraw);
}

void ViewerUtils::highLightVTNode(ViewNode *model, bool is_high_light)
{
    if (!model || !model->is_visible_ || model->index_in_viewer_ < 0)
    {
        return;
    }

    recursiveHighLightVTNode(model, is_high_light, model->index_in_viewer_);
    auto cadview_ = GApp::instance().getViewer(model->index_in_viewer_);
    cadview_->updateViewer(ViewerCore::KUpdate::Redraw);
}

void ViewerUtils::recursiveHighLightVTNode(ViewNode *model, bool hightlight, int viewer_index)
{
    assert(model != nullptr);

    if (model->children_.empty())
    {
        highlightObject(model->viewmodel_item_, hightlight, viewer_index);
    }
    else
    {
        for (const auto &child : model->children_)
        {
            recursiveHighLightVTNode(child, hightlight, viewer_index);
        }
    }
}

void ViewerUtils::recursivedisplayVTNode(ViewNode *model, int viewer_index)
{
    assert(model != nullptr);

    if (model->children_.empty())
    {
        addToViewer(model->viewmodel_item_, viewer_index);
    }
    else
    {
        for (const auto &child : model->children_)
        {
            recursivedisplayVTNode(child, viewer_index);
        }
    }
}

void ViewerUtils::addViewObjToViewer(Handle(AIS_InteractiveObject) temp_model, ViewerCore *viewer,
                                     bool redraw)
{
    if (!viewer)
    {
        return;
    }
    viewer->addModel(temp_model);
    if (redraw)
    {
        viewer->updateViewer(ViewerCore::KUpdate::Redraw);
    }
}

void ViewerUtils::removeViewObjFromViewer(Handle(AIS_InteractiveObject) temp_model,
                                          ViewerCore *viewer, bool redraw)
{
    if (!viewer)
    {
        return;
    }
    viewer->hideModel(temp_model);
    if (redraw)
    {
        viewer->updateViewer(ViewerCore::KUpdate::Redraw);
    }
}

void ViewerUtils::addToViewer(Handle(AIS_InteractiveObject) temp_model_, int viewer_index)
{
    if (viewer_index < 0)
    {
        return;
    }
    ViewerCore *cadview_ = GApp::instance().getViewer(viewer_index);
    cadview_->addModel(temp_model_);
    cadview_->updateSelectMode(temp_model_);
}

void ViewerUtils::highlightObject(Handle(AIS_InteractiveObject) temp_model_, bool is_highlight,
                                  int viewer_index)
{
    auto cadview_ = GApp::instance().getViewer(viewer_index);
    cadview_->setObjHighlight(temp_model_, is_highlight);
}

bool ViewerUtils::isDisplayed(ViewNode *model)
{
    if (!model)
    {
        return false;
    }
    return recursiveCheckDisplayed(model, model->index_in_viewer_);
}

// TODO: add unit test
bool ViewerUtils::recursiveCheckDisplayed(ViewNode *model, int viewer_index)
{
    assert(model != nullptr);

    if (model->children_.empty())
    {
        auto cadview_ = GApp::instance().getViewer(viewer_index);
        return cadview_->isDisplayed(model->viewmodel_item_);
        // auto G_Context = cadview_->getContext();
        // return G_Context->IsDisplayed(model->viewmodel_item_);
    }
    else
    {
        for (const auto &child : model->children_)
        {
            if (!recursiveCheckDisplayed(child, viewer_index))
            {
                return false;
            }
        }
    }
    return true;
}

void ViewerUtils::updateVTNodeViewObject(ViewNode *model)
{
    if (!model || !model->is_visible_ || model->index_in_viewer_ < 0)
    {
        return;
    }
    updateObject(model->viewmodel_item_, model->index_in_viewer_);
}

void ViewerUtils::updateObject(Handle(AIS_InteractiveObject) temp_model_, int viewer_index)
{
    auto cadview_ = GApp::instance().getViewer(viewer_index);
    cadview_->reCalcObjSelection(temp_model_);
    // Handle(AIS_InteractiveContext) G_Context = cadview_->getContext();
    // G_Context->Update(temp_model_, false);
    // May be this can be replaced by RecomputeSelectionOnly to enhance performance
}

void ViewerUtils::clearViewerSelected(int viewer_index, bool redraw)
{
    auto cadview_ = GApp::instance().getViewer(viewer_index);
    if (cadview_)
    {
        cadview_->clearSelected();
        if (redraw)
        {
            cadview_->updateViewer(ViewerCore::KUpdate::Redraw);
        }
    }
}

void ViewerUtils::changeObjSelectedMode(int viewer_index, int flag)
{
    if (viewer_index < 0)
    {
        return;
    }
    auto cadview_ = GApp::instance().getViewer(viewer_index);
    cadview_->setSelectFlag(flag);
}

void ViewerUtils::changeObjDisplayMode(Handle(AIS_InteractiveObject) _model, int mode,
                                       int viewer_index)
{
    auto cadview_ = GApp::instance().getViewer(viewer_index);
    cadview_->setDisplayMode(_model, mode);
}

} // namespace rfgui
