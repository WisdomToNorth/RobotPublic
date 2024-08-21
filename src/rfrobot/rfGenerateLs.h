
#ifndef LS_GEN_H
#define LS_GEN_H

#include <array>
#include <string>
#include <vector>

#include "rfRobotDefine.h"

namespace rfrobot
{
class FanucLSGenerator
{
public:
    enum class KInterMode;
    enum class KInterPrecision;

    void firstPart();
    void secondPartHeader();
    void motionAdd(std::string command);

    void readInArmAngle(); // todo
    void readPosInLocEuler();
    void commandPart();
    void updateLineCnt();
    void posPart();
    void write(const std::string &filename) const;
    std::string getFullProgramStr();

    void setPos(const std::vector<std::array<double, 7>> &pos_quan);
    void setPos(const std::vector<rfrobot::Posture> &pos_le);
    void setPos(const std::vector<std::vector<int>> &pos_arm_angle);

public:
    std::string program_name_;
    std::string comment_;
    std::string prog_size_;
    // std::string create_data_;
    std::string file_name_;
    std::string version_;
    std::string memory_size_;
    std::string velocity_;
    std::string inter_mode_; // todo: replace to enum
    std::string PATH_;

    size_t line_cnt_ = 0;
    size_t MN_cnt_ = 0;

    enum class KInterMode
    {
        L,
        C,
        J // 直线，圆弧，关节
    };
    enum class KInterPrecision
    {
        FINE,
        CNT
    };

private:
    void convertToLE();
    size_t getPosSize() const;

private:
    using ProgramText = std::string;
    ProgramText program_text_;

    std::vector<std::array<double, 7>> pos_quan_; // x,y,z,Quaternion
    std::vector<rfrobot::Posture> pos_le_;
    std::vector<std::vector<int>> pos_arm_angle_;
};
} // namespace rfrobot
#endif
