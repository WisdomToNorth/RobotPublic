
#pragma once
#ifndef READER_STEP_H
#define READER_STEP_H

#include <set>
#include <string>

#include <AIS_ColoredShape.hxx>
#include <TDF_Label.hxx>

#include "reader_base.h"

class XCAFDoc_ShapeTool;
class TDF_Label;
class XCAFDoc_ColorTool;

namespace rfio
{
class StepReader : public FileReaderBase
{
    TDF_Label l_root_label;
    std::string file_path_;

public:
    bool readFile(const std::string &filepath, rfapp::TaskProgress *progress) override;
    rfdb::dbVariant transfer(rfapp::TaskProgress *progress, bool concurrent) override;

private:
    // static void loadShapesnew(const TDF_Label &_label, rfdb::dbPart *_parent_part,
    //                           std::set<int> &is_exist);
    static std::string getLabelNameStp(const TDF_Label &_label);
    static Quantity_Color getLabelColors(const TDF_Label &_label);
    // static bool shapeExist(const TDF_Label &_label, std::set<int> &shp_set, bool _is_ref);
    static rfdb::dbPart *getSimplePartVar(const TDF_Label &_label);
    static void createShape(const TDF_Label &label, const TopLoc_Location &loc,
                            const std::string &name, rfdb::dbPart *lValue, bool mergeShape);
    static void createShape(const TopoDS_Shape &aShape, const TDF_Label &_label,
                            const TopLoc_Location &loc, const std::string &name,
                            rfdb::dbPart *_parent);
    static void tryPlacementFromLoc(rfdb::dbPart *part, const TopLoc_Location &part_loc);
    static void loadShapes(const TDF_Label &label, const TopLoc_Location &loc,
                           const std::string &defaultname, const std::string &assembly, bool isRef,
                           std::set<size_t> &myRefShapes, rfdb::dbPart *lValue);
    // static void loadColors(rfdb::dbPart *part, const TopoDS_Shape &aShape);
};
} // namespace rfio
#endif
