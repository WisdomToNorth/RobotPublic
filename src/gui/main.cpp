
#include "app_manager.h"

int main(int argc, char *argv[])
{
    rfgui::AppManager::instance().initApp();
    return rfgui::AppManager::instance().runApp(argc, argv);
}
