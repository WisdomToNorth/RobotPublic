#include "rfPropertyGroup.h"

#include <algorithm>
#include <cassert>
#include <cstring>

namespace rfapp
{

PropertyGroup::PropertyGroup(PropertyGroup *parentGroup)
    : parent_group_(parentGroup)
{
}

std::span<Property *const> PropertyGroup::properties() const
{
    return properties_;
}

void PropertyGroup::restoreDefaults()
{
}

void PropertyGroup::onPropertyAboutToChange(Property *prop)
{
    if (parent_group_)
        parent_group_->onPropertyAboutToChange(prop);
}

void PropertyGroup::onPropertyChanged(Property *prop)
{
    if (parent_group_)
        parent_group_->onPropertyChanged(prop);
}

void PropertyGroup::onPropertyEnabled(Property *prop, bool on)
{
    if (parent_group_)
        parent_group_->onPropertyEnabled(prop, on);
}

bool PropertyGroup::isPropertyValid(const Property *) const
{
    return true;
}

void PropertyGroup::blockPropertyChanged(bool on)
{
    property_changed_blocked_ = on;
}

bool PropertyGroup::isPropertyChangedBlocked() const
{
    return property_changed_blocked_;
}

void PropertyGroup::addProperty(Property *prop)
{
    assert(prop != nullptr);
    assert(prop->group() == this);
    properties_.emplace_back(prop);
}

void PropertyGroup::removeProperty(Property *prop)
{
    auto it = std::find(properties_.begin(), properties_.end(), prop);
    if (it != properties_.end())
        properties_.erase(it);
}

void PropertyGroupSignals::onPropertyAboutToChange(Property *prop)
{
    PropertyGroup::onPropertyAboutToChange(prop);
    this->signal_property_about_to_change_.send(prop);
}

void PropertyGroupSignals::onPropertyChanged(Property *prop)
{
    PropertyGroup::onPropertyChanged(prop);
    this->signal_property_changed_.send(prop);
}
} // namespace rfapp
