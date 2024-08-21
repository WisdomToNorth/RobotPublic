/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#pragma once
#ifndef RFIO_OCC_PROGRESS_INDICATOR_H
#define RFIO_OCC_PROGRESS_INDICATOR_H

#include <Message_ProgressIndicator.hxx>

namespace rfapp
{
class TaskProgress;
}

namespace rfio
{

// Provides implementation of OpenCascade-based progress indicator around TaskProgress
class OccProgressIndicator : public Message_ProgressIndicator
{
public:
    OccProgressIndicator(rfapp::TaskProgress *progress);

    bool UserBreak() override;

    void Show(const Message_ProgressScope &theScope, const bool isForce) override;

private:
    rfapp::TaskProgress *progress_ = nullptr;
    const char *last_step_name_ = nullptr;
    int last_progress_ = -1;
};

} // namespace rfio
#endif
