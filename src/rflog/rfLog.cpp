#include "rfLog.h"

#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <unordered_map>

#include "rfLogEntries.h"
#include "rfLogEntryType.h"
#include "rfLogListWidget.h"

namespace rflog
{
namespace
{
static std::string getTimeStr(const char *format /* = "%Y-%m-%d_%H-%M-%S"*/)
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), format);
    return ss.str();
}
} // namespace

struct LoggerImpl
{
    std::string logger_name_ = "FR";
    bool enabled_ = true;
    LogEntries log_entries_;
    std::shared_ptr<spdlog::logger> logger_ = nullptr;
    std::unordered_map<size_t, std::chrono::system_clock::time_point> perf_data_;
    RfLogListWidget *log_list_widget_ = nullptr;
};

FRLog::FRLog()
{
    impl_ = new LoggerImpl();
}

FRLog::~FRLog()
{
    delete impl_->log_list_widget_;
    delete impl_;
}

void FRLog::registerLogger(const std::string &path, const std::string &logger_header)
{
    // Check if a logger with a different name exists and de-register it
    deRegisterLogger();

    const std::string &time_str = getTimeStr("%Y-%m-%d_%H-%M-%S");
    const std::string &log_name = logger_header + time_str;
    impl_->logger_name_ = log_name;

    // Create a multi sink logger
    std::vector<spdlog::sink_ptr> sinks;
    // console sink
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    // file sink
    const std::string &log_file_name = path + log_name + ".log";
    sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file_name, true));

    impl_->logger_ = std::make_shared<spdlog::logger>(log_name, begin(sinks), end(sinks));

    spdlog::register_logger(impl_->logger_);
    spdlog::set_default_logger(impl_->logger_);

    impl_->logger_->flush_on(spdlog::level::info);
    impl_->logger_->set_level(spdlog::level::trace);
    impl_->logger_->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
    impl_->logger_->info("####### Logger initialized #######");
}

RfLogListWidget *FRLog::registerLogListWidget()
{
    if (!impl_->log_list_widget_)
    {
        impl_->log_list_widget_ = new RfLogListWidget();

        impl_->log_entries_.addLogEntryAddedListener(
            [this](LogEntryType type, std::string message)
            { impl_->log_list_widget_->addLogEntry(type, message); });
    }

    return impl_->log_list_widget_;
}

void FRLog::deRegisterLogger()
{
    if (impl_->logger_)
    {
        impl_->logger_->info("####### Logger de-initialized #######");
        spdlog::drop(impl_->logger_name_);
        impl_->logger_ = nullptr;
    }
}

void FRLog::disableLogging()
{
    impl_->enabled_ = false;
}

void FRLog::enableStdoutLogging(bool)
{
    // TODO: forbid or enable stdout logging
}

std::string FRLog::formatDuration(double totalSeconds)
{
    double seconds = std::fmod(totalSeconds, 60);
    double minutes = std::fmod(totalSeconds / 60, 60);
    double hours = totalSeconds / 3600;

    std::stringstream ss;
    if (hours > 0)
        ss << static_cast<int>(hours) << " hour(s) ";
    if (minutes > 0)
        ss << static_cast<int>(minutes) << " minute(s) ";
    ss << std::fixed << std::setprecision(2) << seconds << " seconds";

    return ss.str();
}

void FRLog::traceEntry(const std::string &perfId)
{
    if (!impl_->enabled_)
    {
        return;
    }

    impl_->perf_data_[std::hash<std::string>{}(perfId)] = std::chrono::system_clock::now();
    this->trace("Method '" + perfId + "' started.");
}

double FRLog::traceExit(const std::string &perfId)
{
    if (!impl_->enabled_)
    {
        return 0.0;
    }

    if (impl_->perf_data_.find(std::hash<std::string>{}(perfId)) == impl_->perf_data_.end())
    {
        this->error("Method '" + perfId + "' was not started.");
        return 0.0;
    }

    auto start = impl_->perf_data_[std::hash<std::string>{}(perfId)];
    impl_->perf_data_.erase(std::hash<std::string>{}(perfId));

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    std::string logMessage = "Method '" + perfId + "' was performed in " +
                             FRLog::formatDuration(elapsed_seconds.count()) + ".";
    this->trace(logMessage);

    return elapsed_seconds.count();
}

void FRLog::info(const std::string &msg)
{
    if (!impl_->enabled_)
    {
        return;
    }

    impl_->log_entries_.add(LogEntryType::Info, msg);
    if (!impl_->logger_)
    {
        std::cout << "[Uninited Logger]: " << msg << '\n';
        return;
    }

    impl_->logger_->info(msg);
}

void FRLog::warn(const std::string &msg)
{
    if (!impl_->enabled_)
    {
        return;
    }

    impl_->log_entries_.add(LogEntryType::Warning, msg);
    if (!impl_->logger_)
    {
        std::cout << "[Uninited Logger]: " << msg << '\n';
        return;
    }

    impl_->logger_->warn(msg);
}

void FRLog::debug(const std::string &msg)
{
    if (!impl_->enabled_)
    {
        return;
    }

    impl_->log_entries_.add(LogEntryType::Debug, msg);
    if (!impl_->logger_)
    {
        std::cout << "[Uninited Logger]: " << msg << '\n';
        return;
    }

    impl_->logger_->debug(msg);
}

void FRLog::error(const std::string &msg)
{
    if (!impl_->enabled_)
    {
        return;
    }

    impl_->log_entries_.add(LogEntryType::Error, msg);
    if (!impl_->logger_)
    {
        std::cout << "[Uninited Logger]: " << msg << '\n';
        return;
    }

    impl_->logger_->error(msg);
}

void FRLog::trace(const std::string &msg)
{
    if (!impl_->enabled_)
    {
        return;
    }

    impl_->log_entries_.add(LogEntryType::Trace, msg);
    if (!impl_->logger_)
    {
        std::cout << "[Uninited Logger]: " << msg << '\n';
        return;
    }

    impl_->logger_->trace(msg);
}
}; // namespace rflog
