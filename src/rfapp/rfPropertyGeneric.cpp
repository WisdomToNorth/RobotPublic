#include "rfPropertyGeneric.h"

namespace rfapp
{

BasePropertyQuantity::BasePropertyQuantity(PropertyGroup *grp, const TextId &name)
    : Property(grp, name)
{
}

template <>
const char PropertyBool::TypeName[] = "rfapp::PropertyBool";
template <>
const char GenericProperty<int>::TypeName[] = "rfapp::PropertyInt";
template <>
const char GenericProperty<double>::TypeName[] = "rfapp::PropertyDouble";
template <>
const char PropertyString::TypeName[] = "rfapp::PropertyString";
template <>
const char GenericProperty<CheckState>::TypeName[] = "rfapp::PropertyCheckState";
template <>
const char PropertyOccPnt::TypeName[] = "rfapp::PropertyOccPnt";
template <>
const char PropertyOccVec::TypeName[] = "rfapp::PropertyOccVec";
template <>
const char PropertyOccTrsf::TypeName[] = "rfapp::PropertyOccTrsf";
template <>
const char GenericProperty<Quantity_Color>::TypeName[] = "rfapp::PropertyOccColor";
template <>
const char GenericProperty<FilePath>::TypeName[] = "rfapp::PropertyFilePath";

} // namespace rfapp
