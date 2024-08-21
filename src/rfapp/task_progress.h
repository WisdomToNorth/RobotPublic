#pragma once
#ifndef TASK_PROGESS_H
#define TASK_PROGESS_H
#include <atomic>
#include <string>
#include <string_view>

namespace rfapp
{
class TaskManager;

// Task identifier type
using TaskId = uint64_t;

// Reserved value for null tasks
constexpr TaskId KNullTaskId = UINT64_MAX;

// Syntactic sugar for task auto-deletion flag(see TaskManager::run/exec())
enum class TaskAutoDestroy
{
    On,
    Off
};

// Provides feedback on the progress of a running/executing task
class TaskProgress
{
public:
    TaskProgress() = default;
    TaskProgress(TaskProgress *parent, double portionSize, std::string_view step = {});
    ~TaskProgress();

    static TaskProgress &null();
    bool isNull() const;

    TaskId taskId() const
    {
        return task_id_;
    }

    // Value in [0,100]
    int value() const;
    void setValue(int pct);
    void setValue(double pct);

    const std::string &step() const;
    void setStep(std::string_view title);

    bool isRoot() const;
    const TaskProgress *parent() const;

    bool isAbortRequested() const;
    static bool isAbortRequested(const TaskProgress *progress);

    // Disable copy
    TaskProgress(const TaskProgress &) = delete;
    TaskProgress(TaskProgress &&) = delete;
    TaskProgress &operator=(const TaskProgress &) = delete;
    TaskProgress &operator=(TaskProgress &&) = delete;

private:
    friend class TaskManager;

    void setTaskId(TaskId id);
    void setTaskManager(TaskManager *mgr);
    void requestAbort();

    TaskProgress *parent_ = nullptr;
    TaskManager *task_mgr_ = nullptr;
    TaskId task_id_ = KNullTaskId;
    double portion_size_ = -1;
    std::atomic<int> value_ = 0;
    std::string step_;
    bool is_abort_requested_ = false;
};

} // namespace rfapp
#endif
