#include "reader_factory.h"

#include <string_view>

#include "io_format.h"
#include "reader_base.h"
#include "reader_brep.h"
#include "reader_ply.h"
#include "reader_robot.h"
#include "reader_step.h"

namespace rfio
{
FileReaderBase *ReaderFactory::createReader(const std::string &suffix)
{
    FileReaderBase *reader = nullptr;

    std::string_view suffix_view = suffix;
    auto format = FormatUtils::formatFromSuffix(suffix_view);

    switch (format)
    {
    case Format::RfContext:
    {
        reader = new rfio::RobotReader(); // TODO
        break;
    }
    case Format::STEP:
    {
        reader = new rfio::StepReader();
        break;
    }
    case Format::PLY:
    {
        reader = new rfio::PlyReader();
        break;
    }
    case Format::RfModel:
    {
        reader = new rfio::BrepReader();
        break;
    }
    case Format::Unknown:
    case Format::Image:
    case Format::IGES:
    case Format::OCCBREP:
    case Format::STL:
    case Format::OBJ:
    case Format::GLTF:
    case Format::VRML:
    case Format::AMF:
    case Format::DXF:
    case Format::OFF:
    default: break;
    }

    return reader;
}

} // namespace rfio
