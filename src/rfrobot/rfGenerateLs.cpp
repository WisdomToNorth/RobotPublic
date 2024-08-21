/****************************************************************************
** Copyright 2023 by KangYucheng.
** All Rights Reserved.
**
** This file is part of RobotConfig software. No part of this file may be
** reproduced in any form or means, without the prior written consent of KangYucheng.
****************************************************************************/

#include "rfGenerateLs.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <gp_Pnt.hxx>

namespace rfrobot
{
void FanucLSGenerator::motionAdd(std::string command)
{
    MN_cnt_++;
    line_cnt_++;
    program_text_.append(command);
    program_text_.append("\n");
}

void FanucLSGenerator::updateLineCnt()
{
    line_cnt_ += getPosSize();
}

size_t FanucLSGenerator::getPosSize() const
{
    size_t cnt = std::max(pos_quan_.size(), pos_le_.size());
    cnt = std::max(cnt, pos_arm_angle_.size());
    return cnt;
}

void FanucLSGenerator::readPosInLocEuler()
{
    for (size_t i = 0; i < pos_le_.size(); i++)
    {
        program_text_.append("P[");
        program_text_.append(std::to_string(i + 1));
        program_text_.append("]{");
        program_text_.append("\n");
        program_text_.append("   GP1:");
        program_text_.append("\n");
        program_text_.append("\tUF : 0, UT : 1,\t\tCONFIG : 'N U T, 0, 0, 0',");
        program_text_.append("\n");
        program_text_.append("\tX =\t");
        program_text_.append(std::to_string(pos_le_[i].location_.X()));
        program_text_.append("\tmm,\tY =\t");
        program_text_.append(std::to_string(pos_le_[i].location_.Y()));
        program_text_.append("\tmm,\tZ =\t");
        program_text_.append(std::to_string(pos_le_[i].location_.Z()));
        program_text_.append("\tmm,");
        program_text_.append("\n");
        // *********************** wpr = zyx？ *******************
        program_text_.append("\tW =\t");
        program_text_.append(std::to_string(pos_le_[i].euler_.X()));
        program_text_.append("\tdeg,\tP =\t");
        program_text_.append(std::to_string(pos_le_[i].euler_.Y()));
        program_text_.append("\tdeg,\tR =\t");
        program_text_.append(std::to_string(pos_le_[i].euler_.Z()));
        program_text_.append("\tdeg");
        program_text_.append("\n");
        program_text_.append("};");
        program_text_.append("\n");
    }
}

static const std::string currentDateTime()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%y-%m-%d  TIME %H:%M:%S; ");
    return ss.str();
}

void FanucLSGenerator::firstPart()
{
    // PROG
    program_text_.append("/PROG ");
    program_text_.append(program_name_);
    program_text_.append("\n");
    // ATTR
    program_text_.append("/ATTR");
    program_text_.append("\n");
    program_text_.append("OWNER\t\t\t= SJTU;");
    program_text_.append("\n");
    program_text_.append("COMMENT\t\t\t= " + comment_ + ";");
    program_text_.append("\n");
    program_text_.append("PROG_SIZE\t\t= " + prog_size_ + ";");
    program_text_.append("\n");

    std::string time_info = "DATE " + currentDateTime();
    program_text_.append("CREATE\t\t\t= " + time_info);
    program_text_.append("\n");
    program_text_.append("MODIFIED\t\t= " + time_info);
    program_text_.append("\n");

    program_text_.append("FILE_NAME\t\t= " + file_name_ + ";");
    program_text_.append("\n");
    program_text_.append("VERSION\t\t\t= " + version_ + ";");
    program_text_.append("\n");
    program_text_.append("LINE_COUNT\t\t= " + std::to_string(line_cnt_) + ";");
    program_text_.append("\n");
    program_text_.append("MEMORY_SIZE\t\t= " + memory_size_ + ";");
    program_text_.append("\n");
    program_text_.append("PROTECT\t\t\t= READ_WRITE;");
    program_text_.append("\n");
    program_text_.append("TCD:  STACK_SIZE\t\t= 0,\n"
                         "      TASK_PRIORITY\t\t= 50,\n"
                         "      TIME_SLICE\t\t= 0,\n"
                         "      BUSY_LAMP_OFF\t\t= 0,\n"
                         "      ABORT_REQUEST\t\t= 0,\n"
                         "      PAUSE_REQUEST\t\t= 0;");
    program_text_.append("\n");
    program_text_.append("DEFAULT_GROUP\t= 1,*,*,*,*;");
    program_text_.append("\n");
    program_text_.append("CONTROL_CODE\t= 00000000 00000000;");
    program_text_.append("\n");

    // APPL
    program_text_.append("/APPL\n");
    program_text_.append("  SPOT : TRUE ;");
    program_text_.append("\n");
    program_text_.append("\n");
    program_text_.append("AUTO_SINGULARITY_HEADER;");
    program_text_.append("\n");
    program_text_.append("  ENABLE_SINGULARITY_AVOIDANCE   : FALSE;");
    program_text_.append("\n");
    program_text_.append("  SPOT Welding Equipment Number : 1 ;");
    program_text_.append("\n");
}

void FanucLSGenerator::commandPart()
{
    size_t pos_cnt = getPosSize();
    line_cnt_ = line_cnt_ - pos_cnt + 2;
    for (size_t i = 0; i < pos_cnt; i++)
    {
        program_text_.append("\t" + std::to_string(i + line_cnt_ + 1) + ":L" + " P[" +
                             std::to_string(i + 1) + "] " + velocity_ + "mm/sec " + inter_mode_ +
                             ";\n");
    }
    line_cnt_ += pos_cnt;
}

void FanucLSGenerator::secondPartHeader()
{
    // MN
    program_text_.append("/MN");
    program_text_.append("\n");
    program_text_.append("\t1:  UFRAME_NUM=0 ;");
    program_text_.append("\n");
    program_text_.append("\t2:  UTOOL_NUM=1 ;");
    program_text_.append("\n");
}
void FanucLSGenerator::readInArmAngle()
{
}
void FanucLSGenerator::posPart()
{
    program_text_.append("/POS");
    program_text_.append("\n");

    if (!pos_arm_angle_.empty())
    {
        readInArmAngle();
    }
    else if (!pos_quan_.empty())
    {
        convertToLE();
        readPosInLocEuler();
    }
    else
    {
        readPosInLocEuler();
    }

    program_text_.append("\n");
    program_text_.append("/END");
}

void FanucLSGenerator::write(const std::string &filename) const
{
    std::ofstream output_file;
    output_file.open(filename);
    if (output_file.fail())
    {
        std::cout << "cannot open file!\n";
        return;
    }
    output_file << program_text_;
    output_file.close();
}

void FanucLSGenerator::setPos(const std::vector<std::array<double, 7>> &pos_quan)
{
    pos_quan_ = pos_quan;
}
void FanucLSGenerator::setPos(const std::vector<rfrobot::Posture> &pos_le)
{
    pos_le_ = pos_le;
}
void FanucLSGenerator::setPos(const std::vector<std::vector<int>> &pos_arm_angle)
{
    pos_arm_angle_ = pos_arm_angle;
}

std::string FanucLSGenerator::getFullProgramStr()
{
    return program_text_;
}

void FanucLSGenerator::convertToLE()
{
    assert(pos_le_.empty());
    for (size_t i = 0; i < pos_quan_.size(); i++)
    {
        Eigen::Quaterniond quaternion4(pos_quan_[i][6], pos_quan_[i][3], pos_quan_[i][4],
                                       pos_quan_[i][5]);
        Eigen::Vector3d eulerAngle = quaternion4.matrix().eulerAngles(2, 1, 0); // zyx
        double angle_W = eulerAngle[2] / M_PI * 180;
        double angle_P = eulerAngle[1] / M_PI * 180;
        double angle_R = eulerAngle[0] / M_PI * 180;
        pos_le_.emplace_back(gp_Pnt(pos_quan_[i][1], pos_quan_[i][2], pos_quan_[i][3]),
                             gp_Pnt(angle_W, angle_P, angle_R));
    }
    std::vector<std::array<double, 7>> pos_quan_new;
    pos_quan_.swap(pos_quan_new);
}
} // namespace rfrobot
