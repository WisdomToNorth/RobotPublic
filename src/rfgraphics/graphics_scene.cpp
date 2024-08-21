/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#include "graphics_scene.h"

#include <unordered_set>

#include <AIS_Shape.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <V3d_AmbientLight.hxx>
#include <V3d_DirectionalLight.hxx>
#include <V3d_TypeOfOrientation.hxx>

#include "graphics_utils.h"
#include "tkernel_utils.h"

namespace Mayo
{

// Defined in graphics_create_driver.cpp
Handle_Graphic3d_GraphicDriver graphicsCreateDriver();

namespace Internal
{

static Handle_V3d_Viewer createOccViewer()
{
    Handle_V3d_Viewer viewer = new V3d_Viewer(graphicsCreateDriver());
    viewer->SetDefaultViewSize(1000.);
    viewer->SetDefaultViewProj(V3d_XposYnegZpos);
    viewer->SetComputedMode(true);
    viewer->SetDefaultComputedMode(true);
    viewer->SetDefaultLights();
    viewer->SetLightOn();

    return viewer;
}

} // namespace Internal

namespace
{

class InteractiveContext : public AIS_InteractiveContext
{
    DEFINE_STANDARD_RTTI_INLINE(InteractiveContext, AIS_InteractiveContext)
public:
    InteractiveContext(const Handle_V3d_Viewer &viewer)
        : AIS_InteractiveContext(viewer)
    {
    }

    constexpr const GraphicsOwnerPtr &member_myLastPicked() const
    {
        return myLastPicked;
    }

    int defaultDisplayMode(const GraphicsObjectPtr &object) const
    {
        int displayMode;
        int hilightMode;
        int selMode;
        this->GetDefModes(object, displayMode, hilightMode, selMode);
        return displayMode;
    }
};

DEFINE_STANDARD_HANDLE(InteractiveContext, AIS_InteractiveContext)

} // namespace

class GraphicsScene::Private
{
public:
    opencascade::handle<InteractiveContext> context_;
    std::unordered_set<const AIS_InteractiveObject *> set_clip_plane_sensitive_;
    bool is_redraw_blocked_ = false;
    SelectionMode selection_mode_ = SelectionMode::Single;
};

GraphicsScene::GraphicsScene()
    : impl_(new Private)
{
    impl_->context_ = new InteractiveContext(Internal::createOccViewer());
}

GraphicsScene::~GraphicsScene()
{
    // Preventive cleaning fixes weird crash happening in MSVC Debug mode
    impl_->context_->RemoveFilters();
    impl_->context_->Deactivate();
    impl_->context_->EraseAll(false);
    impl_->context_->RemoveAll(false);
    delete impl_;
}

opencascade::handle<V3d_View> GraphicsScene::createV3dView()
{
    return this->v3dViewer()->CreateView();
}

const opencascade::handle<V3d_Viewer> &GraphicsScene::v3dViewer() const
{
    return impl_->context_->CurrentViewer();
}

const opencascade::handle<StdSelect_ViewerSelector3d> &GraphicsScene::mainSelector() const
{
    return impl_->context_->MainSelector();
}

bool GraphicsScene::hiddenLineDrawingOn() const
{
    return impl_->context_->DrawHiddenLine();
}

const opencascade::handle<Prs3d_Drawer> &GraphicsScene::drawerDefault() const
{
    return impl_->context_->DefaultDrawer();
}

const opencascade::handle<Prs3d_Drawer> &
GraphicsScene::drawerHighlight(Prs3d_TypeOfHighlight style) const
{
    return impl_->context_->HighlightStyle(style);
}

void GraphicsScene::addObject(const GraphicsObjectPtr &object, AddObjectFlags flags)
{
    if (object)
    {
        if ((flags & AddObjectDisableSelectionMode) != 0)
        {
            const bool onEntry_AutoActivateSelection = impl_->context_->GetAutoActivateSelection();
            const int defaultDisplayMode = impl_->context_->defaultDisplayMode(object);
            impl_->context_->SetAutoActivateSelection(false);
            impl_->context_->Display(object, defaultDisplayMode, -1, false);
            impl_->context_->SetAutoActivateSelection(onEntry_AutoActivateSelection);
        }
        else
        {
            impl_->context_->Display(object, false);
        }
    }
}

void GraphicsScene::eraseObject(const GraphicsObjectPtr &object)
{
    GraphicsUtils::AisContext_eraseObject(impl_->context_, object);
    impl_->set_clip_plane_sensitive_.erase(object.get());
}

void GraphicsScene::redraw()
{
    if (impl_->is_redraw_blocked_)
        return;

    for (auto itView = this->v3dViewer()->DefinedViewIterator(); itView.More(); itView.Next())
    {
        itView.Value()->Redraw();
    }
}

void GraphicsScene::redraw(const Handle_V3d_View &view)
{
    if (impl_->is_redraw_blocked_)
        return;

    for (auto itView = this->v3dViewer()->DefinedViewIterator(); itView.More(); itView.Next())
    {
        if (itView.Value() == view)
        {
            itView.Value()->Redraw();
            break;
        }
    }
}

bool GraphicsScene::isRedrawBlocked() const
{
    return impl_->is_redraw_blocked_;
}

void GraphicsScene::blockRedraw(bool on)
{
    impl_->is_redraw_blocked_ = on;
}

void GraphicsScene::recomputeObjectPresentation(const GraphicsObjectPtr &object)
{
    impl_->context_->Redisplay(object, false);
}

void GraphicsScene::activateObjectSelection(const GraphicsObjectPtr &object, int mode)
{
    impl_->context_->Activate(object, mode);
}

void GraphicsScene::deactivateObjectSelection(const Mayo::GraphicsObjectPtr &object, int mode)
{
    impl_->context_->Deactivate(object, mode);
}

void GraphicsScene::deactivateObjectSelection(const GraphicsObjectPtr &object)
{
    impl_->context_->Deactivate(object);
}

void GraphicsScene::highlightObject(const GraphicsObjectPtr &object, bool on)
{
    if (on)
    {
        impl_->context_->HilightWithColor(object, impl_->context_->HighlightStyle(), false);
    }
    else
    {
        impl_->context_->Unhilight(object, false);
    }
}

void GraphicsScene::addSelectionFilter(const OccHandle<SelectMgr_Filter> &filter)
{
    impl_->context_->AddFilter(filter);
}

void GraphicsScene::removeSelectionFilter(const OccHandle<SelectMgr_Filter> &filter)
{
    impl_->context_->RemoveFilter(filter);
}

void GraphicsScene::clearSelectionFilters()
{
    impl_->context_->RemoveFilters();
}

void GraphicsScene::setObjectDisplayMode(const GraphicsObjectPtr &object, int displayMode)
{
    impl_->context_->UnsetDisplayMode(object, false);
    impl_->context_->SetDisplayMode(object, displayMode, false);
    if (impl_->context_->IsDisplayed(object))
    {
        impl_->context_->Redisplay(object, true, true);
    }
}

bool GraphicsScene::isObjectClipPlaneSensitive(const GraphicsObjectPtr &object) const
{
    if (object.IsNull())
        return false;

    return impl_->set_clip_plane_sensitive_.find(object.get()) !=
           impl_->set_clip_plane_sensitive_.cend();
}

void GraphicsScene::setObjectClipPlaneSensitive(const GraphicsObjectPtr &object, bool on)
{
    if (object.IsNull())
        return;

    if (on)
        impl_->set_clip_plane_sensitive_.insert(object.get());
    else
        impl_->set_clip_plane_sensitive_.erase(object.get());
}

bool GraphicsScene::isObjectVisible(const GraphicsObjectPtr &object) const
{
    return impl_->context_->IsDisplayed(object);
}

void GraphicsScene::setObjectVisible(const GraphicsObjectPtr &object, bool on)
{
    GraphicsUtils::AisContext_setObjectVisible(impl_->context_, object, on);
}

gp_Trsf GraphicsScene::objectTransformation(const GraphicsObjectPtr &object) const
{
    return impl_->context_->Location(object);
}

void GraphicsScene::setObjectTransformation(const GraphicsObjectPtr &object, const gp_Trsf &trsf)
{
    impl_->context_->SetLocation(object, trsf);
}

GraphicsOwnerPtr GraphicsScene::firstSelectedOwner() const
{
    impl_->context_->InitSelected();
    if (impl_->context_->MoreSelected())
        return impl_->context_->SelectedOwner();

    return {};
}

void GraphicsScene::clearSelection()
{
    impl_->context_->ClearDetected(false);
    impl_->context_->ClearSelected(false);
    // const bool onEntryOwnerSelected = !this->firstSelectedOwner().IsNull();
    // if (onEntryOwnerSelected)
    // this->signalSelectionChanged.send();
}

AIS_InteractiveContext *GraphicsScene::aisContextPtr() const
{
    return impl_->context_.get();
}

void GraphicsScene::toggleOwnerSelection(const GraphicsOwnerPtr &gfxOwner)
{
    auto gfxObject = GraphicsObjectPtr::DownCast(gfxOwner ? gfxOwner->Selectable() :
                                                            Handle_SelectMgr_SelectableObject());
    if (GraphicsUtils::AisObject_isVisible(gfxObject))
    {
        impl_->context_->AddOrRemoveSelected(gfxOwner, false);
    }
}

void GraphicsScene::highlightAt(int xPos, int yPos, const OccHandle<V3d_View> &view)
{
    impl_->context_->MoveTo(xPos, yPos, view, true); // TODO:
}

void GraphicsScene::select()
{
    if (impl_->selection_mode_ == SelectionMode::None)
        return;

    if (impl_->selection_mode_ == SelectionMode::Single)
    {
        impl_->context_->SelectDetected(AIS_SelectionScheme_Replace);
    }
    else if (impl_->selection_mode_ == SelectionMode::Multi)
    {
        impl_->context_->SelectDetected(AIS_SelectionScheme_XOR);
    }

    // this->signalSelectionChanged.send();
}

int GraphicsScene::selectedCount() const
{
    return impl_->context_->NbSelected();
}

const GraphicsOwnerPtr &GraphicsScene::currentHighlightedOwner() const
{
    return impl_->context_->DetectedOwner();
}

GraphicsScene::SelectionMode GraphicsScene::selectionMode() const
{
    return impl_->selection_mode_;
}

void GraphicsScene::setSelectionMode(GraphicsScene::SelectionMode mode)
{
    if (mode != impl_->selection_mode_)
    {
        impl_->selection_mode_ = mode;
    }
}

void GraphicsScene::setObjSelectionMode(const GraphicsObjectPtr &_model, int select_flag)
{
    if (!_model)
    {
        return;
    }

    if (_model->IsKind(STANDARD_TYPE(AIS_Shape)))
    {
        Handle(AIS_Shape) ais_shape = Handle(AIS_Shape)::DownCast(_model);
        impl_->context_->Deactivate(ais_shape);
        auto select_concurrency =
            AIS_SelectionModesConcurrency::AIS_SelectionModesConcurrency_Multiple;

        int flag = select_flag;
        int mode = 0;
        // TODO: this will reset all activate mode, we can optimize it if needed
        do
        {
            if (flag & 1)
            {
                impl_->context_->SetSelectionModeActive(ais_shape, mode, true, select_concurrency);
            }
            mode++;
        } while (flag >>= 1);
    }
}

GraphicsSceneRedrawBlocker::GraphicsSceneRedrawBlocker(GraphicsScene *scene)
    : m_scene(scene)
    , m_isRedrawBlockedOnEntry(scene->isRedrawBlocked())
{
    scene->blockRedraw(true);
}

GraphicsSceneRedrawBlocker::~GraphicsSceneRedrawBlocker()
{
    m_scene->blockRedraw(m_isRedrawBlockedOnEntry);
}

} // namespace Mayo
