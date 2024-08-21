
#pragma once
#ifndef RF_PROPERTY_H
#define RF_PROPERTY_H

#include <string>
#include <vector>

#include "textid.h"

namespace rfapp
{
class PropertyGroup;

// Provides an abstract storage of a value with associated meta-data(name, description, ...)
class Property
{
public:
    Property(PropertyGroup *group, const TextId &name);
    Property() = delete;
    Property(const Property &) = delete;
    Property(Property &&) = delete;
    Property &operator=(const Property &) = delete;
    Property &operator=(Property &&) = delete;
    virtual ~Property() = default;

    inline PropertyGroup *group() const
    {
        return group_;
    }

    const TextId &name() const;
    std::string_view label() const;

    inline const std::string &description() const
    {
        return description_;
    }
    inline void setDescription(std::string_view text)
    {
        description_ = text;
    }

    inline bool isUserReadOnly() const
    {
        return is_user_read_only_;
    }
    inline void setUserReadOnly(bool on)
    {
        is_user_read_only_ = on;
    }

    inline bool isUserVisible() const
    {
        return is_user_visible_;
    }

    inline void setUserVisible(bool on)
    {
        is_user_visible_ = on;
    }

    inline bool isEnabled() const
    {
        return is_enabled_;
    }
    void setEnabled(bool on);

    virtual const char *dynTypeName() const = 0;

protected:
    void notifyAboutToChange();
    void notifyChanged();
    void notifyEnabled(bool on);

    bool isValid() const;

    bool hasGroup() const;

    template <typename T>
    bool setValueHelper(Property *prop, T *ptrValue, const T &newValue)
    {
        bool okResult = true;
        if (prop->hasGroup())
        {
            prop->notifyAboutToChange();
            const T previousValue = *ptrValue;
            *ptrValue = newValue;
            okResult = prop->isValid();
            if (okResult)
                prop->notifyChanged();
            else
                *ptrValue = previousValue;
        }
        else
        {
            *ptrValue = newValue;
            prop->notifyChanged();
        }

        return okResult;
    }

private:
    PropertyGroup *const group_ = nullptr;
    const TextId name_;
    std::string description_;
    bool is_user_read_only_ = false;
    bool is_user_visible_ = true;
    bool is_enabled_ = true;
};

} // namespace rfapp
#endif