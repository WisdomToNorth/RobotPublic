
#pragma once
#ifndef RF_PROPERTY_GROUP_H
#define RF_PROPERTY_GROUP_H

#include <span>
#include <vector>

#include "rfProperty.h"
#include "rfsignal.h"

namespace rfapp
{

// Provides a cohesive container of Property objects
// PropertyGroup defines callbacks to be executed when special events happen on contained
// properties:
//     - a property value is about to be changed
//     - a property value was changed
//     - the "enabled" status of a property was toggled
// A PropertyGroup can be linked to a parent group(optional). In such case the child group executes
// as well the parent group's callbacks.
class PropertyGroup
{
public:
    PropertyGroup(PropertyGroup *parentGroup = nullptr);
    virtual ~PropertyGroup() = default;

    std::span<Property *const> properties() const;

    PropertyGroup *parentGroup() const
    {
        return parent_group_;
    }

    // Reinitialize properties to their default values
    virtual void restoreDefaults();

protected:
    // Callback executed when Property value is about to change
    virtual void onPropertyAboutToChange(Property *prop);

    // Callback executed when Property value was changed
    virtual void onPropertyChanged(Property *prop);

    // Callback executed when Property "enabled" status was changed
    virtual void onPropertyEnabled(Property *prop, bool on);

    virtual bool isPropertyValid(const Property *prop) const;

    void blockPropertyChanged(bool on);
    bool isPropertyChangedBlocked() const;

    void addProperty(Property *prop);
    void removeProperty(Property *prop);

private:
    friend class Property;
    friend struct PropertyChangedBlocker;
    PropertyGroup *parent_group_ = nullptr;
    std::vector<Property *> properties_;
    bool property_changed_blocked_ = false;
};

class PropertyGroupSignals : public PropertyGroup
{
public:
    RfSignal<Property *> signal_property_about_to_change_;
    RfSignal<Property *> signal_property_changed_;

protected:
    void onPropertyAboutToChange(Property *prop) override;
    void onPropertyChanged(Property *prop) override;
};
} // namespace rfapp
#endif