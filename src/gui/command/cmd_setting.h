#pragma once
#ifndef COMMAND_SETTINGS_H
#define COMMAND_SETTINGS_H

#include <string>
#include <unordered_map>
#include <vector>

#include <QDialog>

#include "rfapp/app_settings.h"

#include "cmd_base.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class SettingWidgetUi;
}
QT_END_NAMESPACE

namespace rfgui
{
class CmdSetting : public CmdBase, public QObject
{
public:
    CmdSetting();
    ~CmdSetting() override = default;
    void initUIBySetting(rfapp::RfSettings *setting);

private:
    void execute() override;

private:
    void initThemeList();
    void initFontSizeList();

private slots:
    void onPbApplyClicked();

private:
    Ui::SettingWidgetUi *ui_;
    QDialog *setting_dialog_;

    std::vector<std::string> theme_file_list_;
};
} // namespace rfgui
#endif
