#ifndef RF_FRLOGENTRIES_H
#define RF_FRLOGENTRIES_H

#include <chrono>
#include <exception>
#include <functional>
#include <string>
#include <vector>

#include "rfLogEntryType.h"

namespace rflog
{
class LogEntry
{
private:
    std::chrono::system_clock::time_point timestamp_;
    LogEntryType type_;
    std::string message_;
    std::exception_ptr exception_;

public:
    LogEntry(LogEntryType type, const std::string &message, std::exception_ptr exception);

    LogEntryType getType() const;
    std::string getMessage() const;
    std::string toString() const;

private:
    std::string logEntryTypeToString(LogEntryType _type) const;
};

class LogEntries
{
private:
    std::vector<LogEntry> entries_;
    std::vector<std::function<void(LogEntryType, std::string)>> listeners_;

public:
    int getWarningCount() const;
    int getErrorCount() const;
    void clear();

    std::string getAsString() const;
    std::vector<std::string> get() const;
    std::vector<LogEntry> getEntries() const;

    void add(LogEntryType type, const std::string &message);
    void add(LogEntryType type, const std::string &message, std::exception_ptr exception);
    void addLogEntryAddedListener(std::function<void(LogEntryType, std::string)> listener);
};
} // namespace rflog
#endif
