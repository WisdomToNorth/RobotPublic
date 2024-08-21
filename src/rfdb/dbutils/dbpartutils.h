#pragma once
#ifndef DBPART_UTILS_H
#define DBPART_UTILS_H
#include <vector>

#include "rfdb/dbPart.h"

namespace rfdb
{
class dbPartUtils
{
public:
    // get all shapes at leaf node
    static void getAllShapes(const dbPart *part, std::vector<TopoDS_Shape> &shps);

    // After use, you should delete the returned pointer
    [[nodiscard]] static dbPart *createCombinedPart(const dbPart *_model);

    // print tree struct of part
    static void printStructOfPart(const dbPart *part, int deep);

    // comparation of all node and sub node
    static bool isPartEqual(const dbPart *lhs, const dbPart *rhs);

    [[nodiscard]] bool updatePartVariant(dbPart *part);
};
} // namespace rfdb
#endif
