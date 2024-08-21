#include <QFileDialog>
#include <QString>

#include "generated/config.h"
#include "rfbase/datautils.h"
#include "rfdb/dbRobot.h"
#include "rfio/io_format.h"
#include "rfio/reader_base.h"
#include "rfio/reader_factory.h"
#include "rflog/rfLog.h"

#include "app_system.h"
#include "cmd_importfile.h"
namespace rfgui
{
void CmdImportFile::execute()
{
    std::vector<rfio::Format> enabled_formats = {rfio::Format::RfModel, rfio::Format::RfContext,
                                                 rfio::Format::STEP, rfio::Format::PLY,
                                                 rfio::Format::STL};
    std::string all_filter;
    for (const auto &format : enabled_formats)
    {
        all_filter += rfio::FormatUtils::formatName(format).data() + std::string(" (*.");
        for (const auto &suffix : rfio::FormatUtils::formatFileSuffixes(format))
        {
            all_filter += suffix.data() + std::string(" *.");
        }
        all_filter += ");;";
    }

    QString default_path = QString::fromStdString(APP_RESOURCE_PATH);
    QString filename = QFileDialog::getOpenFileName(nullptr, "Open file", default_path,
                                                    QString::fromStdString(all_filter));
    if (filename.isEmpty())
    {
        this->done();
        return;
    }

    rflog::debug("Open file: {}", filename.toStdString());
    openTheFile(filename.toStdString());
    this->done();
}

void CmdImportFile::openTheFile(const std::string &filename)
{
    std::string file_type =
        rfbase::DataUtils::toLower(rfbase::DataUtils::getFileExtension(filename));

    rfio::FileReaderBase *reader = rfio::ReaderFactory::createReader(file_type);
    if (!reader)
    {
        rflog::error("Can't create reader for file: {}", filename);
        return;
    }

    rfdb::dbVariant var;
    if (reader->readFile(filename, nullptr))
    {
        var = reader->transfer(nullptr, false);
    }

    GApp::instance().addDbVariant(var, UseType::Database, GApp::instance().curViewerIndex());
}
} // namespace rfgui
