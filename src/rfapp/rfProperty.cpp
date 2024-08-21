#include "rfProperty.h"

#include <algorithm>
#include <cstring>

#include "rfPropertyGroup.h"

namespace rfapp
{

const TextId &Property::name() const
{
    return name_;
}

std::string_view Property::label() const
{
    return name_.tr();
}

void Property::setEnabled(bool on)
{
    if (is_enabled_ != on)
    {
        is_enabled_ = on;
        this->notifyEnabled(on);
    }
}

Property::Property(PropertyGroup *group, const TextId &name)
    : group_(group)
    , name_(name)
{
    if (group_)
        group_->addProperty(this);
}

void Property::notifyAboutToChange()
{
    if (group_ && !group_->isPropertyChangedBlocked())
        group_->onPropertyAboutToChange(this);
}

void Property::notifyChanged()
{
    if (group_ && !group_->isPropertyChangedBlocked())
        group_->onPropertyChanged(this);
}

void Property::notifyEnabled(bool on)
{
    if (group_)
        group_->onPropertyEnabled(this, on);
}

bool Property::isValid() const
{
    if (group_)
        return group_->isPropertyValid(this);

    return true;
}

bool Property::hasGroup() const
{
    return group_ != nullptr;
}

} // namespace rfapp
