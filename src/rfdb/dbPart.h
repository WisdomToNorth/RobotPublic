#pragma once
#ifndef DBPART_H
#define DBPART_H

#include <AIS_ColoredShape.hxx>
#include <TopoDS_Shape.hxx>

#include "dbProperty.h"

namespace rfdb
{
class dbPart
{
private:
    friend class dbCheck;

    dbProperty *property_ = nullptr;
    TopoDS_Shape data_;
    dbPart *parent_ = nullptr;
    std::vector<dbPart *> children_;

public:
    dbPart();
    dbPart(Handle(AIS_ColoredShape) model);
    dbPart(const TopoDS_Shape &shape);
    ~dbPart();

    dbPart(const dbPart &) = delete;
    dbPart &operator=(const dbPart &) = delete;
    dbPart(dbPart &&) = delete;
    dbPart &operator=(dbPart &&) = delete;

    // comparation of ID
    bool operator==(const dbPart &part);

    void bindVisibleChangeFunc(const std::function<void(bool)> &func);
    void setVisiable(bool visiable);

    [[nodiscard]] bool setShape(const TopoDS_Shape &shape);
    [[nodiscard]] bool setShape(Handle(AIS_ColoredShape) model); // Rename this function
    Handle(AIS_ColoredShape) getShape();
    TopoDS_Shape getTopoShape() const;
    bool getVisiable() const;
    bool isEmpty() const;
    RfId getId() const;
    std::string getName() const;
    dbProperty *getProperty() const;

    void addChild(dbPart *child);
    void addChildren(std::vector<dbPart *> children);
    void removeChild(dbPart *child);
    void setParent(dbPart *parent);
    dbPart *getParent() const;
    void getChildren(std::vector<dbPart *> &children) const;
    bool hasChild(const dbPart *child) const;
    bool isRoot() const;
    bool isLeaf() const;
    void setTransparency(double transparency);
    size_t childrenCount() const;
};
} // namespace rfdb
#endif
