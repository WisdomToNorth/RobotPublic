/****************************************************************************
** Copyright (c) 2022, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#include "reader_ply.h"

#include <cassert>
#include <iostream>
#include <string>

#include <BRep_TFace.hxx>
#include <Graphic3d_ArrayOfPoints.hxx>
#include <MeshVS_DataSource.hxx>
#include <MeshVS_Drawer.hxx>
#include <MeshVS_DrawerAttribute.hxx>
#include <MeshVS_Mesh.hxx>
#include <MeshVS_MeshPrsBuilder.hxx>
#include <MeshVS_NodalColorPrsBuilder.hxx>
#include <Poly_Triangulation.hxx>
#include <TDataStd_Name.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <miniply/miniply.h>

#include "rfbase/datautils.h"
#include "rfbase/rfmath.h"
#include "rfdb/dbPointCloud.h"

#include "brep_utils.h"
#include "graphics_mesh_data_source.h"
#include "mesh_utils.h"
#include "tkernel_utils.h"

namespace rfio
{
bool PlyReader::readFile(const std::string &filepath, rfapp::TaskProgress *)
{
    file_path_ = filepath;
    miniply::PLYReader reader(filepath.c_str());
    if (!reader.valid())
    {
        std::cout << "load filepath failed: " << filepath << '\n';
        return false;
    }

    // Reset internal data
    node_count_ = 0;
    node_coords_.clear();
    vec_color_component_.clear();
    vec_index_.clear();
    vec_normal_coord_.clear();
    bool assumeTriangles = true;

    // Guess if PLY faces are triangles
    uint32_t faceIdxs[3] = {};
    if (assumeTriangles)
    {
        miniply::PLYElement *faceElem =
            reader.get_element(reader.find_element(miniply::kPLYFaceElement));
        if (faceElem)
        {
            assumeTriangles = faceElem->convert_list_to_fixed_size(
                faceElem->find_property("vertex_indices"), 3, faceIdxs);
        }
    }

    bool okLoad = true;
    bool gotVerts = false;
    bool gotFaces = false;
    while (reader.has_element() && (!gotVerts || !gotFaces))
    {
        if (reader.element_is(miniply::kPLYVertexElement))
        {
            uint32_t prop3Idxs[3] = {};
            if (!reader.load_element() || !reader.find_pos(prop3Idxs))
            {
                okLoad = false;
                break;
            }

            node_count_ = reader.num_rows();
            node_coords_.resize(node_count_ * 3);
            reader.extract_properties(prop3Idxs, 3, miniply::PLYPropertyType::Float,
                                      node_coords_.data());
            if (reader.find_normal(prop3Idxs))
            {
                vec_normal_coord_.resize(node_count_ * 3);
                reader.extract_properties(prop3Idxs, 3, miniply::PLYPropertyType::Float,
                                          vec_normal_coord_.data());
            }

            if (reader.find_color(prop3Idxs))
            {
                vec_color_component_.resize(node_count_ * 3);
                reader.extract_properties(prop3Idxs, 3, miniply::PLYPropertyType::UChar,
                                          vec_color_component_.data());
            }

            gotVerts = true;
        }
        else if (!gotFaces && reader.element_is(miniply::kPLYFaceElement))
        {
            if (!reader.load_element())
            {
                break;
            }

            if (assumeTriangles)
            {
                vec_index_.resize(reader.num_rows() * 3);
                reader.extract_properties(faceIdxs, 3, miniply::PLYPropertyType::Int,
                                          vec_index_.data());
            }
            else
            {
                uint32_t propIdx = 0;
                if (!reader.find_indices(&propIdx))
                {
                    break;
                }

                const bool polys = reader.requires_triangulation(propIdx);
                if (polys && !gotVerts)
                {
                    break;
                }

                if (polys)
                {
                    vec_index_.resize(reader.num_triangles(propIdx) * 3);
                    reader.extract_triangles(propIdx, node_coords_.data(), node_count_,
                                             miniply::PLYPropertyType::Int, vec_index_.data());
                }
                else
                {
                    vec_index_.resize(reader.num_rows() * 3);
                    reader.extract_list_property(propIdx, miniply::PLYPropertyType::Int,
                                                 vec_index_.data());
                }
            }

            gotFaces = true;
        }
        else if (!gotFaces && reader.element_is("tristrips"))
        {
            if (!reader.load_element())
            {
                break;
            }

            uint32_t propIdx = reader.element()->find_property("vertex_indices");
            if (propIdx == miniply::kInvalidIndex)
            {
                break;
            }

            vec_index_.resize(reader.sum_of_list_counts(propIdx));
            reader.extract_list_property(propIdx, miniply::PLYPropertyType::Int, vec_index_.data());
            gotFaces = true;
        }

        reader.next_element();
    } // endwhile

    return okLoad;
}

rfdb::dbVariant PlyReader::transferMesh(rfapp::TaskProgress *)
{
    // Create target mesh
    const int tri_count = static_cast<int>(vec_index_.size() / 3);
    Handle_Poly_Triangulation mesh =
        new Poly_Triangulation(node_count_, tri_count, false /*hasUvNodes*/);

    if (!vec_normal_coord_.empty())
    {
        MeshUtils::allocateNormals(mesh);
    }

    // Copy nodes(vertices) into mesh
    for (int i = 0; rfbase::cmpLess(i, node_coords_.size()); i += 3)
    {
        const auto &vec = node_coords_;
        const gp_Pnt node = {vec.at(i), vec.at(i + 1), vec.at(i + 2)};
        MeshUtils::setNode(mesh, (i / 3) + 1, node);
    }

    // Copy triangles indices into mesh
    for (int i = 0; rfbase::cmpLess(i, vec_index_.size()); i += 3)
    {
        const auto &vec = vec_index_;
        const Poly_Triangle tri = {1 + vec.at(i), 1 + vec.at(i + 1), 1 + vec.at(i + 2)};
        MeshUtils::setTriangle(mesh, (i / 3) + 1, tri);
    }

    // Copy normals(optional) into mesh
    for (int i = 0; rfbase::cmpLess(i, vec_normal_coord_.size()); i += 3)
    {
        const auto &vec = vec_normal_coord_;
        const MeshUtils::Poly_Triangulation_NormalType n(vec.at(i), vec.at(i + 1), vec.at(i + 2));
        MeshUtils::setNormal(mesh, (i / 3) + 1, n);
    }

    // Copy colors(optional) into mesh
    [[maybe_unused]] std::vector<Quantity_Color> vecColor; // TODO: unused now
    for (int i = 0; rfbase::cmpLess(i, vec_color_component_.size()); i += 3)
    {
        const auto &vec = vec_color_component_;
        const Quantity_Color color = {vec.at(i) / 255., vec.at(i + 1) / 255., vec.at(i + 2) / 255.,
                                      ColorUtils::preferredRgbColorType()};
        vecColor.push_back(color);
    }
    // int i = vecColor.size();
    // std::cout << i <<'\n';
    TopoDS_Face face_shape = BRepUtils::makeFace(mesh);

    return createObject(face_shape, vecColor);
}

// TODO: remove this
struct TempDefaultValue
{
    bool showEdges = false;
    bool showNodes = false;
    Quantity_Color color = Quantity_NOC_GRAY;
    Graphic3d_NameOfMaterial material = Graphic3d_NOM_PLASTIC;
    Quantity_Color edgeColor = Quantity_NOC_BLACK;
};

TempDefaultValue defaultValues()
{
    return {};
}

rfdb::dbVariant PlyReader::createObject(const TopoDS_Shape &shape,
                                        const std::vector<Quantity_Color> &spanNodeColor)
{
    rfdb::dbVariant doc(new rfdb::dbMesh());

    if (shape.ShapeType() != TopAbs_FACE)
    {
        std::cout << "shape type is not face" << '\n';
        return doc;
    }

    Handle(BRep_TFace) tface = Handle_BRep_TFace::DownCast(shape.TShape());
    assert(!tface.IsNull());
    Handle(Poly_Triangulation) polyTri = tface->Triangulation();

    if (polyTri.IsNull())
    {
        std::cout << "polyTri is null" << '\n';
        return doc;
    }

    const Handle(MeshVS_Mesh) object = new MeshVS_Mesh();
    object->SetDataSource(new GraphicsMeshDataSource(polyTri));
    if (!spanNodeColor.empty())
    {
        const auto meshPrsBuilder = new MeshVS_NodalColorPrsBuilder(
            object, MeshVS_DMF_NodalColorDataPrs | MeshVS_DMF_OCCMask);
        for (int i = 0; rfbase::cmpLess(i, spanNodeColor.size()); ++i)
            meshPrsBuilder->SetColor(i + 1, spanNodeColor[i]);

        object->AddBuilder(meshPrsBuilder, true);
    }
    else
    {
        object->AddBuilder(new MeshVS_MeshPrsBuilder(object), true);
    }

    // -- MeshVS_DrawerAttribute
    object->GetDrawer()->SetBoolean(MeshVS_DA_ShowEdges, defaultValues().showEdges);
    object->GetDrawer()->SetBoolean(MeshVS_DA_DisplayNodes, defaultValues().showNodes);
    object->GetDrawer()->SetColor(MeshVS_DA_InteriorColor, defaultValues().color);
    object->GetDrawer()->SetMaterial(MeshVS_DA_FrontMaterial,
                                     Graphic3d_MaterialAspect(defaultValues().material));
    object->GetDrawer()->SetColor(MeshVS_DA_EdgeColor, defaultValues().edgeColor);
    object->GetDrawer()->SetBoolean(MeshVS_DA_ColorReflection, true);
    object->SetDisplayMode(MeshVS_DMF_Shading);

    // object->SetHilightMode(MeshVS_DMF_WireFrame);
    object->SetMeshSelMethod(MeshVS_MSM_PRECISE);

    rfdb::dbMesh *mesh_var = doc.toMesh();
    mesh_var->setMesh(object);
    mesh_var->setShape(shape);
    mesh_var->setMeshColors(spanNodeColor);
    return doc;
}

rfdb::dbVariant PlyReader::transferPointCloud(rfapp::TaskProgress *)
{
    const bool hasColors = !vec_color_component_.empty();
    const bool hasNormals = false; // TODO: handle normals
    auto gfxPoints =
        new Graphic3d_ArrayOfPoints(static_cast<int>(node_coords_.size()), hasColors, hasNormals);

    // Add nodes(vertices) into point cloud
    for (int i = 0; rfbase::cmpLess(i, node_coords_.size()); i += 3)
    {
        const auto &vec = node_coords_;
        const gp_Pnt node{vec.at(i), vec.at(i + 1), vec.at(i + 2)};
        gfxPoints->AddVertex(node);
    }

    if (hasColors)
    {
        for (int i = 0; rfbase::cmpLess(i, vec_color_component_.size()); i += 3)
        {
            const auto &vec = vec_color_component_;
            const Quantity_Color color{vec.at(i) / 255., vec.at(i + 1) / 255., vec.at(i + 2) / 255.,
                                       ColorUtils::preferredRgbColorType()};
            gfxPoints->SetVertexColor((i / 3) + 1, color);
        }
    }

    if (hasNormals)
    {
        for (int i = 0; rfbase::cmpLess(i, vec_normal_coord_.size()); i += 3)
        {
            const auto &vec = vec_normal_coord_;
            gfxPoints->SetVertexNormal((i / 3) + 1, vec.at(i), vec.at(i + 1), vec.at(i + 2));
        }
    }

    rfdb::dbPointCloud *pointcloud = new rfdb::dbPointCloud();
    pointcloud->setPoints(gfxPoints);
    return rfdb::dbVariant(pointcloud);
}

rfdb::dbVariant PlyReader::transfer(rfapp::TaskProgress *progress, bool /*concurrent*/)
{
    if (!node_coords_.empty() && !vec_index_.empty())
    {
        auto var = transferMesh(progress);
        var.getProperty()->setName(rfbase::DataUtils::getFileName(file_path_));
        return var;
    }

    if (!node_coords_.empty() && vec_index_.empty())
    {
        auto var = transferPointCloud(progress);
        var.getProperty()->setName(rfbase::DataUtils::getFileName(file_path_));
        return var;
    }

    return rfdb::dbVariant();
}

} // namespace rfio
