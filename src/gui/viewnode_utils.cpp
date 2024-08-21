#include "viewnode_utils.h"

#include <algorithm>
#include <cstdint>
#include <queue>

#include <AIS_PointCloud.hxx>

#include "rfdb/dbMesh.h"
#include "rfdb/dbPart.h"
#include "rfdb/dbPointCloud.h"
#include "rfdb/dbRobot.h"
#include "rfdb/dbVariant.h"
#include "rfgeo3d/vmutils.h"

namespace rfgui
{
void ViewNodeUtils::setViewerIndex(ViewNode *root, int index)
{
    if (!root)
    {
        return;
    }

    root->index_in_viewer_ = index;
    for (auto child : root->children_)
    {
        setViewerIndex(child, index);
    }
}

void ViewNodeUtils::setUseType(ViewNode *root, UseType type)
{
    if (!root)
    {
        return;
    }

    root->type_ = type;
    for (auto child : root->children_)
    {
        setUseType(child, type);
    }
}

ViewNode *recursiveBuildTree(rfdb::dbPart *part)
{
    ViewNode *root = new ViewNode(rfdb::dbVariant(part));
    root->is_selected_ = false;
    root->is_visible_ = part->getProperty()->isVisiable();
    root->is_box_visible_ = part->getProperty()->isVisiable();
    root->transparency_ = part->getProperty()->getTransparency();
    root->name_ = part->getName().empty() ? part->getId() : part->getName();

    if (part->isLeaf())
    {
        if (part->getShape().IsNull())
        {
            return nullptr;
        }
        root->viewmodel_item_ = part->getShape();
        root->viewmodel_item_->SetTransparency(root->transparency_);
        return root;
    }
    else
    {
        root->viewmodel_item_ = Handle(AIS_ColoredShape)(new AIS_ColoredShape(TopoDS_Shape()));

        std::vector<rfdb::dbPart *> children;
        part->getChildren(children);
        for (auto &child : children)
        {
            auto child_node = recursiveBuildTree(child);
            root->viewmodel_item_->AddChild(child_node->viewmodel_item_);
            root->children_.push_back(child_node);
        }

        return root;
    }
};

ViewNode *ViewNodeUtils::buildTree(const rfdb::dbVariant &db_variant)
{
    ViewNode *root = nullptr;

    switch (db_variant.getType())
    {
    case rfdb::dbVariant::Type::Mesh:
    {
        rfdb::dbMesh *mesh_var = db_variant.toMesh();
        const auto &model = mesh_var->getMesh();
        model->SetTransparency(mesh_var->getProperty()->getTransparency());

        root = new ViewNode(db_variant);
        root->name_ = mesh_var->getProperty()->getName();
        root->viewmodel_item_ = model;
        root->is_selected_ = false;
        root->is_visible_ = true;
        root->is_box_visible_ = true;
        break;
    }
    case rfdb::dbVariant::Type::PointCloud:
    {
        auto point_cloud_var = db_variant.toPointCloud();
        auto pnts = point_cloud_var->getPoints();
        Handle(AIS_PointCloud) ais_pnts = new AIS_PointCloud();
        ais_pnts->SetPoints(pnts);
        ais_pnts->SetColor(Quantity_NOC_RED);
        ais_pnts->SetWidth(2.0);
        ais_pnts->SetTransparency(point_cloud_var->getProperty()->getTransparency());

        root = new ViewNode(db_variant);
        root->name_ = point_cloud_var->getProperty()->getName();
        root->viewmodel_item_ = ais_pnts;
        root->is_selected_ = false;
        root->is_visible_ = true;
        root->is_box_visible_ = true;
        break;
    }
    case rfdb::dbVariant::Type::Part:
    {
        root = recursiveBuildTree(db_variant.toPart());
        break;
    }
    case rfdb::dbVariant::Type::Robot:
    {
        root = recursiveBuildTree(db_variant.toRobot()->getModel());
        break;
    }
    case rfdb::dbVariant::Type::Trace:
    case rfdb::dbVariant::Type::Trajectory:
    case rfdb::dbVariant::Type::Assembly:
    case rfdb::dbVariant::Type::Empty:
    {
        root = new ViewNode(db_variant);
        break;
    }
    }

    return root;
}

bool ViewNodeUtils::updateViewNodeToDbVariant(ViewNode *root)
{
    if (!root)
    {
        return false;
    }
    root->db_variant_.getProperty()->setInitTrsf(root->viewmodel_item_->LocalTransformation());
    return true;
}

void ViewNodeUtils::transform(ViewNode *root, const gp_Trsf &trsf)
{
    if (!root)
    {
        return;
    }

    rfgeo::VMUtils::transformLeft(root->viewmodel_item_, trsf);
}
} // namespace rfgui
