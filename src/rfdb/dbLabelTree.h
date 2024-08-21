#pragma once
#ifndef DBLABELTREE_H
#define DBLABELTREE_H

#include "dbProperty.h"
namespace rfdb
{
class dbLabelTree
{
private:
    dbProperty *property_ = nullptr;

public:
    dbLabelTree(/* args */);
    ~dbLabelTree();

    dbProperty *getProperty() const;
};
} // namespace rfdb
#endif
