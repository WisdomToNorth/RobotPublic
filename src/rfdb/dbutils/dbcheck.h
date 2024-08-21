#pragma once
#ifndef DBCHECK_H
#define DBCHECK_H

namespace rfdb
{
class dbPart;
class dbCheck
{
public:
    // get all shapes at leaf node
    static bool checkPart(const dbPart *part);
};
} // namespace rfdb
#endif
