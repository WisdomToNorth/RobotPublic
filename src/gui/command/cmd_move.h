#pragma once
#ifndef COMMAND_MOVE_H
#define COMMAND_MOVE_H

#include <QPoint>

#include <AIS_ColoredShape.hxx>
#include <NCollection_Handle.hxx>
#include <gp_Ax2.hxx>
#include <gp_Pnt.hxx>
#include <gp_Trsf.hxx>

#include "cmd_base.h"
#include "viewnode.h"

namespace rfgui
{
class CmdMoveUi;
class CmdMove : public CmdBase
{
private:
    enum class Direction
    {
        XP,
        XM,
        YP,
        YM,
        ZP,
        ZM
    };

public:
    CmdMove();
    ~CmdMove() override;

    void processVEvent(const rfapp::ViewerEvent &info) override;
    void createRMenu() override;
    void processRMAction(const std::string &action_name) override;

private:
    void execute() override;
    void setCurObj(Handle(SelectMgr_EntityOwner) _currentModel);
    void setBenchmark(gp_Ax2 benchmark);

private:
    void cmdOk();
    void cmdCancel();

    void moveXPos();
    void moveXNeg();
    void moveYPos();
    void moveYNeg();
    void moveZPos();
    void moveZNeg();
    void rotX();
    void rotY();
    void rotZ();

    void moveToOri();
    void alignment();
    void reverse();
    void resetMove();
    void setAsBenchmark();
    void resetBenchmark();
    void setMoveStep(double step);
    void setRotateStep(double step);
    void setReverseDir(bool reverse);

private:
    void objRotate(Direction dir);
    void objMove(Direction dir);
    void transformObj(const gp_Trsf &myTrsf) const;
    gp_Pnt dirEnumToDir(Direction dir);

private:
    friend class CmdMoveUi;
    CmdMoveUi *cmd_move_ui_;

    gp_Ax2 benchmark_;
    ViewNode *current_model_;

    QPoint off_pos_;
    double move_step_ = 0.0;
    double rotate_step_ = 0.0;
    bool reverse_dir_ = false;

private:
    // action name
    static const std::string action_resetmove;
    static const std::string action_move_x_pos;
    static const std::string action_move_x_neg;
    static const std::string action_move_y_pos;
    static const std::string action_move_y_neg;
    static const std::string action_move_z_pos;
    static const std::string action_move_z_neg;
};
} // namespace rfgui
#endif
