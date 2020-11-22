#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <memory>

#include <QWidget>

#include "ui_settingswidget.h"

class SettingsWidget : public QWidget
{
    Q_OBJECT
    
public:
    std::unique_ptr<Ui::SettingsWidget> ui;

    explicit SettingsWidget(QWidget* parent = nullptr);
    
};

#endif // SETTINGSWIDGET_H
