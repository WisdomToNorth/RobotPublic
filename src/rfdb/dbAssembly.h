#pragma once
#ifndef DBASSEMBLY_H
#define DBASSEMBLY_H

#include "dbPart.h"
#include "dbProperty.h"

namespace rfdb
{

class dbAssembly
{
private:
    dbProperty *property_ = nullptr;
    std::vector<dbPart *> parts_;

public:
    dbAssembly();
    ~dbAssembly();

    // Compration of Id
    bool operator==(const dbAssembly &part);

    void bindVisibleChangeFunc(const std::function<void(bool)> &func);
    void setVisiable(bool visiable);
    void setShape(Handle(AIS_ColoredShape) model);
    bool getVisiable() const;
    bool isEmpty() const;
    RfId getId() const;
    dbProperty *getProperty() const
    {
        return property_;
    }
};
} // namespace rfdb
#endif
