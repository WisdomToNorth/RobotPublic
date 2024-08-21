#pragma once
#ifndef READER_TRACE_H
#define READER_TRACE_H
#include <vector>

#include "rfdb/dbRobot.h"

#include "reader_base.h"

namespace rfio
{
class RobotReader : public FileReaderBase
{
public:
    RobotReader();
    ~RobotReader();

    // filename: absolutly path of CONTEXT.json
    bool readFile(const std::string &filepath, rfapp::TaskProgress *progress) override;
    rfdb::dbVariant transfer(rfapp::TaskProgress *progress, bool concurrent = false) override;

    // default source path is "../resources", if you want to use other source path, please set it
    // before read file
    void setSourcePath(const std::string &filename)
    {
        source_path_set_ = true;
        source_path_ = filename;
    }

public:
    static bool loadParams(const std::string &filename,
                           std::vector<rfdb::dbRobot::JointParams> &arm_params);

private:
    static rfdb::dbRobot::JType strToJointType(const std::string &str);
    static std::string getRobotModelPath(const std::string &filename, const std::string &robot_id);
    static std::string getRobotParamPath(const std::string &filename, const std::string &robot_id);
    static bool loadModel(const std::string &filename, rfdb::dbVariant &part);

private:
    std::string file_path_;
    std::vector<rfdb::dbRobot::JointParams> arm_params_;
    rfdb::dbVariant robot_model_;

    // for test
    std::string source_path_;
    bool source_path_set_ = false;
};
} // namespace rfio
#endif
