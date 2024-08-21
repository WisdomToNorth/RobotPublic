#include "reader_step.h"

#include "stdafx.h"

#include <cassert>
#include <unordered_set>

#include <TopoDS_Shape.hxx>

#include "rfbase/datautils.h"
#include "rfdb/dbPart.h"
#include "rfgeo3d/shapeutils.h"

bool g_merge = false;
namespace rfio
{
bool StepReader::readFile(const std::string &_filepath, rfapp::TaskProgress * /*progress*/)
{
    file_path_ = _filepath;
    STEPCAFControl_Reader l_step_reader;
    l_step_reader.SetColorMode(true);
    l_step_reader.SetNameMode(true);
    l_step_reader.SetLayerMode(true);
    IFSelect_ReturnStatus read_status = l_step_reader.ReadFile(_filepath.c_str());
    if (read_status != IFSelect_ReturnStatus::IFSelect_RetDone)
    {
        return false;
    }

    Handle_TDocStd_Document aDoc;
    Handle(XCAFApp_Application) anApp = XCAFApp_Application::GetApplication();
    anApp->NewDocument("MDTV-XCAF", aDoc); // TODO:
    l_step_reader.Transfer(aDoc);
    l_root_label = aDoc->Main();
    return true;
}

Quantity_Color StepReader::getLabelColors(const TDF_Label &_label)
{
    Quantity_Color l_color;
    Handle(XCAFDoc_ColorTool) aColorTool = XCAFDoc_DocumentTool::ColorTool(_label);
    if (aColorTool->GetColor(_label, XCAFDoc_ColorGen, l_color) ||
        aColorTool->GetColor(_label, XCAFDoc_ColorSurf, l_color) ||
        aColorTool->GetColor(_label, XCAFDoc_ColorCurv, l_color))
    {
        return l_color;
    }
    else
    {
        return Quantity_NOC_GRAY70;
    }
}

std::string StepReader::getLabelNameStp(const TDF_Label &_label)
{
    Handle(TDataStd_Name) name;
    std::string part_name;
    std::string defaultname = "Unnamed";
    if (_label.FindAttribute(TDataStd_Name::GetID(), name))
    {
        TCollection_ExtendedString extstr = name->Get();
        char *str = new char[extstr.LengthOfCString() + 1];
        extstr.ToUTF8CString(str);
        part_name = str;
        delete[] str;
        if (part_name.empty())
        {
            part_name = defaultname;
        }
        else
        {
            bool ws = true;
            for (char it : part_name)
            {
                if (it != ' ')
                {
                    ws = false;
                    break;
                }
            }
            if (ws)
            {
                part_name = defaultname;
            }
        }
    }

    return part_name;
}

size_t HashUpper = UINT_MAX;
void StepReader::loadShapes(const TDF_Label &label, const TopLoc_Location &loc,
                            const std::string &defaultname, const std::string &assembly, bool isRef,
                            std::set<size_t> &myRefShapes, rfdb::dbPart *lValue)
{
    size_t hash = 0;

    TopoDS_Shape aShape;

    Handle(XCAFDoc_ShapeTool) aShapeTool = XCAFDoc_DocumentTool::ShapeTool(label);
    if (aShapeTool->GetShape(label, aShape))
    {
        hash = rfgeo::ShapeUtils::getShapeHash(aShape);
    }

    Handle(TDataStd_Name) name;
    std::string part_name = defaultname;
    if (label.FindAttribute(TDataStd_Name::GetID(), name))
    {
        TCollection_ExtendedString extstr = name->Get();
        char *str = new char[extstr.LengthOfCString() + 1];
        extstr.ToUTF8CString(str);
        part_name = str;
        delete[] str;
        if (part_name.empty())
        {
            part_name = defaultname;
        }
        else
        {
            bool ws = true;
            for (char it : part_name)
            {
                if (it != ' ')
                {
                    ws = false;
                    break;
                }
            }
            if (ws)
            {
                part_name = defaultname;
            }
        }
    }

    TopLoc_Location part_loc = loc;
    Handle(XCAFDoc_Location) hLoc;
    if (label.FindAttribute(XCAFDoc_Location::GetID(), hLoc))
    {
        if (isRef)
        {
            part_loc = part_loc * hLoc->Get();
        }
        else
        {
            part_loc = hLoc->Get();
        }
    }

    std::string asm_name = part_name;
    (void)assembly;

    TDF_Label ref;
    if (aShapeTool->IsReference(label) && aShapeTool->GetReferredShape(label, ref))
    {
        loadShapes(ref, part_loc, part_name, asm_name, true, myRefShapes, lValue);
    }

    rfdb::dbPart *localValue = new rfdb::dbPart();
    if (isRef || myRefShapes.find(hash) == myRefShapes.end())
    {
        TopoDS_Shape l_aShape;
        if (isRef && aShapeTool->GetShape(label, l_aShape))
        {
            hash = rfgeo::ShapeUtils::getShapeHash(aShape);
            myRefShapes.insert(hash);
        }

        if (aShapeTool->IsSimpleShape(label) && (isRef || aShapeTool->IsFree(label)))
        {
            if (!asm_name.empty())
            {
                part_name = asm_name;
            }

            // TODO: The merge parameter (last one from createShape) should become an
            // Importer/Exporter option within the FreeCAD preference menu Currently it is merging
            // STEP Compound Shape into a single Shape Part::Feature which is an OpenCascade
            // computed Compound

            if (isRef)
            {
                createShape(label, loc, part_name, lValue, g_merge);
            }
            else
            {
                createShape(label, part_loc, part_name, localValue, g_merge);
            }
        }
        else
        {
            if (aShapeTool->IsSimpleShape(label))
            {
                // We are not creating a list of Part::Feature in that case but just
                // a single Part::Feature which has as a Shape a Compound of the Subshapes contained
                // within the global shape
                // This is standard behavior of many STEP reader and avoid to register a crazy
                // amount of Shape within the Tree as STEP file do mostly contain large assemblies
                return;
            }

            // This is probably an Assembly let's try to create a Compound with the name
            for (TDF_ChildIterator it(label); it.More(); it.Next())
            {
                if (isRef)
                {
                    loadShapes(it.Value(), part_loc, part_name, asm_name, false, myRefShapes,
                               localValue);
                }
                else
                {
                    loadShapes(it.Value(), part_loc, part_name, asm_name, isRef, myRefShapes,
                               localValue);
                }
            }
        }
    }
    if (localValue->childrenCount() > 0)
    {
        localValue->getProperty()->setName(asm_name);
        tryPlacementFromLoc(localValue, part_loc);
        lValue->addChild(localValue);
    }
}

void StepReader::tryPlacementFromLoc(rfdb::dbPart *part, const TopLoc_Location &part_loc)
{
    gp_Trsf trf;

    if (part_loc.IsIdentity())
    {
        trf = part_loc.Transformation();
    }
    else
    {
        trf = TopLoc_Location(part_loc.FirstDatum()).Transformation();
    }

    std::vector<rfdb::dbPart *> children;
    part->getChildren(children);
    for (auto &child : children)
    {
        [[maybe_unused]] bool succ = child->setShape(child->getTopoShape().Moved(part_loc));
    }
    part->getProperty()->setInitTrsf(trf);
}

rfdb::dbVariant StepReader::transfer(rfapp::TaskProgress *, bool /*concurrent*/)
{
    rfdb::dbPart *aux_root = new rfdb::dbPart();

    std::set<size_t> is_exist;
    loadShapes(l_root_label, TopLoc_Location(), rfbase::DataUtils::getFileName(file_path_), "",
               false, is_exist, aux_root);

    std::vector<rfdb::dbPart *> children;
    aux_root->getChildren(children);
    assert(children.size() == 1);
    rfdb::dbPart *res = children.front();
    aux_root->removeChild(res);
    delete aux_root;
    return rfdb::dbVariant(res);
}

void StepReader::createShape(const TDF_Label &label, const TopLoc_Location &loc,
                             const std::string &name, rfdb::dbPart *lValue, bool mergeShape)
{
    Handle(XCAFDoc_ShapeTool) aShapeTool = XCAFDoc_DocumentTool::ShapeTool(label);
    const TopoDS_Shape &aShape = aShapeTool->GetShape(label);

    if (!aShape.IsNull() && aShape.ShapeType() == TopAbs_COMPOUND)
    {
        TopExp_Explorer xp;
        int ctSolids = 0, ctShells = 0, ctVertices = 0, ctEdges = 0;
        rfdb::dbPart *localValue = new rfdb::dbPart();

        if (mergeShape)
        {
            // We should do that only if there is more than a single shape inside
            // Computing Compounds takes time
            // We must keep track of the Color. If there is more than 1 Color into
            // a STEP Compound then the Merge can't be done and we cancel the operation

            BRep_Builder builder;
            TopoDS_Compound comp;
            builder.MakeCompound(comp);

            for (xp.Init(aShape, TopAbs_SOLID); xp.More(); xp.Next(), ctSolids++)
            {
                const TopoDS_Shape &sh = xp.Current();
                if (!sh.IsNull())
                {
                    builder.Add(comp, sh);
                }
            }

            for (xp.Init(aShape, TopAbs_SHELL, TopAbs_SOLID); xp.More(); xp.Next(), ctShells++)
            {
                const TopoDS_Shape &sh = xp.Current();
                if (!sh.IsNull())
                {
                    builder.Add(comp, sh);
                }
            }

            for (xp.Init(aShape, TopAbs_EDGE); xp.More(); xp.Next(), ctEdges++)
            {
                const TopoDS_Shape &sh = xp.Current();
                if (!sh.IsNull())
                {
                    builder.Add(comp, sh);
                }
            }

            for (xp.Init(aShape, TopAbs_VERTEX); xp.More(); xp.Next(), ctVertices++)
            {
                const TopoDS_Shape &sh = xp.Current();
                if (!sh.IsNull())
                {
                    builder.Add(comp, sh);
                }
            }

            // Ok we got a Compound which is computed
            // Just need to add it to a Part::Feature and push it to lValue
            if (!comp.IsNull() && (ctSolids || ctShells || ctEdges || ctVertices))
            {
                rfdb::dbPart *part = new rfdb::dbPart();

                //  Let's allocate the relative placement of the Compound from the STEP file
                tryPlacementFromLoc(part, loc);
                if (!loc.IsIdentity())
                {
                    [[maybe_unused]] bool succ = part->setShape(comp.Moved(loc));
                    assert(succ);
                }
                else
                {
                    [[maybe_unused]] bool succ = part->setShape(comp);
                    assert(succ);
                }
                part->getProperty()->setName(name);
                lValue->addChild(part);

                part->getProperty()->setColor(getLabelColors(label));
                // loadColors(part, aShape);
            }
        }
        else
        {
            for (xp.Init(aShape, TopAbs_SOLID); xp.More(); xp.Next(), ctSolids++)
            {
                createShape(xp.Current(), label, loc, name, localValue);
            }
            for (xp.Init(aShape, TopAbs_SHELL, TopAbs_SOLID); xp.More(); xp.Next(), ctShells++)
            {
                createShape(xp.Current(), label, loc, name, localValue);
            }
        }

        if (localValue->childrenCount() > 0 && !mergeShape)
        {
            rfdb::dbPart *pcPart = new rfdb::dbPart();
            pcPart->getProperty()->setName(name);
            pcPart->addChild(localValue);
            // localValue contain the objects that  must added to the local Part
            // We must add the PartOrigin and the Part itself

            // Let's compute relative placement of the Part
            lValue->addChild(pcPart);
        }

        if (ctSolids > 0 || ctShells > 0)
        {
            return;
        }
    }
    else if (!aShape.IsNull())
    {
        createShape(aShape, label, loc, name, lValue);
    }
}

void StepReader::createShape(const TopoDS_Shape &aShape, const TDF_Label &_label,
                             const TopLoc_Location &loc, const std::string &name,
                             rfdb::dbPart *lValue)
{
    rfdb::dbPart *part = new rfdb::dbPart();
    if (!loc.IsIdentity())
    {
        [[maybe_unused]] bool succ = part->setShape(aShape.Moved(loc));
        assert(succ);
    }
    else
    {
        [[maybe_unused]] bool succ = part->setShape(aShape);
        assert(succ);
    }
    part->getProperty()->setName(name);

    lValue->addChild(part);
    part->getProperty()->setColor(getLabelColors(_label));
}

} // namespace rfio
