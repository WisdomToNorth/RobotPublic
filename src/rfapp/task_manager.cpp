#include "task_manager.h"

#include <atomic>
#include <cassert>
#include <cmath>
#include <future>
#include <memory>
#include <unordered_map>

#include <kdbindings/signal.h>

#include "rfbase/rfmath.h"

#include "task_progress.h"

namespace rfapp
{

// Helper struct providing all required data to manage a Task object
struct TaskManager::Entity
{
    TaskId task_id_;
    TaskJob task_job_;
    TaskProgress task_progress_;
    std::string title_;
    std::future<void> control_;
    std::atomic<bool> is_finished_ = false;
    TaskAutoDestroy auto_destroy_ = TaskAutoDestroy::On;
};

// Pimpl struct providing private(hidden) interface of TaskManager class
struct TaskManager::Private
{
public:
    // Ctor
    Private(TaskManager *mgr)
        : taskMgr(mgr)
    {
    }

    // Const/mutable functions to find an Entity from a task identifier. Returns null if not found
    TaskManager::Entity *findEntity(TaskId id);
    const TaskManager::Entity *findEntity(TaskId id) const;

    // Execute(synchronous) task entity, sending started/ended signals accordingly
    void execEntity(TaskManager::Entity *entity);

    // Destroy finished task entities whose policy was set to TaskAutoDestroy::On
    void cleanGarbage();

    TaskManager *taskMgr = nullptr;
    std::atomic<TaskId> taskIdSeq = {};
    std::unordered_map<TaskId, std::unique_ptr<TaskManager::Entity>> mapEntity;

    /*Signals*/
    // Signal emitted when some task execution has just started
    KDBindings::Signal<TaskId> signal_started_;

    // Signal emitted when the current step description of some task has changed
    KDBindings::Signal<TaskId, const std::string &> signal_progress_step_;

    // Signal emitted when the current progress of some task has changed
    KDBindings::Signal<TaskId, int> signal_progress_changed_;

    // Signal emitted when requestAbort() was called on some task
    KDBindings::Signal<TaskId> signal_abort_requested_;

    // Signal emitted when some task execution has just ended(whatever stop condition: finished
    // or aborted)
    KDBindings::Signal<TaskId> signal_ended_;
};

TaskManager::TaskManager()
{
    impl_ = new Private(this);
}

TaskManager::~TaskManager()
{
    // Make sure all tasks are really finished
    for (const auto &mapPair : impl_->mapEntity)
    {
        const std::unique_ptr<Entity> &ptrEntity = mapPair.second;
        if (ptrEntity->control_.valid())
        {
            ptrEntity->control_.wait();
        }
    }

    // Erase the task from its container before destruction, this will allow TaskProgress destructor
    // to behave correctly(it calls TaskProgress::setValue())
    for (auto it = impl_->mapEntity.begin(); it != impl_->mapEntity.end();)
    {
        it = impl_->mapEntity.erase(it);
    }

    delete impl_;
}

TaskId TaskManager::newTask(TaskJob fn)
{
    const TaskId taskId = impl_->taskIdSeq.fetch_add(1);
    std::unique_ptr<Entity> ptrEntity(new Entity);
    ptrEntity->task_id_ = taskId;
    ptrEntity->task_job_ = std::move(fn);
    ptrEntity->task_progress_.setTaskId(taskId);
    ptrEntity->task_progress_.setTaskManager(this);
    impl_->mapEntity.insert({taskId, std::move(ptrEntity)});
    return taskId;
}

void TaskManager::run(TaskId id, TaskAutoDestroy policy)
{
    impl_->cleanGarbage();
    Entity *entity = impl_->findEntity(id);
    if (!entity)
    {
        return;
    }

    entity->is_finished_ = false;
    entity->auto_destroy_ = policy;
    entity->control_ = std::async([this, entity] { impl_->execEntity(entity); });
}

void TaskManager::exec(TaskId id, TaskAutoDestroy policy)
{
    impl_->cleanGarbage();
    Entity *entity = impl_->findEntity(id);
    if (!entity)
    {
        return;
    }

    entity->is_finished_ = false;
    entity->auto_destroy_ = policy;
    impl_->execEntity(entity);
}

bool TaskManager::waitForDone(TaskId id, int msecs)
{
    Entity *entity = impl_->findEntity(id);
    if (!entity)
    {
        return true;
    }

    if (!entity->control_.valid())
    {
        return true;
    }

    if (msecs < 0)
    {
        entity->control_.wait();
        return true;
    }

    return entity->control_.wait_for(std::chrono::milliseconds(msecs)) == std::future_status::ready;
}

void TaskManager::requestAbort(TaskId id)
{
    Entity *entity = impl_->findEntity(id);
    if (entity)
    {
        this->impl_->signal_abort_requested_.emit(id);
        entity->task_progress_.requestAbort();
    }
}

void TaskManager::foreachTask(const std::function<void(TaskId)> &fn)
{
    for (const auto &mapPair : impl_->mapEntity)
    {
        fn(mapPair.first);
    }
}

int TaskManager::progress(TaskId id) const
{
    const Entity *entity = impl_->findEntity(id);
    return entity ? entity->task_progress_.value() : 0;
}

int TaskManager::globalProgress() const
{
    int taskAccumPct = 0;
    for (const auto &mapPair : impl_->mapEntity)
    {
        const std::unique_ptr<Entity> &ptrEntity = mapPair.second;
        if (ptrEntity->task_progress_.value() > 0)
        {
            taskAccumPct += ptrEntity->task_progress_.value();
        }
    }

    const auto pct = rfbase::toPercent(taskAccumPct, 0, impl_->mapEntity.size() * 100);
    return static_cast<int>(std::lround(pct));
}

std::string TaskManager::title(TaskId id) const
{
    const Entity *entity = impl_->findEntity(id);
    return entity ? entity->title_ : std::string();
}

void TaskManager::setTitle(TaskId id, std::string_view title)
{
    Entity *entity = impl_->findEntity(id);
    if (entity)
    {
        entity->title_ = title;
    }
}

// KDBindings::Signal<TaskId> signal_started_;
void TaskManager::bindTaskStartFunc(std::function<void(TaskId)> func)
{
    this->impl_->signal_started_.connect(func);
}
void TaskManager::onTaskStart(TaskId id)
{
    this->impl_->signal_started_.emit(id);
}
// KDBindings::Signal<TaskId, const std::string &> signal_progress_step_;
void TaskManager::bindTaskProcessStepFunc(std::function<void(TaskId)> func)
{
    this->impl_->signal_progress_step_.connect(func);
}
void TaskManager::onTaskProcessStep(TaskId id, const std::string &step)
{
    this->impl_->signal_progress_step_.emit(id, step);
}

// KDBindings::Signal<TaskId, int> signal_progress_changed_;
void TaskManager::bindTaskProcessChangedFunc(std::function<void(TaskId)> func)
{
    this->impl_->signal_progress_changed_.connect(func);
}
void TaskManager::onTaskProcessChanged(TaskId id, int value)
{
    this->impl_->signal_progress_changed_.emit(id, value);
}

// KDBindings::Signal<TaskId> signal_abort_requested_;
void TaskManager::bindTaskAbortFunc(std::function<void(TaskId)> func)
{
    this->impl_->signal_abort_requested_.connect(func);
}
void TaskManager::onTaskAbort(TaskId id)
{
    this->impl_->signal_abort_requested_.emit(id);
}

// KDBindings::Signal<TaskId> signal_ended_;
void TaskManager::bindTaskEndFunc(std::function<void(TaskId)> func)
{
    this->impl_->signal_ended_.connect(func);
}
void TaskManager::onTaskEnd(TaskId id)
{
    this->impl_->signal_ended_.emit(id);
}

TaskManager::Entity *TaskManager::Private::findEntity(TaskId id)
{
    auto it = this->mapEntity.find(id);
    return it != this->mapEntity.end() ? it->second.get() : nullptr;
}

const TaskManager::Entity *TaskManager::Private::findEntity(TaskId id) const
{
    auto it = this->mapEntity.find(id);
    return it != this->mapEntity.cend() ? it->second.get() : nullptr;
}

void TaskManager::Private::execEntity(Entity *entity)
{
    if (!entity)
    {
        return;
    }

    this->signal_started_.emit(entity->task_id_);
    const TaskJob &fn = entity->task_job_;
    fn(&entity->task_progress_);
    if (!entity->task_progress_.isAbortRequested())
    {
        entity->task_progress_.setValue(100);
    }

    this->signal_ended_.emit(entity->task_id_);
    entity->is_finished_ = true;
}

void TaskManager::Private::cleanGarbage()
{
    auto it = this->mapEntity.begin();
    while (it != this->mapEntity.end())
    {
        Entity *entity = it->second.get();
        if (entity->is_finished_ && entity->auto_destroy_ == TaskAutoDestroy::On)
        {
            if (entity->control_.valid())
            {
                entity->control_.wait();
            }
            it = this->mapEntity.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
} // namespace rfapp
