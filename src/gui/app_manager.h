#pragma once
#ifndef APP_MANAGER_H
#define APP_MANAGER_H

namespace rfgui
{
class AppManager
{
    AppManager() = default;
    ~AppManager() = default;
    AppManager(const AppManager &) = delete;
    AppManager &operator=(const AppManager &) = delete;

public:
    static AppManager &instance()
    {
        static AppManager instance;
        return instance;
    }
    void initApp();
    int runApp(int argc, char *argv[]);
};
} // namespace rfgui
#endif
