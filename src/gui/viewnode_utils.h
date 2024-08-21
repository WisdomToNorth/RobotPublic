#pragma once
#ifndef VIEW_NOTE_UTILS_H
#define VIEW_NOTE_UTILS_H

#include "rfdb/dbVariant.h"

#include "viewnode.h"

namespace rfgui
{
struct ViewNodeUtils
{
    static ViewNode *buildTree(const rfdb::dbVariant &db_variant);

    // db is not changed until this function is called
    // update inside:
    // 1. update dbVariant->property->init_trsf
    static bool updateViewNodeToDbVariant(ViewNode *root);

    static void setViewerIndex(ViewNode *root, int index);
    static void setUseType(ViewNode *root, UseType type);
    static void transform(ViewNode *root, const gp_Trsf &trsf);
};
} // namespace rfgui
#endif
