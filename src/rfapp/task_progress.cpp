/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#include "task_progress.h"

#include <algorithm>
#include <cmath>

#include "task_manager.h"

namespace rfapp
{

TaskProgress::TaskProgress(TaskProgress *parent, double portion_size, std::string_view step) :
    parent_(parent), task_id_(parent ? parent->task_id_ : KNullTaskId),
    portion_size_(std::clamp(portion_size, 0., 100.))
{
    if (!step.empty())
    {
        this->setStep(step);
    }
}

TaskProgress::~TaskProgress()
{
    if (parent_)
    {
        this->setValue(100);
    }
}

TaskProgress &TaskProgress::null()
{
    static TaskProgress null;
    return null;
}

bool TaskProgress::isNull() const
{
    return task_id_ == KNullTaskId;
}

int TaskProgress::value() const
{
    return value_;
}

void TaskProgress::setValue(int pct)
{
    if (task_id_ == KNullTaskId)
    {
        return;
    }

    if (is_abort_requested_)
    {
        return;
    }

    const int value_on_entry = value_;
    value_ = std::clamp(pct, 0, 100);
    if (value_ != 0 && value_ == value_on_entry)
    {
        return;
    }

    if (parent_)
    {
        const auto value_delta_in_perent =
            std::round((value_ - value_on_entry) * (portion_size_ / 100.));
        parent_->setValue(parent_->value() + value_delta_in_perent);
    }
    else
    {
        task_mgr_->onTaskProcessChanged(task_id_, value_);
    }
}

void TaskProgress::setValue(double pct)
{
    this->setValue(static_cast<int>(std::lround(pct)));
}

void TaskProgress::setStep(std::string_view title)
{
    if (task_mgr_ && task_id_ != KNullTaskId)
    {
        step_ = title;

        task_mgr_->onTaskProcessStep(task_id_, step_);
    }
}

const std::string &TaskProgress::step() const
{
    return step_;
}

bool TaskProgress::isRoot() const
{
    return parent_ != nullptr;
}

const TaskProgress *TaskProgress::parent() const
{
    return parent_;
}

bool TaskProgress::isAbortRequested() const
{
    return is_abort_requested_;
}

bool TaskProgress::isAbortRequested(const TaskProgress *progress)
{
    return progress ? progress->isAbortRequested() : false;
}

void TaskProgress::requestAbort()
{
    if (task_id_ != KNullTaskId)
    {
        is_abort_requested_ = true;
    }
}

void TaskProgress::setTaskId(TaskId id)
{
    task_id_ = id;
}
void TaskProgress::setTaskManager(TaskManager *mgr)
{
    task_mgr_ = mgr;
}
} // namespace rfapp
