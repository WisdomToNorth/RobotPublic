#include "dbpartutils.h"

#include <vector>

#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <ShapeFix_Shape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>

namespace rfdb
{
void dbPartUtils::getAllShapes(const dbPart *part, std::vector<TopoDS_Shape> &shps)
{
    if (part == nullptr)
    {
        return;
    }

    if (part->isLeaf())
    {
        shps.push_back(part->getTopoShape());
        return;
    }

    std::vector<dbPart *> children;
    part->getChildren(children);
    for (size_t n = 0; n < children.size(); ++n)
    {
        getAllShapes(children[n], shps);
    }
}

dbPart *dbPartUtils::createCombinedPart(const dbPart *_model)
{
    std::vector<TopoDS_Shape> shps;
    getAllShapes(_model, shps);

    TopoDS_Shape t_fuse;
    for (size_t n = 0; n < shps.size(); ++n)
    {
        for (TopExp_Explorer exp(shps[n], TopAbs_SHELL); exp.More(); exp.Next())
        {
            if (t_fuse.IsNull())
            {
                t_fuse = exp.Current();
            }
            else
            {
                auto fuse_res = BRepAlgoAPI_Fuse(t_fuse, exp.Current());
                t_fuse = fuse_res;
            }
        }
    }

    BRepBuilderAPI_MakeSolid t_solid;
    for (TopExp_Explorer exp(t_fuse, TopAbs_SHELL); exp.More(); exp.Next())
    {
        t_solid.Add(TopoDS::Shell(exp.Current()));
    }

    auto shp_fix = ShapeFix_Shape(t_solid.Solid());
    shp_fix.Perform();

    dbPart *part = new dbPart(shp_fix.Shape());
    return part;
}

std::string getPreFix(int deep)
{
    std::string res;
    int it_cnt = deep;
    while (it_cnt--)
    {
        res.append("  ");
    }
    return res;
}

void dbPartUtils::printStructOfPart(const dbPart *part, int deep)
{
    if (part == nullptr)
    {
        return;
    }

    if (part->isLeaf())
    {
        std::cout << getPreFix(deep) << part->getId() << '\n';
    }
    else
    {
        std::cout << getPreFix(deep) << part->getId() << '\n';
        std::vector<dbPart *> children;
        part->getChildren(children);
        for (size_t n = 0; n < children.size(); ++n)
        {
            printStructOfPart(children[n], deep + 2);
        }
    }
}

bool dbPartUtils::isPartEqual(const dbPart *lhs, const dbPart *rhs)
{
    if (lhs == nullptr || rhs == nullptr)
    {
        return false;
    }

    if (lhs->isLeaf() && rhs->isLeaf())
    {
        return lhs->getId() == rhs->getId();
    }

    if (lhs->isLeaf() || rhs->isLeaf())
    {
        return false;
    }

    if (lhs->getId() != rhs->getId())
    {
        return false;
    }

    std::vector<dbPart *> lhs_children;
    lhs->getChildren(lhs_children);
    std::vector<dbPart *> rhs_children;
    rhs->getChildren(rhs_children);

    if (lhs_children.size() != rhs_children.size())
    {
        return false;
    }

    for (size_t n = 0; n < lhs_children.size(); ++n)
    {
        if (!isPartEqual(lhs_children[n], rhs_children[n]))
        {
            return false;
        }
    }

    return true;
}

bool dbPartUtils::updatePartVariant(dbPart * /*part*/)
{
    return false;
};
} // namespace rfdb
