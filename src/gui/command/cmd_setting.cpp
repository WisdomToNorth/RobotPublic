#include "cmd_setting.h"

#include <iostream>

#include <QFontComboBox>
#include <QStringList>
#include <QTimer>

#include <magic_enum/magic_enum.hpp>

#include "generated/config.h"
#include "rfapp/app_settings.h"
#include "rfbase/datautils.h"
#include "rflog/rfLog.h"

#include "app_system.h"
#include "ui/form/listwidget.h"
#include "ui_cmd_setting.h"

namespace rfgui
{

CmdSetting::CmdSetting()
    : CmdBase(CmdType::Immediate)
{
    ui_ = new Ui::SettingWidgetUi();
    setting_dialog_ = new QDialog(GApp::instance().getMainWindow(), Qt::Tool);
    ui_->setupUi(setting_dialog_);
    setting_dialog_->setWindowTitle("Setting");
    setting_dialog_->setModal(true);

    initThemeList();
    initFontSizeList();

    QObject::connect(ui_->pb_apply, &QPushButton::clicked, this, &CmdSetting::onPbApplyClicked);
    QObject::connect(setting_dialog_, &QDialog::accepted, this, &CmdSetting::onPbApplyClicked);
}

void CmdSetting::initFontSizeList()
{
    QComboBox *cb_fontsize = ui_->cb_fontsize;
    cb_fontsize->clear();
    for (int i = 6; i <= 32; i++)
    {
        cb_fontsize->addItem(QString::number(i));
    }
}

void CmdSetting::initThemeList()
{
    std::ignore =
        rfbase::DataUtils::getAllFilesByDir(APP_THEME_PATH, ".qss", true, theme_file_list_);
    QStringList theme_name_list;
    for (const auto &filename : theme_file_list_)
    {
        theme_name_list.push_back(QString::fromStdString(filename));
    }
    ui_->cb_theme->addItems(theme_name_list);
}

void CmdSetting::onPbApplyClicked()
{
    rfapp::RfSettings *setting = GApp::instance().getSetting();
    rflog::debug("Apply setting");
    setting->setNewThemeFile(theme_file_list_[ui_->cb_theme->currentIndex()]);
    setting->setFont(ui_->cb_font->currentFont().family().toStdString(),
                     ui_->cb_fontsize->currentText().toInt());
    setting->apply();
}

void CmdSetting::initUIBySetting(rfapp::RfSettings *setting)
{
    const std::string &qss_name = setting->getThemeFileName();
    auto it = std::find(theme_file_list_.begin(), theme_file_list_.end(), qss_name);
    if (it != theme_file_list_.end())
    {
        int cur_index = static_cast<int>(std::distance(theme_file_list_.begin(), it));
        ui_->cb_theme->setCurrentIndex(cur_index);
    }
    else
    {
        rflog::warn("Can't find theme file {}", qss_name);
    }

    QFontComboBox *cb_font = ui_->cb_font;
    const auto &fontfamily = setting->getFontFamily();
    cb_font->setCurrentFont(QFont(fontfamily.c_str()));

    double fontSize = setting->getFontSize();
    // Round up to the nearest integer
    int fontSizeRounded = std::ceil(fontSize);
    // Ensure the font size is within the range of 6 to 72
    fontSizeRounded = std::clamp(fontSizeRounded, 6, 72);
    ui_->cb_fontsize->setCurrentText(QString::number(fontSizeRounded));
}

void CmdSetting::execute()
{
    initUIBySetting(GApp::instance().getSetting());
    setting_dialog_->setMinimumSize(730, 110);

    setting_dialog_->exec();
    this->done();
}

} // namespace rfgui
