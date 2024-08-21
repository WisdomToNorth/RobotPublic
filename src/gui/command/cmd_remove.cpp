#include "cmd_remove.h"

#include "app_system.h"
namespace rfgui
{
void CmdRemove::execute()
{
    // auto selected_nodes = GApp::instance().getSelectedNodes();
    // for (auto node : selected_nodes)
    // {
    //     removeNode(node);
    // }
    // GApp::instance().clearSelectedNodes();
    this->done();
}

bool CmdRemove::removeNode(ViewNode *node)
{
    if (node == nullptr)
    {
        return false;
    }
    return GApp::instance().removeDbVariant(node);
} // namespace rfgui
} // namespace rfgui
