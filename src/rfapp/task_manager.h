
#pragma once
#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H
#include <functional>
#include <string>
#include <string_view>

#include "task_progress.h"

namespace rfapp
{

// Piece of code to be executed as a task(ie with TaskManager::run/exec())
using TaskJob = std::function<void(TaskProgress *)>;

// Central class providing creation/execution/deletion of Task objects
class TaskManager
{
public:
    // Ctor & dtor
    TaskManager();
    ~TaskManager();

    // Not copyable
    TaskManager(const TaskManager &) = delete;
    TaskManager &operator=(const TaskManager &) = delete;

    // Allocates a new task entity in manager
    // Returns the task identifier(unique in the scope of the owning TaskManager)
    TaskId newTask(TaskJob fn);

    // Asynchronous execution of job associated with task identifier 'id'
    // By default destroy policy is set to 'On' meaning the task will be
    // deleted at some point after its completion
    // This function is based on std::async() so depending on the C++ stdlib implementation this is
    // probably using a thread pool
    // NOTE The task must have been allocated previously with newTask()
    void run(TaskId id, TaskAutoDestroy policy = TaskAutoDestroy::On);

    // Same as run() but execution of the task job is synchronous(it runs in the current thread
    // just like a regular function call)
    void exec(TaskId id, TaskAutoDestroy policy = TaskAutoDestroy::On);

    // Current progress of task identified by 'id'
    // NOTE The task must have been allocated previously with newTask()
    int progress(TaskId id) const;

    // Current progress of all tasks
    int globalProgress() const;

    // Title(description) of a task identified by 'id'
    std::string title(TaskId id) const;
    void setTitle(TaskId id, std::string_view title);

    // Blocks the current thread until task of identifier 'id' has finished
    bool waitForDone(TaskId id, int msecs = -1);

    // Instructs the task of identifier 'id' to abort as soon as possible
    // Task interruption relies on the task job for this: it has to check regularly the
    // TaskProgress::isAbortRequested() flag and interrupt consequently
    void requestAbort(TaskId id);

    // Applies function 'fn' to each task
    void foreachTask(const std::function<void(TaskId)> &fn);

public:
    /*bind singals*/
    // KDBindings::Signal<TaskId> signal_started_;
    void bindTaskStartFunc(std::function<void(TaskId)> func);
    void onTaskStart(TaskId id);
    // KDBindings::Signal<TaskId, const std::string &> signal_progress_step_;
    void bindTaskProcessStepFunc(std::function<void(TaskId)> func);
    void onTaskProcessStep(TaskId id, const std::string &step);

    // KDBindings::Signal<TaskId, int> signal_progress_changed_;
    void bindTaskProcessChangedFunc(std::function<void(TaskId)> func);
    void onTaskProcessChanged(TaskId id, int value);

    // KDBindings::Signal<TaskId> signal_abort_requested_;
    void bindTaskAbortFunc(std::function<void(TaskId)> func);
    void onTaskAbort(TaskId id);

    // KDBindings::Signal<TaskId> signal_ended_;
    void bindTaskEndFunc(std::function<void(TaskId)> func);
    void onTaskEnd(TaskId id);

private:
    struct Entity;
    struct Private;
    Private *impl_ = nullptr;
};

} // namespace rfapp
#endif
