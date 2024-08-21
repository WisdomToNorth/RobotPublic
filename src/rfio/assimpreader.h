#pragma once
#ifndef ASSIMPFILE_H
#define ASSIMPFILE_H

#include <string>

#include <AIS_ColoredShape.hxx>
#include <assimp/scene.h>

#include "reader_base.h"

struct aiNode;
struct aiMaterial;

namespace rfio
{
class AssimpReader
{
public:
    /**
        @brief Saves an AIS_ColoredShape object to a file.
        @param _model The AIS_ColoredShape object to save.
        @param _filepath The path to the file where the object will be saved.
        @return true if the file was successfully saved, false otherwise.
    */
    static bool saveFiles(Handle(AIS_ColoredShape) _model, const std::string &_filepath);

    rfdb::dbVariant readFiles(const std::string &filepath);

private:
    static void processNodesIFC(const aiNode *_node, const aiScene *_scene,
                                Handle(AIS_ColoredShape) & _model);
    static aiNode *exportShapeToNode(aiScene *, Handle(AIS_ColoredShape) _select_shape);
};
} // namespace rfio
#endif
