#include <cassert>

#include "generated/config.h"
#include "rfbase/datautils.h"
#include "rfbase/rfTimer.h"
#include "rfgeo3d/shapeutils.h"
#include "rfio/reader_brep.h"
#include "rfio/reader_step.h"
#include "rfio/writer_brep.h"

void convertAll()
{
    using namespace rfio;
    using namespace rfgeo;
    std::string to_convert_path = REG_RESOURCE_FOLDER + "/rfbrep";
    std::vector<std::string> files;
    std::ignore = rfbase::DataUtils::getAllFilesByDir(to_convert_path, ".json", true, files);

    for (const auto &file : files)
    {
        std::string brep_file = to_convert_path + '/' + file;
        std::cout << "Converting file: " << file << '\n';
        BrepReader reader;
        [[maybe_unused]] bool succ = reader.readFile(brep_file, nullptr);
        rfdb::dbVariant var = reader.transfer(nullptr);

        assert(succ);

        BrepWriter writer;
        writer.transfer(var, nullptr, false);
        std::string out_file = REG_OUTPUT_FOLDER + '/' + file;
        writer.save(out_file, nullptr);
    }
}
