/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#include "ais_text.h"

#include <vector>

#include <Graphic3d_AspectText3d.hxx>
#include <OSD_Environment.hxx>
#include <Prs3d_Root.hxx>
#include <Prs3d_Text.hxx>
#include <Prs3d_TextAspect.hxx>
#include <SelectMgr_SelectableObject.hxx>
#include <SelectMgr_Selection.hxx>
#include <Standard_CString.hxx>
#include <Standard_IStream.hxx>
#include <Standard_Integer.hxx>
#include <Standard_OStream.hxx>
#include <Standard_Real.hxx>
#include <Standard_Transient.hxx>
#include <Standard_Version.hxx>
#include <TCollection_AsciiString.hxx>
#include <gp_Pnt.hxx>

namespace Mayo
{

AIS_Text::AIS_Text(const TCollection_ExtendedString &text, const gp_Pnt &pos)
{
    TextProperties defaultProps;
    text_props_.push_back(defaultProps);
    this->setText(text);
    this->setPosition(pos);
}

Handle_Prs3d_TextAspect AIS_Text::presentationTextAspect(unsigned i) const
{
    return this->isValidTextIndex(i) ? text_props_.at(i).m_aspect : Handle_Prs3d_TextAspect();
}

Handle_Graphic3d_AspectText3d AIS_Text::graphicTextAspect(unsigned i) const
{
    return this->isValidTextIndex(i) ? text_props_.at(i).m_aspect->Aspect() :
                                       Handle_Graphic3d_AspectText3d();
}

gp_Pnt AIS_Text::position(unsigned i) const
{
    return this->isValidTextIndex(i) ? text_props_.at(i).m_position : gp_Pnt();
}

TCollection_ExtendedString AIS_Text::text(unsigned i) const
{
    return this->isValidTextIndex(i) ? text_props_.at(i).m_text : TCollection_ExtendedString();
}

bool AIS_Text::isValidTextIndex(unsigned i) const
{
    return i < this->textCount();
}

unsigned AIS_Text::textCount() const
{
    return static_cast<unsigned>(text_props_.size());
}

void AIS_Text::addText(const TCollection_ExtendedString &text, const gp_Pnt &pos)
{
    TextProperties newProps;
    text_props_.push_back(newProps);
    const unsigned i = this->textCount() - 1;

    this->presentationTextAspect(i)->SetColor(default_color_);
    this->presentationTextAspect(i)->SetFont(default_font_);

    this->setPosition(pos, i);
    this->setText(text, i);
    this->setTextDisplayMode(default_text_display_mode_, i);
    this->setTextStyle(default_text_style_, i);
    this->setTextBackgroundColor(default_bgd_color_, i);
}

void AIS_Text::setPosition(const gp_Pnt &pos, unsigned i)
{
    if (this->isValidTextIndex(i))
        text_props_.at(i).m_position = pos;
}

void AIS_Text::setText(const TCollection_ExtendedString &v, unsigned i)
{
    if (this->isValidTextIndex(i))
        text_props_.at(i).m_text = v;
}

//! Only works when the i-th text display mode is set to Aspect_TODT_SUBTITLE
void AIS_Text::setTextBackgroundColor(const Quantity_Color &color, unsigned i)
{
    if (this->isValidTextIndex(i))
        this->graphicTextAspect(i)->SetColorSubTitle(color);
}

void AIS_Text::setTextDisplayMode(Aspect_TypeOfDisplayText mode, unsigned i)
{
    if (this->isValidTextIndex(i))
        this->graphicTextAspect(i)->SetDisplayType(mode);
}

void AIS_Text::setTextStyle(Aspect_TypeOfStyleText style, unsigned i)
{
    if (this->isValidTextIndex(i))
        this->graphicTextAspect(i)->SetStyle(style);
}

void AIS_Text::setDefaultColor(const Quantity_Color &c)
{
    default_color_ = c;
}

void AIS_Text::setDefaultFont(const char *fontName)
{
    default_font_ = fontName;
}

void AIS_Text::setDefaultTextBackgroundColor(const Quantity_Color &c)
{
    default_bgd_color_ = c;
}

void AIS_Text::setDefaultTextDisplayMode(Aspect_TypeOfDisplayText mode)
{
    default_text_display_mode_ = mode;
}

void AIS_Text::setDefaultTextStyle(Aspect_TypeOfStyleText style)
{
    default_text_style_ = style;
}

void AIS_Text::Compute(const Handle(PrsMgr_PresentationManager) &,
                       const Handle(Prs3d_Presentation) & pres, const int)
{
    for (unsigned i = 0; i < this->textCount(); ++i)
    {
        Prs3d_Text::Draw(pres->CurrentGroup(), this->presentationTextAspect(i), this->text(i),
                         this->position(i));
    }
}

void AIS_Text::ComputeSelection(const Handle(SelectMgr_Selection) &, const int)
{
}

AIS_Text::TextProperties::TextProperties()
    : m_aspect(new Prs3d_TextAspect)
{
}

bool AIS_Text::TextProperties::operator==(const AIS_Text::TextProperties &other) const
{
    return m_font == other.m_font &&
           m_position.SquareDistance(other.m_position) < Precision::Confusion() &&
           m_text == other.m_text && m_aspect == other.m_aspect;
}

} // namespace Mayo
