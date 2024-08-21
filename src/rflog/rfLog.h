#ifndef FRLOG_H
#define FRLOG_H

#include <fmt/core.h>
#include <fmt/format.h>
#include <string>

namespace rflog
{
class RfLogListWidget;
struct LoggerImpl;

class FRLog
{
private:
    FRLog();
    ~FRLog();
    static std::string formatDuration(double totalSeconds);

public:
    static FRLog *instancePtr()
    {
        static FRLog instance;
        return &instance;
    }
    static FRLog &instance()
    {
        return *instancePtr();
    }

    void traceEntry(const std::string &perfId);
    double traceExit(const std::string &perfId);

    /**
     * @brief Register a global logger with the given header.
     * You should manually disable logging if you don't want to use it anymore.
     * @param logger_header: The header of the logger filename.
     * @param path: The path to the log file. default is the current directory.
     */
    void registerLogger(const std::string &path = "", const std::string &logger_header = "FR");
    void deRegisterLogger();
    RfLogListWidget *registerLogListWidget();
    void disableLogging();
    void enableStdoutLogging(bool enable);

    void trace(const std::string &msg);
    void info(const std::string &msg);
    void debug(const std::string &msg);
    void warn(const std::string &msg);
    void error(const std::string &msg);

private:
    LoggerImpl *impl_ = nullptr;
};

template <typename... Args>
using FormatString = fmt::format_string<Args...>;
// TODO: rename to RFLOG_TRACE
template <typename... Args>
static void trace(FormatString<Args...> fmt, Args &&...args)
{
    const std::string &str = fmt::format(fmt, std::forward<Args>(args)...);
    FRLog::instance().trace(str);
}

template <typename... Args>
static void info(FormatString<Args...> fmt, Args &&...args)
{
    const std::string &str = fmt::format(fmt, std::forward<Args>(args)...);
    FRLog::instance().info(str);
}

template <typename... Args>
static void debug(FormatString<Args...> fmt, Args &&...args)
{
    const std::string &str = fmt::format(fmt, std::forward<Args>(args)...);
    FRLog::instance().debug(str);
}

template <typename... Args>
static void warn(FormatString<Args...> fmt, Args &&...args)
{
    const std::string &str = fmt::format(fmt, std::forward<Args>(args)...);
    FRLog::instance().warn(str);
}

template <typename... Args>
static void error(FormatString<Args...> fmt, Args &&...args)
{
    const std::string &str = fmt::format(fmt, std::forward<Args>(args)...);
    FRLog::instance().error(str);
}

} // namespace rflog
#endif
