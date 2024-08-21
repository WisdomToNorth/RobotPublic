#include "geoutils.h"

#include "stdafx.h"

#include <cmath>
#include <iomanip>
#include <vector>

#include <Eigen/Eigen>

namespace rfgeo
{
bool l_FuzzyEqual(double d1, double d2)
{
    return (std::abs(d1 - d2) * 1000000000000. <= (std::min)(std::abs(d1), std::abs(d2)));
}

gp_Pnt GeoUtils::scalePt(const gp_Pnt &p1, double scale)
{
    return gp_Pnt(p1.X() * scale, p1.Y() * scale, p1.Z() * scale);
}

gp_Pnt GeoUtils::minusPt(const gp_Pnt &p1, const gp_Pnt &p2)
{
    return gp_Pnt(p1.X() - p2.X(), p1.Y() - p2.Y(), p1.Z() - p2.Z());
}

gp_Pnt GeoUtils::plusPt(const gp_Pnt &p1, const gp_Pnt &p2)
{
    return gp_Pnt(p1.X() + p2.X(), p1.Y() + p2.Y(), p1.Z() + p2.Z());
}

bool GeoUtils::isEqualTrsf(const gp_Trsf &t1, const gp_Trsf &t2, double eps)
{
    NCollection_Mat4<float> theMatT1, theMatT2;
    t1.GetMat4(theMatT1);
    t2.GetMat4(theMatT2);

    for (int h = 0; h < 4; ++h)
    {
        for (int c = 0; c < 4; ++c)
        {
            auto v1 = theMatT1.GetValue(h, c);
            auto v2 = theMatT2.GetValue(h, c);
            if (abs(v1 - v2) > eps)
                return false;
        }
    }

    return true;
}

/**
 * Determines whether a ray intersects with a triangle.
 *
 * @param tri_points The three vertices of the triangle.
 * @param st The starting point of the ray.
 * @param ed The ending point of the ray.
 * @return Whether the ray intersects with the triangle.
 */
bool GeoUtils::rayInTriangle(const std::vector<gp_Pnt> &tri_points, const gp_Pnt &st,
                             const gp_Pnt &ed)
{
    const double EPSILON = 1e-6;

    const gp_Vec e1(tri_points[0], tri_points[1]);
    const gp_Vec e2(tri_points[0], tri_points[2]);

    // unit vector of rays
    gp_Vec D(st, ed);
    D.Normalize();

    // Plane normal vector
    const gp_Vec P = D.Crossed(e2);
    const double det = e1.Dot(P);

    // If the determinant is 0, then the ray is in the same plane as the triangle
    if (det > -EPSILON && det < EPSILON)
    {
        return false;
    }

    // Calculate the intersection of rays into triangles
    const double inv_det = 1.0 / det;
    const gp_Vec T(tri_points[0], st);
    const double U = T.Dot(P) * inv_det;

    // 如果 U 不在 [0, 1] 区间内，则交点在三角形外
    if (U < 0.0 || U > 1.0)
    {
        return false;
    }

    const gp_Vec Q = T.Crossed(e1);
    const double V = D.Dot(Q) * inv_det;

    // 如果 V 不在 [0, 1-U] 区间内，则交点在三角形外
    if (V < 0.0 || U + V > 1.0)
    {
        return false;
    }

    const double t = e2.Dot(Q) * inv_det;

    // If the distance from the intersection point to the start of the ray is less than EPSILON,
    // the intersection point is before the start of the ray.
    if (t > EPSILON)
    {
        // 计算交点
        const gp_Pnt crossP = plusPt(st, scalePt(gp_Pnt(D.XYZ()), t));

        // 判断交点是否在射线上
        const gp_Vec v1(crossP, st);
        const gp_Vec v2(crossP, ed);
        if (v1.Dot(v2) < 0)
        {
            return true;
        }
    }

    return false;
}

gp_Vec GeoUtils::getNormalByThreePnts(const std::vector<gp_Pnt> &ori_pts)
{
    assert(ori_pts.size() == 3);

    gp_Vec v1(ori_pts[0], ori_pts[1]);
    gp_Vec v2(ori_pts[1], ori_pts[2]);

    return v1.Crossed(v2).Normalized();
}

bool GeoUtils::isTwoTriCollision(const std::vector<gp_Pnt> &_tripoints1,
                                 const std::vector<gp_Pnt> &_tripoints2)
{
    std::vector<std::vector<gp_Pnt>> Lines1;
    Lines1.push_back(std::vector<gp_Pnt>{_tripoints1[0], _tripoints1[1]});
    Lines1.push_back(std::vector<gp_Pnt>{_tripoints1[0], _tripoints1[2]});
    Lines1.push_back(std::vector<gp_Pnt>{_tripoints1[1], _tripoints1[2]});

    for (size_t i = 0; i < Lines1.size(); i++)
    {
        if (rayInTriangle(_tripoints2, Lines1[i].front(), Lines1[i].back()))
        {
            return true;
        }
    }
    Lines1.clear();
    Lines1.push_back(std::vector<gp_Pnt>{_tripoints2[0], _tripoints2[1]});
    Lines1.push_back(std::vector<gp_Pnt>{_tripoints2[0], _tripoints2[2]});
    Lines1.push_back(std::vector<gp_Pnt>{_tripoints2[1], _tripoints2[2]});
    for (size_t i = 0; i < Lines1.size(); i++)
    {
        if (rayInTriangle(_tripoints1, Lines1[i].front(), Lines1[i].back()))
        {
            return true;
        }
    }

    return false;
}

void GeoUtils::triPntsTransformed(const std::vector<gp_Pnt> &input, const gp_Trsf &trsf,
                                  std::vector<gp_Pnt> &_output)
{
    std::vector<gp_Pnt> output;
    for (size_t i = 0; i < input.size(); i++)
    {
        output.push_back(input[i].Transformed(trsf));
    }
    output.swap(_output);
}

double GeoUtils::caculateTriangleArea(const std::vector<gp_Pnt> &tri_points)
{
    const double a = tri_points[0].Distance(tri_points[1]);
    const double b = tri_points[1].Distance(tri_points[2]);
    const double c = tri_points[2].Distance(tri_points[0]);
    const double p = (a + b + c) / 2;

    const double s = sqrt(p * (p - a) * (p - b) * (p - c));
    return s;
}

gp_Pnt GeoUtils::pointProjLine(const gp_Pnt &pt, const gp_Lin &lin)
{
    gp_Vec V1(lin.Direction());
    V1.Normalize();

    const gp_Pnt &OR = lin.Location();
    const gp_Vec V(OR, pt);
    const double dist = V1.Dot(V);
    return OR.Translated(dist * V1);
}

gp_Vec GeoUtils::eulerToVec(const gp_Pnt &euler, const gp_Pnt &loc)
{
    gp_Trsf trsfx, trsfy, trsfz, trsf;
    trsf.SetTranslation(gp_Pnt(0, 0, 0), loc);
    trsfx.SetRotation(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(1, 0, 0)), euler.X());
    trsfy.SetRotation(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(0, 1, 0)), euler.Y());
    trsfz.SetRotation(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)), euler.Z());

    gp_Vec vec(gp_Pnt(0, 0, 0), gp_Pnt(0, 0, 1));
    vec.Transform(trsf * trsfx * trsfy * trsfz);
    return vec;
}

gp_Ax2 GeoUtils::eulerToAx2(const gp_Pnt &euler, const gp_Pnt &loc)
{
    gp_Trsf trsfx, trsfy, trsfz, trsf;
    trsf.SetTranslation(gp_Pnt(0, 0, 0), loc);
    trsfx.SetRotation(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(1, 0, 0)), euler.X());
    trsfy.SetRotation(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(0, 1, 0)), euler.Y());
    trsfz.SetRotation(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)), euler.Z());

    gp_Ax2 l_ax2(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1), gp_Dir(1, 0, 0));
    l_ax2.Transform(trsf * trsfx * trsfy * trsfz);
    return l_ax2;
}

gp_Trsf GeoUtils::cpuRotZDirection(const gp_Ax2 &ax2_left, const gp_Ax2 &ax2_right) // 从左到右变换
{
    gp_Trsf rotZ;

    const gp_Dir &l1 = ax2_left.Direction();
    const gp_Dir &l2 = ax2_right.Direction();

    const auto l_angle = l1.Angle(l2);

    if (!l_FuzzyEqual(l_angle, 0.0))
    {
        gp_Dir laser_normal;
        if (l_FuzzyEqual(l_angle, M_PI)) // 两坐标系的Z轴平行反向，随便取一个方向作为旋转轴
        {
            laser_normal = ax2_right.XDirection();
        }
        else
            laser_normal = l1.Crossed(l2); // 法向量

        const gp_Ax1 l_ax(ax2_left.Location(), laser_normal);
        rotZ.SetRotation(l_ax, l_angle);
    }
    return rotZ;
}

gp_Trsf GeoUtils::cpuRot(const gp_Ax2 &ax2_left, const gp_Ax2 &ax2_right) // 从左到右变换
{
    const gp_Trsf &rotZ = cpuRotZDirection(ax2_left, ax2_right);
    const gp_Ax2 &ax2_temp = ax2_left.Transformed(rotZ); // 已经对齐Z轴的坐标系
    const gp_Trsf &rotX = cpuRotZDirection(ax2_temp, ax2_right);

    return rotX * rotZ;
}

gp_Trsf GeoUtils::cpuTrsf(const gp_Ax2 &ax2_left, const gp_Ax2 &ax2_right)
{
    gp_Trsf trsf_trans;
    trsf_trans.SetTranslation(ax2_left.Location(), ax2_right.Location());
    gp_Trsf trsf_rot = cpuRot(ax2_left, ax2_right);
    return trsf_trans * trsf_rot;
}

gp_Pnt GeoUtils::cpuEuler(const gp_Ax2 &ax2_0, const gp_Ax2 &ax2_1)
{
    const gp_Trsf &trsf = cpuRot(ax2_0, ax2_1);
    const gp_Quaternion &gq = trsf.GetRotation();
    double x, y, z;
    gq.GetEulerAngles(gp_EulerSequence::gp_Intrinsic_XYZ, x, y, z);
    return {x, y, z};
}

void GeoUtils::discreteEdge(const TopoDS_Edge &edge, double distance,
                            std::vector<std::pair<gp_Pnt, gp_Vec>> &res)
{
    double st_para, ed_para;
    Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, st_para, ed_para);

    GeomAdaptor_Curve GAC(curve);

    if (GAC.GetType() == GeomAbs_Line) // 如果边为直线
    {
        const gp_Pnt &p1 = curve->Value(st_para);
        const gp_Pnt &p2 = curve->Value(ed_para);
        gp_Vec vec = gp_Vec(p1, p2).Normalized();

        gp_Pnt pt = p1;
        while (pt.Distance(p1) < p1.Distance(p2))
        {
            res.emplace_back(std::make_pair(pt, vec));
            pt = pt.Translated(vec * distance);
        }
        if (res.back().first.Distance(p2) > Precision::Confusion())
        {
            res.emplace_back(std::make_pair(p2, vec));
        }
    }
    else // 边为曲线
    {
        const GCPnts_UniformAbscissa UA(GAC, distance);
        gp_Pnt pt;
        gp_Vec v;
        for (Standard_Integer index = 1; index <= UA.NbPoints(); ++index)
        {
            const double parami = UA.Parameter(index);
            if (parami < st_para || parami > ed_para)
            {
                std::cout << "Filter the point\n";
                continue;
            }
            curve->D1(parami, pt, v);
            res.emplace_back(std::make_pair(pt, v));
        }

        curve->D1(ed_para, pt, v);
        if (res.back().first.Distance(pt) > Precision::Confusion())
            res.emplace_back(std::make_pair(pt, v));
    }
}

gp_Pnt GeoUtils::getCenterOfCircle(const std::vector<gp_Pnt> &points)
{
    assert(points.size() == 3);

    const double x1 = points[0].X(), x2 = points[1].X(), x3 = points[2].X();
    const double y1 = points[0].Y(), y2 = points[1].Y(), y3 = points[2].Y();
    const double z1 = points[0].Z(), z2 = points[1].Z(), z3 = points[2].Z();

    const double a1 = (y1 * z2 - y2 * z1 - y1 * z3 + y3 * z1 + y2 * z3 - y3 * z2);
    const double b1 = -(x1 * z2 - x2 * z1 - x1 * z3 + x3 * z1 + x2 * z3 - x3 * z2);
    const double c1 = (x1 * y2 - x2 * y1 - x1 * y3 + x3 * y1 + x2 * y3 - x3 * y2);
    const double d1 =
        -(x1 * y2 * z3 - x1 * y3 * z2 - x2 * y1 * z3 + x2 * y3 * z1 + x3 * y1 * z2 - x3 * y2 * z1);

    const double a2 = 2 * (x2 - x1);
    const double b2 = 2 * (y2 - y1);
    const double c2 = 2 * (z2 - z1);
    const double d2 = x1 * x1 + y1 * y1 + z1 * z1 - x2 * x2 - y2 * y2 - z2 * z2;

    const double a3 = 2 * (x3 - x1);
    const double b3 = 2 * (y3 - y1);
    const double c3 = 2 * (z3 - z1);
    const double d3 = x1 * x1 + y1 * y1 + z1 * z1 - x3 * x3 - y3 * y3 - z3 * z3;

    const double x =
        -(b1 * c2 * d3 - b1 * c3 * d2 - b2 * c1 * d3 + b2 * c3 * d1 + b3 * c1 * d2 - b3 * c2 * d1) /
        (a1 * b2 * c3 - a1 * b3 * c2 - a2 * b1 * c3 + a2 * b3 * c1 + a3 * b1 * c2 - a3 * b2 * c1);
    const double y =
        (a1 * c2 * d3 - a1 * c3 * d2 - a2 * c1 * d3 + a2 * c3 * d1 + a3 * c1 * d2 - a3 * c2 * d1) /
        (a1 * b2 * c3 - a1 * b3 * c2 - a2 * b1 * c3 + a2 * b3 * c1 + a3 * b1 * c2 - a3 * b2 * c1);
    const double z =
        -(a1 * b2 * d3 - a1 * b3 * d2 - a2 * b1 * d3 + a2 * b3 * d1 + a3 * b1 * d2 - a3 * b2 * d1) /
        (a1 * b2 * c3 - a1 * b3 * c2 - a2 * b1 * c3 + a2 * b3 * c1 + a3 * b1 * c2 - a3 * b2 * c1);

    return {x, y, z};
}

gp_Lin GeoUtils::twoPlanesIntersection(const gp_Pln &plane1, const gp_Pln &plane2)
{
    auto crossFn = IntAna_QuadQuadGeo();
    crossFn.Perform(plane1, plane2, 1e-7, 1e-7);
    const gp_Lin &line = crossFn.Line(1);
    return line;
}

gp_Pnt GeoUtils::newPointOnDir(const gp_Pnt &begin_point, const gp_Dir &dir, double length)
{
    gp_Pnt p(dir.XYZ().Normalized());
    return plusPt(begin_point, scalePt(p, length));
}

gp_Pnt GeoUtils::getCenterOfPnts(const std::vector<gp_Pnt> &ori_pts)
{
    gp_Pnt center_pnt;
    double x = 0.0, y = 0.0, z = 0.0;
    for (const auto &pnt : ori_pts)
    {
        x = x + pnt.X();
        y = y + pnt.Y();
        z = z + pnt.Z();
    }
    const double cnt = static_cast<double>(ori_pts.size());
    center_pnt.SetX(x / cnt);
    center_pnt.SetY(y / cnt);
    center_pnt.SetZ(z / cnt);
    return center_pnt;
}

double GeoUtils::planePosition(const gp_Pln &plane)
{
    const gp_Vec vecLoc(plane.Location().XYZ());
    const gp_Vec vecNormal(plane.Axis().Direction());
    return vecLoc.Dot(vecNormal);
}

Bnd_OBB GeoUtils::getPntsObb(const std::vector<gp_Pnt> &_pnts)
{
    Eigen::MatrixXd X(_pnts.size(), 3);
    for (size_t i = 0; i < _pnts.size(); i++)
    {
        X(i, 0) = _pnts[i].X();
        X(i, 1) = _pnts[i].Y();
        X(i, 2) = _pnts[i].Z();
    }

    // 计算协方差矩阵
    Eigen::MatrixXd X2 = X; // 原数据不需要归一化
    // 计算每一维度均值
    Eigen::RowVectorXd meanval = X.colwise().mean();
    const Eigen::RowVectorXd &meanvecRow = meanval;
    // 样本均值化为0
    X2.rowwise() -= meanvecRow;
    // 计算协方差矩阵C = XTX / n-1;
    Eigen::Matrix3d C2 = X2.adjoint() * X2;
    C2 = C2.array() / (X2.rows() - 1);

    // 获得特征值和特征向量
    Eigen::EigenSolver<Eigen::Matrix3d> es(C2);
    [[maybe_unused]] Eigen::Vector3d D = es.pseudoEigenvalueMatrix().diagonal(); // 特征值
    Eigen::Matrix3d V = es.pseudoEigenvectors();                                 // 特征向量

    Eigen::Vector3d v0 = V.col(0), v1 = V.col(1), v2 = V.col(2); // 特征向量正交化
    v0.normalize();
    v1 -= v1.dot(v0) * v0;
    v1.normalize();
    v2 = v0.cross(v1);

    for (int i = 0; i < 3; i++)
    {
        V(i, 0) = v0[i];
        V(i, 1) = v1[i];
        V(i, 2) = v2[i];
    }

    // 原始数据映射到主成份特征向量轴上
    X = X * V;
    Eigen::Matrix3d vec = V;

    // 计算x, y, z的最大差，和中心点
    double minx = DBL_MAX, miny = DBL_MAX, minz = DBL_MAX;
    double maxx = DBL_MIN, maxy = DBL_MIN, maxz = DBL_MIN;

    for (int i = 0; i < X.rows(); i++)
    {
        double temp = X(i, 0);
        if (temp < minx)
            minx = temp;
        if (temp > maxx)
            maxx = temp;

        temp = X(i, 1);
        if (temp < miny)
            miny = temp;
        if (temp > maxy)
            maxy = temp;

        temp = X(i, 2);
        if (temp < minz)
            minz = temp;
        if (temp > maxz)
            maxz = temp;
    }

    // 半轴长度
    double xlength = (maxx - minx) / 2;
    double ylength = (maxy - miny) / 2;
    double zlength = (maxz - minz) / 2;

    // 轴方向
    gp_Vec vecx(vec(0, 0), vec(1, 0), vec(2, 0));
    gp_Vec vecy(vec(0, 1), vec(1, 1), vec(2, 1));
    gp_Vec vecz(vec(0, 2), vec(1, 2), vec(2, 2));

    gp_Dir xDir = vecx.Normalized();
    gp_Dir yDir = vecy.Normalized();
    gp_Dir zDir = vecz.Normalized();

    // 映射空间中的中心点
    gp_Pnt center((maxx + minx) / 2, (maxy + miny) / 2, (maxz + minz) / 2);

    Eigen::Matrix3d center2 = Eigen::Matrix3d::Random(1, 3);
    center2(0, 0) = center.X();
    center2(0, 1) = center.Y();
    center2(0, 2) = center.Z();
    // 反映射到原空间中的中心点
    center2 = center2 * V.inverse();
    gp_Pnt center3(center2(0, 0), center2(0, 1), center2(0, 2));
    Bnd_OBB bndObb(center3, xDir, yDir, zDir, xlength, ylength, zlength);

    return bndObb;
}
} // namespace rfgeo
