#include "dbProperty.h"

#include <memory>
#include <string>

#include "rfapp/rfsignal.h"
#include "rfbase/datautils.h"

namespace rfdb
{

struct Implementation
{
    rfapp::RfSignal<bool> visible_change_signal_;
    rfapp::RfSignal<int> value_change_signal_;
};

dbProperty::dbProperty()
{
    impl_ = new Implementation();
    id_ = rfbase::DataUtils::generateUuid();
}
dbProperty::~dbProperty()
{
    delete impl_;
}

void dbProperty::setVisibleChangeEvent(std::function<void(bool)> func)
{
    impl_->visible_change_signal_.connect(func);
}
void dbProperty::setValueChangeEvent(std::function<void(int)> func)
{
    impl_->value_change_signal_.connect(func);
}
void dbProperty::setVisiable(bool visiable)
{
    is_visiable_ = visiable;
    impl_->visible_change_signal_.send(visiable);
}
void dbProperty::setValue(int value)
{
    value_ = value;
    impl_->value_change_signal_.send(value);
}

void dbProperty::setDeleted(bool deleted)
{
    is_deleted_ = deleted;
}
void dbProperty::setDirty(bool dirty)
{
    is_dirty_ = dirty;
}
void dbProperty::setName(std::string name)
{
    name_ = name;
}
void dbProperty::setFilePath(std::string file_path)
{
    file_path_ = file_path;
}
void dbProperty::setId(RfId id)
{
    id_ = id;
}

RfId dbProperty::getId() const
{
    return id_;
}

int dbProperty::getValue() const
{
    return value_;
}
bool dbProperty::isVisiable() const
{
    return is_visiable_;
}
bool dbProperty::isDeleted() const
{
    return is_deleted_;
}
bool dbProperty::isDirty() const
{
    return is_dirty_;
}

void dbProperty::setInitTrsf(const gp_Trsf &trsf)
{
    init_trsf_ = trsf;
}
void dbProperty::setColor(const Quantity_Color &color)
{
    color_ = color;
}
std::string dbProperty::getName() const
{
    return name_;
}
std::string dbProperty::getFilePath() const
{
    return file_path_;
}
gp_Trsf dbProperty::getInitTrsf() const
{
    return init_trsf_;
}
Quantity_Color dbProperty::getColor() const
{
    return color_;
}
void dbProperty::setTransparency(double transparency)
{
    transparency_ = transparency;
}

double dbProperty::getTransparency() const
{
    return transparency_;
}

void dbProperty::setInitLoc(const TopLoc_Location &loc)
{
    init_loc_ = loc;
}

TopLoc_Location dbProperty::getInitLoc() const
{
    return init_loc_;
}
} // namespace rfdb
// namespace rfdb
