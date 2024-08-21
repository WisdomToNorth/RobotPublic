#include "dbLabelTree.h"

#include "dbProperty.h"
namespace rfdb
{

dbLabelTree::dbLabelTree()
{
}

dbLabelTree::~dbLabelTree()
{
}

dbProperty *dbLabelTree::getProperty() const
{
    return property_;
}
} // namespace rfdb
