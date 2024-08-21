
#include "shapebuilder.h"

#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepLib.hxx>
#include <BRepOffsetAPI_MakeOffset.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeWedge.hxx>
#include <GCE2d_MakeSegment.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeSegment.hxx>
#include <Geom2d_Ellipse.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom_Plane.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Wire.hxx>
#include <gp_Pnt.hxx>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>

#include "rfdb/dbPart.h"

namespace rfdb
{

void ShapeBuilder::makeTestShapes(const double myWidth, const double myThickness,
                                  std::vector<TopoDS_Shape> &res)
{
    // 轮廓：定义支持点
    gp_Pnt aPnt1(-myWidth * 0.5, 0, 0);
    gp_Pnt aPnt2(-myWidth * 0.5, -myThickness * 0.25, 0);
    gp_Pnt aPnt3(0, -myThickness * 0.5, 0);
    gp_Pnt aPnt4(myWidth * 0.5, -myThickness * 0.25, 0);
    gp_Pnt aPnt5(myWidth * 0.5, 0, 0);
    // 轮廓：定义几何图形
    Handle(Geom_TrimmedCurve) anArcOfCircle = GC_MakeArcOfCircle(aPnt2, aPnt3, aPnt4);
    Handle(Geom_TrimmedCurve) aSegment1 = GC_MakeSegment(aPnt1, aPnt2);
    Handle(Geom_TrimmedCurve) aSegment2 = GC_MakeSegment(aPnt4, aPnt5);
    // 轮廓：定义拓扑结构
    const TopoDS_Edge &anEdge1 = BRepBuilderAPI_MakeEdge(aSegment1);
    const TopoDS_Edge &anEdge2 = BRepBuilderAPI_MakeEdge(anArcOfCircle);
    const TopoDS_Edge &anEdge3 = BRepBuilderAPI_MakeEdge(aSegment2);
    TopoDS_Wire aWire = BRepBuilderAPI_MakeWire(anEdge1, anEdge2, anEdge3);
    // 轮廓：完成轮廓
    gp_Ax1 xAxis = gp::OX(); // 获得X轴
    gp_Trsf aTrsf;
    aTrsf.SetMirror(xAxis);
    BRepBuilderAPI_Transform aBRepTrsf(aWire, aTrsf);
    TopoDS_Shape aMirroredShape = aBRepTrsf.Shape();
    TopoDS_Wire aMirroredWire = TopoDS::Wire(aMirroredShape);
    BRepBuilderAPI_MakeWire mkWire;
    mkWire.Add(aWire);
    mkWire.Add(aMirroredWire);
    TopoDS_Wire myWireProfile = mkWire.Wire();

    BRepOffsetAPI_MakeOffset ori(myWireProfile);
    ori.Init();
    ori.Perform(10.0);
    if (ori.IsDone())
    {
        res.push_back(ori.Shape());
    }
    res.push_back(myWireProfile);
}

TopoDS_Shape ShapeBuilder::makeBottle(const double myWidth, const double myHeight,
                                      const double myThickness)
{
    // 轮廓：定义支持点
    gp_Pnt aPnt1(-myWidth * 0.5, 0, 0);
    gp_Pnt aPnt2(-myWidth * 0.5, -myThickness * 0.25, 0);
    gp_Pnt aPnt3(0, -myThickness * 0.5, 0);
    gp_Pnt aPnt4(myWidth * 0.5, -myThickness * 0.25, 0);
    gp_Pnt aPnt5(myWidth * 0.5, 0, 0);
    // 轮廓：定义几何图形
    Handle(Geom_TrimmedCurve) anArcOfCircle = GC_MakeArcOfCircle(aPnt2, aPnt3, aPnt4);
    Handle(Geom_TrimmedCurve) aSegment1 = GC_MakeSegment(aPnt1, aPnt2);
    Handle(Geom_TrimmedCurve) aSegment2 = GC_MakeSegment(aPnt4, aPnt5);
    // 轮廓：定义拓扑结构
    const TopoDS_Edge &anEdge1 = BRepBuilderAPI_MakeEdge(aSegment1);
    const TopoDS_Edge &anEdge2 = BRepBuilderAPI_MakeEdge(anArcOfCircle);
    const TopoDS_Edge &anEdge3 = BRepBuilderAPI_MakeEdge(aSegment2);
    TopoDS_Wire aWire = BRepBuilderAPI_MakeWire(anEdge1, anEdge2, anEdge3);
    // 轮廓：完成轮廓
    gp_Ax1 xAxis = gp::OX(); // 获得X轴
    gp_Trsf aTrsf;
    aTrsf.SetMirror(xAxis);
    BRepBuilderAPI_Transform aBRepTrsf(aWire, aTrsf);
    TopoDS_Shape aMirroredShape = aBRepTrsf.Shape();
    TopoDS_Wire aMirroredWire = TopoDS::Wire(aMirroredShape);
    BRepBuilderAPI_MakeWire mkWire;
    mkWire.Add(aWire);
    mkWire.Add(aMirroredWire);
    TopoDS_Wire myWireProfile = mkWire.Wire();
    // 瓶身：拉伸轮廓
    TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace(myWireProfile);
    gp_Vec aPrismVec(0, 0, myHeight);
    TopoDS_Shape myBody = BRepPrimAPI_MakePrism(myFaceProfile, aPrismVec);
    // 瓶身：倒角
    BRepFilletAPI_MakeFillet mkFillet(myBody);
    TopExp_Explorer anEdgeExplorer(myBody, TopAbs_EDGE);
    while (anEdgeExplorer.More())
    {
        const TopoDS_Edge &anEdge = TopoDS::Edge(anEdgeExplorer.Current());
        // Add edge to fillet algorithm
        mkFillet.Add(myThickness / 12., anEdge);
        anEdgeExplorer.Next();
    }
    myBody = mkFillet.Shape();
    // 瓶身：添加瓶颈
    gp_Pnt neckLocation(0, 0, myHeight);
    gp_Dir neckAxis = gp::DZ();
    gp_Ax2 neckAx2(neckLocation, neckAxis);
    double myNeckRadius = myThickness * 0.25;
    double myNeckHeight = myHeight * 0.1;
    BRepPrimAPI_MakeCylinder MKCylinder(neckAx2, myNeckRadius, myNeckHeight);
    TopoDS_Shape myNeck = MKCylinder.Shape();
    myBody = BRepAlgoAPI_Fuse(myBody, myNeck);
    // 瓶身：创造中空的实体
    TopoDS_Face faceToRemove;
    double zMax = -1;
    for (TopExp_Explorer aFaceExplorer(myBody, TopAbs_FACE); aFaceExplorer.More();
         aFaceExplorer.Next())
    {
        TopoDS_Face aFace = TopoDS::Face(aFaceExplorer.Current());
        // Check if <aFace> is the top face of the bottle's neck
        Handle(Geom_Surface) aSurface = BRep_Tool::Surface(aFace);
        if (aSurface->DynamicType() == STANDARD_TYPE(Geom_Plane))
        {
            Handle(Geom_Plane) aPlane = Handle(Geom_Plane)::DownCast(aSurface);
            gp_Pnt aPnt = aPlane->Location();
            double aZ = aPnt.Z();
            if (aZ > zMax)
            {
                zMax = aZ;
                faceToRemove = aFace;
            }
        }
    }
    TopTools_ListOfShape facesToRemove;
    facesToRemove.Append(faceToRemove);
    BRepOffsetAPI_MakeThickSolid BodyMaker;
    BodyMaker.MakeThickSolidByJoin(myBody, facesToRemove, -myThickness / 50, 1.e-3);
    myBody = BodyMaker.Shape();

    // 螺纹：创建表面
    Handle(Geom_CylindricalSurface) aCyl1 =
        new Geom_CylindricalSurface(neckAx2, myNeckRadius * 0.99);
    Handle(Geom_CylindricalSurface) aCyl2 =
        new Geom_CylindricalSurface(neckAx2, myNeckRadius * 1.05);
    // 螺纹：定义二维曲线
    gp_Pnt2d aPnt(2. * M_PI, myNeckHeight * 0.5);
    gp_Dir2d aDir(2. * M_PI, myNeckHeight * 0.25);
    gp_Ax2d anAx2d(aPnt, aDir);
    double aMajor = 2. * M_PI;
    double aMinor = myNeckHeight / 10;

    Handle(Geom2d_Ellipse) anEllipse1 = new Geom2d_Ellipse(anAx2d, aMajor, aMinor);
    Handle(Geom2d_Ellipse) anEllipse2 = new Geom2d_Ellipse(anAx2d, aMajor, aMinor / 4);
    Handle(Geom2d_TrimmedCurve) anArc1 = new Geom2d_TrimmedCurve(anEllipse1, 0, M_PI);
    Handle(Geom2d_TrimmedCurve) anArc2 = new Geom2d_TrimmedCurve(anEllipse2, 0, M_PI);
    gp_Pnt2d anEllipsePnt1 = anEllipse1->Value(0);
    gp_Pnt2d anEllipsePnt2 = anEllipse1->Value(M_PI);
    Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(anEllipsePnt1, anEllipsePnt2);
    // 螺纹：创建边和线
    const TopoDS_Edge &anEdge1OnSurf1 = BRepBuilderAPI_MakeEdge(anArc1, aCyl1);
    const TopoDS_Edge &anEdge2OnSurf1 = BRepBuilderAPI_MakeEdge(aSegment, aCyl1);
    const TopoDS_Edge &anEdge1OnSurf2 = BRepBuilderAPI_MakeEdge(anArc2, aCyl2);
    const TopoDS_Edge &anEdge2OnSurf2 = BRepBuilderAPI_MakeEdge(aSegment, aCyl2);
    TopoDS_Wire threadingWire1 = BRepBuilderAPI_MakeWire(anEdge1OnSurf1, anEdge2OnSurf1);
    TopoDS_Wire threadingWire2 = BRepBuilderAPI_MakeWire(anEdge1OnSurf2, anEdge2OnSurf2);
    BRepLib::BuildCurves3d(threadingWire1);
    BRepLib::BuildCurves3d(threadingWire2);
    // 构建螺纹
    BRepOffsetAPI_ThruSections aTool(Standard_True);
    aTool.AddWire(threadingWire1);
    aTool.AddWire(threadingWire2);
    aTool.CheckCompatibility(Standard_False);
    TopoDS_Shape myThreading = aTool.Shape();
    // 整合模型
    TopoDS_Compound aRes;
    BRep_Builder aBuilder;
    aBuilder.MakeCompound(aRes);
    aBuilder.Add(aRes, myBody);

    aBuilder.Add(aRes, myThreading);

    return aRes;
};

TopoDS_Shape ShapeBuilder::makeCubePlusQuarterBall(double _dx, double _dy, double _dz)
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 0, 0.0));
    TopoDS_Shape t_topo_box = BRepPrimAPI_MakeBox(anAxis, _dx, _dy, _dz).Shape();
    TopoDS_Shape t_topo_Sphere = BRepPrimAPI_MakeSphere(anAxis, _dx, 3 * M_PI / 4).Shape();

    return BRepAlgoAPI_Fuse(t_topo_box, t_topo_Sphere);
}

TopoDS_Shape ShapeBuilder::makeBall(gp_Pnt loc_, double R)
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(loc_);
    return BRepPrimAPI_MakeSphere(anAxis, R).Shape();
}

dbPart *ShapeBuilder::createDumbBellVariant(const gp_Pnt &p1, const gp_Pnt &p2, double l_w_scale)
{
    double lengthof2Pt = p1.Distance(p2);
    TopoDS_Shape t_topo_Sphere = BRepPrimAPI_MakeSphere(p1, lengthof2Pt / 100).Shape();
    TopoDS_Shape t_topo_Sphere_pre = BRepPrimAPI_MakeSphere(p2, lengthof2Pt / 100).Shape();
    TopoDS_Shape DistLine = BRepPrimAPI_MakeCylinder(gp_Ax2(p1, gp_Dir(gp_Vec(p1, p2))),
                                                     lengthof2Pt / l_w_scale, lengthof2Pt);

    rfdb::dbPart *part = new rfdb::dbPart();
    rfdb::dbPart *sub_part1 = new rfdb::dbPart(DistLine);
    sub_part1->getProperty()->setColor(Quantity_NOC_RED);
    rfdb::dbPart *sub_part2 = new rfdb::dbPart(t_topo_Sphere);
    sub_part2->getProperty()->setColor(Quantity_NOC_ALICEBLUE);
    rfdb::dbPart *sub_part3 = new rfdb::dbPart(t_topo_Sphere_pre);
    sub_part3->getProperty()->setColor(Quantity_NOC_ALICEBLUE);

    // part->addChild({sub_part1, sub_part2, sub_part3});
    part->addChild(sub_part1);
    part->addChild(sub_part2);
    part->addChild(sub_part3);

    return part;
}

TopoDS_Shape ShapeBuilder::arrowBuilder(gp_Pnt p1, gp_Pnt p2, double arrow_scale)
{
    gp_Dir dir(gp_Vec(p2, p1));
    const TopoDS_Edge &branch = BRepBuilderAPI_MakeEdge(p1, p2);
    double dis = p1.Distance(p2);

    double arrow_length = dis * arrow_scale;
    double scale_h_r = 1.0 / 3.0;
    gp_Ax2 arr_loc(p2, dir);
    double arrow_r2 = arrow_length * scale_h_r;
    TopoDS_Shape arrow = BRepPrimAPI_MakeCone(arr_loc, 0.0, arrow_r2, arrow_length);

    TopoDS_Compound aRes;
    BRep_Builder aBuilder;
    aBuilder.MakeCompound(aRes);
    aBuilder.Add(aRes, branch);
    aBuilder.Add(aRes, arrow);
    return aRes;
}

dbPart *ShapeBuilder::buildAbassica(const gp_Ax2 &_loc, bool with_face, double _size)
{
    double scale_w_l = 0.005;
    double center_point_scale = 0.03;

    TopoDS_Shape DistLine_Z = BRepPrimAPI_MakeCylinder(gp_Ax2(_loc.Location(), _loc.Direction()),
                                                       _size * scale_w_l, _size);
    TopoDS_Shape DistLine_X = BRepPrimAPI_MakeCylinder(gp_Ax2(_loc.Location(), _loc.XDirection()),
                                                       _size * scale_w_l, _size);
    TopoDS_Shape DistLine_Y = BRepPrimAPI_MakeCylinder(gp_Ax2(_loc.Location(), _loc.YDirection()),
                                                       _size * scale_w_l, _size);
    TopoDS_Shape ctPT = BRepPrimAPI_MakeSphere(_loc.Location(), _size * center_point_scale);

    dbPart *part = new rfdb::dbPart();
    rfdb::dbPart *sub_part1 = new rfdb::dbPart(DistLine_Z);
    sub_part1->getProperty()->setColor(Quantity_NOC_BLUE);
    sub_part1->getProperty()->setName("Z");

    rfdb::dbPart *sub_part2 = new rfdb::dbPart(DistLine_X);
    sub_part2->getProperty()->setColor(Quantity_NOC_GREEN);
    sub_part2->getProperty()->setName("X");

    rfdb::dbPart *sub_part3 = new rfdb::dbPart(DistLine_Y);
    sub_part3->getProperty()->setColor(Quantity_NOC_RED);
    sub_part3->getProperty()->setName("Y");

    rfdb::dbPart *sub_part4 = new rfdb::dbPart(ctPT);
    sub_part4->getProperty()->setColor(Quantity_NOC_ALICEBLUE);
    sub_part4->getProperty()->setName("Origin");

    part->addChild(sub_part1);
    part->addChild(sub_part2);
    part->addChild(sub_part3);
    part->addChild(sub_part4);

    if (with_face)
    {
        createFace(part, _loc, _size);
    }
    return part;
}

gp_Pnt ShapeBuilder::plusPt(const gp_Pnt &p1, const gp_Pnt &p2)
{
    return gp_Pnt(p1.X() + p2.X(), p1.Y() + p2.Y(), p1.Z() + p2.Z());
}

void ShapeBuilder::createFace(dbPart *part, const gp_Ax2 &_loc, double _size)
{
    if (!part)
    {
        return;
    }

    const auto &loc = _loc.Location();
    const gp_Pnt &aPnt0 = plusPt(gp_Pnt(0, 0, 0), loc);
    const gp_Pnt &aPnt1 = plusPt(gp_Pnt(_size, 0, 0), loc);
    const gp_Pnt &aPnt2 = plusPt(gp_Pnt(0, _size, 0), loc);
    const gp_Pnt &aPnt3 = plusPt(gp_Pnt(0, 0, _size), loc);
    const gp_Pnt &aPnt4 = plusPt(gp_Pnt(_size, _size, 0), loc);
    const gp_Pnt &aPnt5 = plusPt(gp_Pnt(_size, 0, _size), loc);
    const gp_Pnt &aPnt6 = plusPt(gp_Pnt(0, _size, _size), loc);

    const TopoDS_Edge &anEdge1 = BRepBuilderAPI_MakeEdge(aPnt0, aPnt1);
    const TopoDS_Edge &anEdge2 = BRepBuilderAPI_MakeEdge(aPnt1, aPnt4);
    const TopoDS_Edge &anEdge3 = BRepBuilderAPI_MakeEdge(aPnt4, aPnt2);
    const TopoDS_Edge &anEdge4 = BRepBuilderAPI_MakeEdge(aPnt2, aPnt0);

    const TopoDS_Edge &anEdge5 = BRepBuilderAPI_MakeEdge(aPnt0, aPnt3);
    const TopoDS_Edge &anEdge6 = BRepBuilderAPI_MakeEdge(aPnt3, aPnt6);
    const TopoDS_Edge &anEdge7 = BRepBuilderAPI_MakeEdge(aPnt6, aPnt2);

    const TopoDS_Edge &anEdge8 = BRepBuilderAPI_MakeEdge(aPnt1, aPnt5);
    const TopoDS_Edge &anEdge9 = BRepBuilderAPI_MakeEdge(aPnt5, aPnt3);
    const TopoDS_Edge &anEdge10 = BRepBuilderAPI_MakeEdge(aPnt3, aPnt0);

    TopoDS_Wire aWire1 = BRepBuilderAPI_MakeWire(anEdge1, anEdge2, anEdge3, anEdge4);
    TopoDS_Face XOY_F = BRepBuilderAPI_MakeFace(aWire1, true);
    dbPart *XOY = new rfdb::dbPart();
    std::ignore = XOY->setShape(XOY_F);
    XOY->getProperty()->setColor(Quantity_NOC_ALICEBLUE);
    XOY->getProperty()->setName("XOY");
    part->addChild(XOY);

    TopoDS_Wire aWire2 = BRepBuilderAPI_MakeWire(anEdge4, anEdge5, anEdge6, anEdge7);
    TopoDS_Face YOZ_F = BRepBuilderAPI_MakeFace(aWire2, true);
    dbPart *YOZ = new rfdb::dbPart();
    std::ignore = YOZ->setShape(YOZ_F);
    YOZ->getProperty()->setColor(Quantity_NOC_ALICEBLUE);
    YOZ->getProperty()->setName("YOZ");
    part->addChild(YOZ);

    TopoDS_Wire aWire3 = BRepBuilderAPI_MakeWire(anEdge10, anEdge1, anEdge8, anEdge9);
    TopoDS_Face ZOX_F = BRepBuilderAPI_MakeFace(aWire3, true);
    dbPart *ZOX = new rfdb::dbPart();
    std::ignore = ZOX->setShape(ZOX_F);
    ZOX->getProperty()->setColor(Quantity_NOC_ALICEBLUE);
    ZOX->getProperty()->setName("ZOX");
    part->addChild(ZOX);
}
} // namespace rfdb
