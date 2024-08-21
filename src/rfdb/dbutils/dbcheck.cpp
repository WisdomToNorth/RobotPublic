#include "dbcheck.h"

#include "rfdb/dbPart.h"

namespace rfdb
{
bool dbCheck::checkPart(const dbPart *part)
{
    if (part == nullptr)
    {
        return false;
    }

    if (!part->data_.IsNull() && !part->children_.empty())
    {
        // if have shape, it must be leaf node
        return false;
    }
    else if (part->isLeaf() && part->data_.IsNull())
    {
        // empty leaf node
        return false;
    }
    else
    {
        return true;
    }
}
} // namespace rfdb
