#include "viewer_observer.h"

#include "app_system.h"
#include "viewer_utils.h"

namespace rfgui
{

void ViewObserver::updateEvent(ViewNode *node, rfapp::KMode mode)
{
    if (!node || node->index_in_viewer_ < 0)
    {
        return;
    }

    switch (mode)
    {
    case rfapp::KMode::add:
    {
        ViewerUtils::displayVTNode(node);
        break;
    }
    case rfapp::KMode::remove:
    {
        ViewerUtils::removeVTNode(node);
        break;
    }
    case rfapp::KMode::update:
    {
        break;
    }
    case rfapp::KMode::hide:
    {
        if (node->is_visible_)
        {
            ViewerUtils::displayVTNode(node);
        }
        else
        {
            ViewerUtils::removeVTNode(node);
        }
        break;
    }
    case rfapp::KMode::highlight:
    {
        ViewerUtils::highLightVTNode(node, node->is_selected_);
        break;
    }
    }
};
} // namespace rfgui
