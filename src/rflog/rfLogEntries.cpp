#include "rfLogEntries.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

namespace rflog
{
namespace
{
static std::string getTimeStr(const std::chrono::system_clock::time_point &time_point,
                              const char *format /* = "%Y-%m-%d_%H-%M-%S"*/)
{
    auto now_c = std::chrono::system_clock::to_time_t(time_point);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), format);
    return ss.str();
}

std::string getTimeStrWithBrackets(const std::chrono::system_clock::time_point &time_point)
{
    return "[" + getTimeStr(time_point, "%Y-%m-%d %H:%M:%S") + "]";
}
} // namespace

LogEntry::LogEntry(LogEntryType type, const std::string &message, std::exception_ptr exception)
    : timestamp_(std::chrono::system_clock::now())
    , type_(type)
    , message_(message)
    , exception_(exception)
{
}

LogEntryType LogEntry::getType() const
{
    return this->type_;
}

std::string LogEntry::getMessage() const
{
    return this->message_;
}

std::string LogEntry::toString() const
{
    std::string typeStr = logEntryTypeToString(this->type_);
    std::string timeStr = getTimeStrWithBrackets(this->timestamp_);
    return timeStr + typeStr + this->message_;
}

std::string LogEntry::logEntryTypeToString(LogEntryType _type) const
{
    switch (_type)
    {
    case LogEntryType::Error: return "[Error] ";
    case LogEntryType::Warning: return "[Warning] ";
    case LogEntryType::Info: return "[Info] ";
    case LogEntryType::Trace: return "[Trace] ";
    case LogEntryType::Debug: return "[Debug] ";
    }
    return "";
}

int LogEntries::getWarningCount() const
{
    auto cnt = std::count_if(entries_.begin(), entries_.end(), [](const LogEntry &entry)
                             { return entry.getType() == LogEntryType::Warning; });
    return static_cast<int>(cnt);
}

int LogEntries::getErrorCount() const
{
    auto cnt = std::count_if(entries_.begin(), entries_.end(), [](const LogEntry &entry)
                             { return entry.getType() == LogEntryType::Error; });
    return static_cast<int>(cnt);
}

void LogEntries::clear()
{
    entries_.clear();
}

std::string LogEntries::getAsString() const
{
    std::string result;
    for (const auto &entry : entries_)
    {
        result += entry.toString() + "\n";
    }
    return result;
}

std::vector<std::string> LogEntries::get() const
{
    std::vector<std::string> result;
    for (const auto &entry : entries_)
    {
        result.push_back(entry.toString());
    }
    return result;
}

std::vector<LogEntry> LogEntries::getEntries() const
{
    return entries_;
}

void LogEntries::add(LogEntryType type, const std::string &message)
{
    add(type, message, nullptr);
}

void LogEntries::add(LogEntryType type, const std::string &message, std::exception_ptr exception)
{
    entries_.emplace_back(type, message, exception);

    const auto &latest_entry = entries_.back();
    for (const auto &listener : listeners_)
    {
        listener(latest_entry.getType(), latest_entry.toString());
    }
}

void LogEntries::addLogEntryAddedListener(std::function<void(LogEntryType, std::string)> listener)
{
    listeners_.push_back(listener);
}
} // namespace rflog
