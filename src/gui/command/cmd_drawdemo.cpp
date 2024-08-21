#include "cmd_drawdemo.h"

#include <QString>
#include <QStringList>

#include <gp_Ax2.hxx>

#include "rfdb/dbPart.h"
#include "rfdb/dbutils/shapebuilder.h"

#include "app_system.h"
#include "ui/form/listwidget.h"
namespace rfgui
{
void CmdAddDemoVariant::execute()
{
    QStringList items;
    items << ("Bottle") << ("CubePlusQuarterBall") << ("Ball") << ("DumbBell") << ("Abassica");

    ListWidget list_widget(items, GApp::instance().getMainWindow());
    list_widget.exec();
    if (list_widget.exec_ == ListWidget::ListHandleState::KAccept)
    {
        QString item = items.at(list_widget.index_c1_);
        DemoType type = DemoType::None;
        if (item == ("Bottle"))
        {
            type = DemoType::Bottle;
        }
        else if (item == ("CubePlusQuarterBall"))
        {
            type = DemoType::CubePlusQuarterBall;
        }
        else if (item == ("Ball"))
        {
            type = DemoType::Ball;
        }
        else if (item == ("DumbBell"))
        {
            type = DemoType::DumbBell;
        }
        else if (item == ("Abassica"))
        {
            type = DemoType::Abassica;
        }
        generate(type);
    }

    this->done();
}

void CmdAddDemoVariant::generate(DemoType type)
{
    switch (type)
    {
    case DemoType::Bottle:
    {
        const auto &shp = rfdb::ShapeBuilder::makeBottle();
        rfdb::dbPart *part = new rfdb::dbPart(shp);
        part->getProperty()->setName("Bottle");
        GApp::instance().addDbVariant(rfdb::dbVariant(part), UseType::Database,
                                      GApp::instance().curViewerIndex());
        break;
    }
    case DemoType::CubePlusQuarterBall:
    {
        const auto &shp = rfdb::ShapeBuilder::makeCubePlusQuarterBall();
        rfdb::dbPart *part = new rfdb::dbPart(shp);
        part->getProperty()->setName("CubePlusQuarterBall");
        GApp::instance().addDbVariant(rfdb::dbVariant(part), UseType::Database,
                                      GApp::instance().curViewerIndex());
        break;
    }
    case DemoType::Ball:
    {
        const auto &shp = rfdb::ShapeBuilder::makeBall({0, 0, 0});
        rfdb::dbPart *part = new rfdb::dbPart(shp);
        part->getProperty()->setName("Ball");
        part->getProperty()->setColor(Quantity_NOC_AZURE2);
        GApp::instance().addDbVariant(rfdb::dbVariant(part), UseType::Database,
                                      GApp::instance().curViewerIndex());
        break;
    }
    case DemoType::DumbBell:
    {
        rfdb::dbPart *part = rfdb::ShapeBuilder::createDumbBellVariant({0, 0, 0}, {0, 0, 100});
        part->getProperty()->setName("DumbBell");
        GApp::instance().addDbVariant(rfdb::dbVariant(part), UseType::Database,
                                      GApp::instance().curViewerIndex());
        break;
    }
    case DemoType::Abassica:
    {
        gp_Dir dir(0, 0, 1);
        rfdb::dbPart *part = rfdb::ShapeBuilder::buildAbassica(gp_Ax2({0, 0, 0}, dir), true, 100);
        part->getProperty()->setName("Abassica");
        GApp::instance().addDbVariant(rfdb::dbVariant(part), UseType::Database,
                                      GApp::instance().curViewerIndex());
        break;
    }
    case DemoType::None:
    default: break;
    }
}
} // namespace rfgui
