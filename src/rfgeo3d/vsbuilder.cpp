#include "vsbuilder.h"

#include "stdafx.h"

#include <vector>

#include "shapeutils.h"

namespace rfgeo
{

Handle(AIS_ColoredShape) VMBuilder::createDumbBellViewShape(const gp_Pnt &p1, const gp_Pnt &p2,
                                                            double l_w_scale)
{
    const double lengthof2Pt = p1.Distance(p2);
    TopoDS_Shape t_topo_Sphere = BRepPrimAPI_MakeSphere(p1, lengthof2Pt / 100).Shape();
    TopoDS_Shape t_topo_Sphere_pre = BRepPrimAPI_MakeSphere(p2, lengthof2Pt / 100).Shape();
    TopoDS_Shape DistLine = BRepPrimAPI_MakeCylinder(gp_Ax2(p1, gp_Dir(gp_Vec(p1, p2))),
                                                     lengthof2Pt / l_w_scale, lengthof2Pt);

    const auto &comp = ShapeUtils::makeCompound({t_topo_Sphere, t_topo_Sphere_pre, DistLine});

    Handle(AIS_ColoredShape) ais_shape = new AIS_ColoredShape(comp);
    ais_shape->SetCustomColor(t_topo_Sphere, Quantity_NOC_ALICEBLUE);
    ais_shape->SetCustomColor(t_topo_Sphere_pre, Quantity_NOC_ALICEBLUE);
    ais_shape->SetCustomColor(DistLine, Quantity_NOC_RED);
    return ais_shape;
}
} // namespace rfgeo
