#ifndef CONTROL_DIALOGS_SETTING_DIALOG_H
#define CONTROL_DIALOGS_SETTING_DIALOG_H

#include "control/models/SettingsModel.h"

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>
#include <QDialogButtonBox>

class SettingsDialog : public QDialog
{
    Q_OBJECT

    SettingsModel* m_settings;

public:
    explicit SettingsDialog(SettingsModel* settings, QWidget* parent = nullptr);

private slots:
    void onRefreshPortsButtonPressed();

    void updateFeedRateRanges(int _);
    void updateSpindleRpmRanges(int _);

    void onOkButtonPressed();

private:
    void createUi();

    // UI members
    QComboBox* m_portComboBox;
    QPushButton* m_refreshPortsButton;
    QComboBox* m_baudRateComboBox;

    QSpinBox* m_minimumFeedRateInMmPerMinSpinBox;
    QSpinBox* m_maximumFeedRateInMmPerMinSpinBox;
    QSpinBox* m_defaultFeedRateInMmPerMinSpinBox;

    QSpinBox* m_minimumSpindleRpmSpinBox;
    QSpinBox* m_maximumSpindleRpmSpinBox;
    QSpinBox* m_defaultSpindleRpmSpinBox;

    QSpinBox* m_xCncSizeInMmSpinBox;
    QSpinBox* m_yCncSizeInMmSpinBox;
    QSpinBox* m_zCncSizeInMmSpinBox;

    QDialogButtonBox* m_okCancelButtonBox;
};

#endif
