#include "assimpreader.h"

#include "stdafx.h"

#include <iostream>

#include <TopoDS_Shape.hxx>
#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>
#include <assimp/ai_assert.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/vector3.h>

#include "rfdb/dbPart.h"
#include "rfgeo3d/geodataio.h"

using Assimp::Importer;
using Assimp::Exporter;

namespace rfio
{

void aiMatrix4x4_To_gpTrsf(const aiMatrix4x4 &_matrix, gp_Trsf &_transfer)
{
    ai_real l_scaling_x = aiVector3D(_matrix.a1, _matrix.a2, _matrix.a3).Length();
    ai_real l_scaling_y = aiVector3D(_matrix.b1, _matrix.b2, _matrix.b3).Length();
    ai_real l_scaling_z = aiVector3D(_matrix.c1, _matrix.c2, _matrix.c3).Length();

    _transfer.SetValues(
        _matrix.a1 / l_scaling_x, _matrix.a2 / l_scaling_x, _matrix.a3 / l_scaling_x, _matrix.a4,
        _matrix.b1 / l_scaling_y, _matrix.b2 / l_scaling_y, _matrix.b3 / l_scaling_y, _matrix.b4,
        _matrix.c1 / l_scaling_z, _matrix.c2 / l_scaling_z, _matrix.c3 / l_scaling_z, _matrix.c4);
}

void gpTrsf_To_aiMatrix4x4(const gp_Trsf &_transfer, aiMatrix4x4 &_matrix)
{
    NCollection_Mat4<float> theMat;
    _transfer.GetMat4(theMat);

    _matrix.a1 = theMat.GetValue(0, 0);
    _matrix.a2 = theMat.GetValue(0, 1);
    _matrix.a3 = theMat.GetValue(0, 2);
    _matrix.a4 = theMat.GetValue(0, 3);

    _matrix.b1 = theMat.GetValue(1, 0);
    _matrix.b2 = theMat.GetValue(1, 1);
    _matrix.b3 = theMat.GetValue(1, 2);
    _matrix.b4 = theMat.GetValue(1, 3);

    _matrix.c1 = theMat.GetValue(2, 0);
    _matrix.c2 = theMat.GetValue(2, 1);
    _matrix.c3 = theMat.GetValue(2, 2);
    _matrix.c4 = theMat.GetValue(2, 3);

    _matrix.d1 = theMat.GetValue(3, 0);
    _matrix.d2 = theMat.GetValue(3, 1);
    _matrix.d3 = theMat.GetValue(3, 2);
    _matrix.d4 = theMat.GetValue(3, 3);
}

gp_Pnt aiVector3D_To_gpPnt(const aiVector3D &lhs)
{
    return gp_Pnt(lhs.x, lhs.y, lhs.z);
}

Graphic3d_MaterialAspect materialTransfer(aiMaterial *_material)
{
    //! Convert model materials from ASSIMP to OCC representation
    // Declare and initialize OCC material parameters
    Graphic3d_MaterialAspect t_result;
    t_result.SetMaterialType(Graphic3d_MATERIAL_PHYSIC);
    Quantity_Color t_occ_colors[Graphic3d_TypeOfReflection_NB];
    t_occ_colors[Graphic3d_TOR_AMBIENT] = Quantity_Color(Graphic3d_Vec3(0.2f, 0.2f, 0.2f));
    t_occ_colors[Graphic3d_TOR_DIFFUSE] = Quantity_Color(Graphic3d_Vec3(0.2f, 0.2f, 0.2f));
    t_occ_colors[Graphic3d_TOR_SPECULAR] = Quantity_Color(Graphic3d_Vec3(1.0f, 1.0f, 1.0f));
    float t_occ_shininess = 0.039f;

    aiString name; // Material name, raw data
    if (AI_SUCCESS == aiGetMaterialString(_material, AI_MATKEY_NAME, &name))
    {
        t_result.SetMaterialName(name.C_Str());
    }
    // Ambient light
    aiColor4D ambient; // Ambient light, raw data
    if (AI_SUCCESS == aiGetMaterialColor(_material, AI_MATKEY_COLOR_AMBIENT, &ambient))
    {
        t_occ_colors[Graphic3d_TOR_AMBIENT] =
            Quantity_Color(ambient.r, ambient.g, ambient.b, Quantity_TOC_RGB);
        t_result.SetAmbientColor(t_occ_colors[Graphic3d_TOR_AMBIENT]);
    }
    // Diffuse reflection
    aiColor4D diffuse; // Diffuse reflection, raw data
    if (AI_SUCCESS == aiGetMaterialColor(_material, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
    {
        t_occ_colors[Graphic3d_TOR_DIFFUSE] =
            Quantity_Color(diffuse.r, diffuse.g, diffuse.b, Quantity_TOC_RGB);
        t_result.SetDiffuseColor(t_occ_colors[Graphic3d_TOR_DIFFUSE]);
    }
    // Specular light
    aiColor4D specular; // Specular light, raw data
    if (AI_SUCCESS == aiGetMaterialColor(_material, AI_MATKEY_COLOR_SPECULAR, &specular))
    {
        t_occ_colors[Graphic3d_TOR_SPECULAR] =
            Quantity_Color(specular.r, specular.g, specular.b, Quantity_TOC_RGB);
        t_result.SetSpecularColor(t_occ_colors[Graphic3d_TOR_SPECULAR]);
    }
    // Emissive light
    aiColor4D emissive; // Emissive light, raw data
    if (AI_SUCCESS == aiGetMaterialColor(_material, AI_MATKEY_COLOR_EMISSIVE, &emissive))
    {
        t_occ_colors[Graphic3d_TOR_EMISSION] =
            Quantity_Color(emissive.r, emissive.g, specular.b, Quantity_TOC_RGB);
        t_result.SetEmissiveColor(t_occ_colors[Graphic3d_TOR_EMISSION]);
    }

    float shininess; // Shininess, raw data
    if (AI_SUCCESS == aiGetMaterialFloat(_material, AI_MATKEY_SHININESS, &shininess))
    {
        t_occ_shininess = shininess / 128.0f; // Convert from OpenGL value to VRML97
        // OCC's shininess representation only accepts values between 0 and 1, otherwise errors
        // occur
        t_occ_shininess = t_occ_shininess < 1.0f ? t_occ_shininess : 1.0f;
        t_occ_shininess = t_occ_shininess < 0.0f ? 0.0f : t_occ_shininess;
        t_result.SetShininess(t_occ_shininess); // Set shininess
    }
    return t_result;
}

void aiNodeToTopoShape(const aiNode *_node, const aiScene *_scene, TopoDS_Shape &shape,
                       Graphic3d_MaterialAspect &material)
{
    if (!_node)
    {
        return;
    }
    TopoDS_Compound l_compound;
    BRep_Builder l_build_tool;
    l_build_tool.MakeCompound(l_compound);

    // Get all mesh
    for (unsigned int i = 0; i < _node->mNumMeshes; i++)
    {
        aiMesh *l_mesh = _scene->mMeshes[_node->mMeshes[i]];

        // Get all faces
        for (unsigned int iface = 0; iface < l_mesh->mNumFaces; iface++)
        {
            const aiFace &l_face = l_mesh->mFaces[iface];

            // Get all vertices
            BRepBuilderAPI_MakePolygon l_polygon;
            for (unsigned int ivertex = 0; ivertex < l_face.mNumIndices; ivertex++)
            {
                gp_Pnt l_pnt = aiVector3D_To_gpPnt(l_mesh->mVertices[l_face.mIndices[ivertex]]);
                l_polygon.Add(l_pnt);
            }
            l_polygon.Close();

            TopoDS_Face l_topo_face = BRepBuilderAPI_MakeFace(l_polygon);
            if (!l_topo_face.IsNull())
            {
                l_build_tool.Add(l_compound, l_topo_face);
            }
        }

        // Get material
        aiMaterial *l_material = _scene->mMaterials[l_mesh->mMaterialIndex];
        material = materialTransfer(l_material);
    }

    BRepBuilderAPI_Sewing t_sewing_tool;
    t_sewing_tool.Init(1.0e-06, true);
    t_sewing_tool.Load(l_compound);
    t_sewing_tool.Perform();
    TopoDS_Shape l_topo_shape = t_sewing_tool.SewedShape();

    if (l_topo_shape.IsNull())
    {
        shape = l_compound;
    }
    else
    {
        shape = l_topo_shape;
    }
    return;
}

rfdb::dbVariant AssimpReader::readFiles(const std::string &_filepath)
{
    Assimp::Importer t_importer;
    const aiScene *t_scene = t_importer.ReadFile(
        _filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!t_scene || t_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !t_scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP:: " << t_importer.GetErrorString();
        return rfdb::dbVariant();
    }

    Handle(AIS_ColoredShape) _model = new AIS_ColoredShape(TopoDS_Shape());
    processNodesIFC(t_scene->mRootNode, t_scene, _model);

    rfdb::dbPart *file = new rfdb::dbPart();
    std::ignore = file->setShape(_model);
    return rfdb::dbVariant(file);
};

void AssimpReader::processNodesIFC(const aiNode *_node, const aiScene *_scene,
                                   Handle(AIS_ColoredShape) & model_)
{
    std::string t_node_name = _node->mName.C_Str();

    gp_Trsf l_transfer;
    aiMatrix4x4_To_gpTrsf(_node->mTransformation, l_transfer);

    TopoDS_Shape l_topo_shape;
    Graphic3d_MaterialAspect l_material;
    aiNodeToTopoShape(_node, _scene, l_topo_shape, l_material);

    model_->SetShape(l_topo_shape);
    model_->SetLocalTransformation(l_transfer);
    model_->SetMaterial(l_material);

    // 递归处理子节点
    for (unsigned int i = 0; i < _node->mNumChildren; i++)
    {
        Handle(AIS_ColoredShape) sub_model = new AIS_ColoredShape(TopoDS_Shape());
        processNodesIFC(_node->mChildren[i], _scene, sub_model);
        model_->AddChild(sub_model);
    }
}

bool AssimpReader::saveFiles(Handle(AIS_ColoredShape) _model, const std::string &_filepath)
{
    auto l_scene = new aiScene();
    std::ignore = exportShapeToNode(l_scene, _model);
    Exporter exporter;
    exporter.Export(l_scene, "obj", _filepath);
    return false;
}

aiMaterial occMaterialToAMaterial(const Graphic3d_MaterialAspect &shpae_material)
{
    aiMaterial pMaterial = aiMaterial();

    Quantity_Color amb = shpae_material.AmbientColor();
    aiColor4D ambient((ai_real)amb.Red(), (ai_real)amb.Green(), (ai_real)amb.Blue(), 1.0);
    pMaterial.AddProperty(&ambient, 1, AI_MATKEY_COLOR_AMBIENT);

    Quantity_Color diff = shpae_material.DiffuseColor();
    aiColor4D diffuse((ai_real)diff.Red(), (ai_real)diff.Green(), (ai_real)diff.Blue(), 1.0);
    pMaterial.AddProperty(&diffuse, 1, AI_MATKEY_COLOR_DIFFUSE);

    Quantity_Color spec = shpae_material.SpecularColor();
    aiColor4D specular((ai_real)spec.Red(), (ai_real)spec.Green(), (ai_real)spec.Blue(), 1.0);
    pMaterial.AddProperty(&specular, 1, AI_MATKEY_COLOR_SPECULAR);

    Standard_ShortReal shin = shpae_material.Shininess();
    pMaterial.AddProperty(&shin, 1, AI_MATKEY_SHININESS);
    return pMaterial;
}

aiNode *AssimpReader::exportShapeToNode(aiScene *t_scene, Handle(AIS_ColoredShape) model_)
{
    aiNode *currentNode = new aiNode();
    if (model_->Children().Size() > 0)
    {
        gpTrsf_To_aiMatrix4x4(model_->Transformation(), currentNode->mTransformation);

        int numNode = model_->Children().Size();
        aiNode **t_node_list = new aiNode *[numNode];
        int idx = 0;
        for (PrsMgr_ListOfPresentableObjectsIter i(model_->Children()); i.More(); i.Next())
        {
            Handle(PrsMgr_PresentableObject) t_object = i.Value();

            if (t_object->IsKind(STANDARD_TYPE(AIS_ColoredShape)))
            {
                Handle(AIS_ColoredShape) child_model = Handle(AIS_ColoredShape)::DownCast(t_object);
                if (idx < numNode)
                {
                    t_node_list[idx] = AssimpReader::exportShapeToNode(t_scene, model_);
                }
                ++idx;
            }
            else
            {
                std::cerr << "out of range in CModel::exportShapeToNode(aiScene* t_scene)\n";
            }
        }

        currentNode->addChildren(idx, t_node_list);
    }
    else if (!model_->Shape().IsNull())
    {
        currentNode->mNumMeshes = 1;                   // 一个网格
        currentNode->mNumChildren = 0;                 // 无子节点
        currentNode->mMeshes = new unsigned int[1];    // 一个网格地址
        currentNode->mMeshes[0] = t_scene->mNumMeshes; // 网格地址索引
        gpTrsf_To_aiMatrix4x4(model_->Transformation(), currentNode->mTransformation);

        aiMesh *pMesh = t_scene->mMeshes[currentNode->mMeshes[0]] = new aiMesh();
        pMesh->mMaterialIndex = t_scene->mNumMaterials;
        pMesh->mNumVertices = 0;
        pMesh->mNumFaces = 0;

        t_scene->mMaterials[pMesh->mMaterialIndex] =
            new aiMaterial(occMaterialToAMaterial(model_->Material()));
        t_scene->mNumMeshes++;
        t_scene->mNumMaterials++;

        // calculate total number of the nodes and triangles
        for (TopExp_Explorer anExpSF(model_->Shape(), TopAbs_FACE); anExpSF.More(); anExpSF.Next())
        {
            TopLoc_Location aLoc;
            Handle(Poly_Triangulation) aTriangulation =
                BRep_Tool::Triangulation(TopoDS::Face(anExpSF.Current()), aLoc);
            if (!aTriangulation.IsNull())
            {
                pMesh->mNumVertices += aTriangulation->NbNodes();
                pMesh->mNumFaces += aTriangulation->NbTriangles();
            }
        }

        aiVector3D *vp, *vn;
        vp = pMesh->mVertices = new aiVector3D[pMesh->mNumVertices];
        vn = pMesh->mNormals = new aiVector3D[pMesh->mNumVertices];
        pMesh->mFaces = new aiFace[pMesh->mNumFaces];

        unsigned int index = 0;
        unsigned int face_index = 0;
        // fill temporary triangulation
        Standard_Integer aNodeOffset = 0;
        for (TopExp_Explorer anExpSF(model_->Shape(), TopAbs_FACE); anExpSF.More(); anExpSF.Next())
        {
            TopLoc_Location aLoc;
            Handle(Poly_Triangulation) aTriangulation =
                BRep_Tool::Triangulation(TopoDS::Face(anExpSF.Current()), aLoc);
            const Poly_ArrayOfNodes &aNodes = aTriangulation->InternalNodes();
            gp_Trsf l_trsf = aLoc.Transformation();

            for (auto aNodeIter = aNodes.Lower(); aNodeIter <= aNodes.Upper(); ++aNodeIter)
            {
                gp_Pnt l_pnt = aNodes[aNodeIter];

                l_pnt.Transform(l_trsf);
                vp[index].Set((ai_real)l_pnt.X(), (ai_real)l_pnt.Y(), (ai_real)l_pnt.Z());
                vn[index].Set(0.0, 0.0, 1.0);
                index++;
            }

            for (auto a = 0; a <= aTriangulation->NbNodes(); ++a)
            {
                Poly_Triangle aTri = aTriangulation->Triangle(a);

                Standard_Integer anId[3];
                aTri.Get(anId[0], anId[1], anId[2]);
                if (anExpSF.Current().Orientation() == TopAbs_REVERSED)
                {
                    Standard_Integer aTmpIdx = anId[1];
                    anId[1] = anId[2];
                    anId[2] = aTmpIdx;
                }
                // Update nodes according to the offset.
                anId[0] += aNodeOffset;
                anId[1] += aNodeOffset;
                anId[2] += aNodeOffset;

                aiFace &face = pMesh->mFaces[face_index++];
                face.mIndices = new unsigned int[face.mNumIndices = 3];
                face.mIndices[0] = anId[0] - 1;
                face.mIndices[1] = anId[1] - 1;
                face.mIndices[2] = anId[2] - 1;
            }

            aNodeOffset += aNodes.Size();
        }
    }

    return currentNode;
}

} // namespace rfio
