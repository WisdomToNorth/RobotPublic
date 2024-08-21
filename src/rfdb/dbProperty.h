#pragma once
#ifndef DBPROPERTY_H
#define DBPROPERTY_H
#include <functional>

#include <Quantity_Color.hxx>
#include <TopLoc_Location.hxx>
#include <gp_Trsf.hxx>

#include "dbCommon.h"

namespace rfdb
{
struct Implementation;

class dbProperty
{
private:
    Implementation *impl_;

    RfId id_;
    std::string file_path_;
    std::string name_;
    gp_Trsf init_trsf_;
    TopLoc_Location init_loc_;
    Quantity_Color color_;

    double transparency_ = 0.0;
    int value_ = -1;
    bool is_visiable_ = true;
    bool is_deleted_ = false;
    bool is_dirty_ = false;

public:
    dbProperty();
    ~dbProperty();

    void setVisibleChangeEvent(std::function<void(bool)> func);
    void setValueChangeEvent(std::function<void(int)> func);
    void setVisiable(bool visiable);
    void setValue(int value);
    void setDeleted(bool deleted);
    void setDirty(bool dirty);
    void setName(std::string name);
    void setFilePath(std::string file_path);
    void setId(RfId id);
    void setInitTrsf(const gp_Trsf &trsf);
    void setColor(const Quantity_Color &color);
    void setTransparency(double transparency);
    void setInitLoc(const TopLoc_Location &loc);

    double getTransparency() const;
    RfId getId() const;
    int getValue() const;
    bool isVisiable() const;
    bool isDeleted() const;
    bool isDirty() const;
    std::string getName() const;
    std::string getFilePath() const;
    Quantity_Color getColor() const;
    gp_Trsf getInitTrsf() const;
    TopLoc_Location getInitLoc() const;
};

} // namespace rfdb
#endif
