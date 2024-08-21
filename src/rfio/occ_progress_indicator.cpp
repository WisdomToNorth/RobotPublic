/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#include "occ_progress_indicator.h"

#include <algorithm>

#include "rfapp/task_progress.h"

namespace rfio
{

OccProgressIndicator::OccProgressIndicator(rfapp::TaskProgress *progress) : progress_(progress)
{
}

void OccProgressIndicator::Show(const Message_ProgressScope &scope, const bool isForce)
{
    if (progress_)
    {
        if (scope.Name() && (scope.Name() != last_step_name_ || isForce))
        {
            progress_->setStep(scope.Name());
            last_step_name_ = scope.Name();
        }

        const double pc = this->GetPosition(); // Always within [0,1]
        const int val = std::clamp(static_cast<int>(pc * 100), 0, 100);
        if (last_progress_ != val || isForce)
        {
            progress_->setValue(val);
            last_progress_ = val;
        }
    }
}

bool OccProgressIndicator::UserBreak()
{
    return rfapp::TaskProgress::isAbortRequested(progress_);
}

} // namespace rfio
