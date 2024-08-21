#include "cmd_exportfile.h"

#include <iostream>
#include <thread>

namespace rfgui
{
void CmdExportFile::execute()
{
    std::cout << "CmdExportFile::execute()\n";
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "CmdExportFile::execute() end\n";
    this->done();
    // auto context = getContext();
    // auto model = context->getModel();
    // auto filepath = context->getFilePath();
    // if (model.IsNull() || filepath.empty())
    // {
    //     return;
    // }
    // FileParser::exportFile(model, filepath);
}

// bool FileParser::exportFile(Handle(AIS_ColoredShape) _model, const std::string &_filepath)
// {
//     if (_model.IsNull())
//     {
//         return false;
//     }
//     std::string file_type = rfbase::DataUtils::getFileExtension(_filepath);
//     if (file_type == "step" || file_type == "stp")
//     {
//         return false;
//     }
//     else if (file_type == "json" || file_type == "brep")
//     {
//         BrepWriter writer;
//         rfdb::dbVariant part = rfdb::dbVariant(new rfdb::dbPart(_model));
//         writer.transfer(part, nullptr, false);
//         return writer.save(_filepath, nullptr);
//     }
//     else // use assimp
//     {
//     }
//     return true;
// }
} // namespace rfgui
