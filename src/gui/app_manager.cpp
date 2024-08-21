#include "app_manager.h"

#include <iostream>

#include <QApplication>
#include <QMessageBox>

#include "app_system.h"

namespace rfgui
{

std::string handleInput(int argc, char *argv[])
{
    if (argc > 1)
    {
        if (argv[1] == std::string("Command Mode"))
        {
            std::string command_str;
            while (std::cin >> command_str)
            {
                return command_str;
            }
        }
    }
    return "";
}

void AppManager::initApp()
{
}

int AppManager::runApp(int argc, char *argv[])
{
    QApplication a(argc, argv);

    rfgui::GApp &system = rfgui::GApp::instance();

    if (handleInput(argc, argv) == std::string("Command Mode"))
    {
        std::string command_str;
        while (std::cin >> command_str)
        {
            if (system.addToCmdList(command_str))
            {
                std::ignore = system.executeLatestCmd();
            }
        }
    }
    else
    {
        system.createGUI();
    }

    try
    {
        system.loadSetting();
        a.exec();
        system.saveSetting();
        return EXIT_SUCCESS;
    }
    catch (const std::exception &theError)
    {
        std::cerr << theError.what() << '\n';
        QMessageBox::critical(nullptr, "Error", theError.what());
        return EXIT_FAILURE;
    }
}

} // namespace rfgui
