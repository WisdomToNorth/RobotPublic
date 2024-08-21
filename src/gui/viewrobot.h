#pragma once
#ifndef VIEW_ROBOT_H
#define VIEW_ROBOT_H

#include "rfdb/dbRobot.h"

#include "viewnode.h"

namespace rfgui
{
class ViewRobot
{
public:
    ViewRobot(rfdb::dbRobot *robot_db, ViewNode *part_root) :
        db_robot_(robot_db), part_node_(part_root)
    {
    }
    ~ViewRobot()
    {
    }

public:
    rfdb::dbRobot *db_robot_;
    ViewNode *part_node_;
};
} // namespace rfgui
#endif
