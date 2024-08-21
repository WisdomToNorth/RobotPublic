#pragma once
#ifndef VIEW_TREE_H
#define VIEW_TREE_H

#include <vector>

#include <AIS_InteractiveObject.hxx>

#include "rfdb/dbVariant.h"

namespace rfgui
{
enum class UseType
{
    Unknown = 0,
    Database,
    Auxiliary,
    Preview
};

struct ViewNode
{
    std::vector<ViewNode *> children_;
    ViewNode *parent_;

    // simple dbVariant, no dbRobot
    rfdb::dbVariant db_variant_;

    // init from dbVariant->property->init_trsf, then update by user
    Handle(AIS_InteractiveObject) viewmodel_item_ = NULL;

    std::string name_;
    UseType type_;
    double transparency_ = 0.0;
    int index_in_viewer_ = 0;
    bool is_selected_;
    bool is_visible_;
    bool is_box_visible_;

public:
    ViewNode() :
        parent_(nullptr), type_(UseType::Unknown), is_selected_(false), is_visible_(true),
        is_box_visible_(false)
    {
    }

    ViewNode(const rfdb::dbVariant &var) :
        parent_(nullptr), db_variant_(var), type_(UseType::Unknown), is_selected_(false),
        is_visible_(true), is_box_visible_(false)
    {
    }

    ~ViewNode()
    {
    }
};
} // namespace rfgui
#endif
