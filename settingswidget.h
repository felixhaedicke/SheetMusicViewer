#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>

#include "ui_settingswidget.h"

class SettingsWidget : public QWidget
{
    Q_OBJECT
    
public:
    Ui::SettingsWidget *ui;

    explicit SettingsWidget(QWidget *parent = 0);
    ~SettingsWidget();
    
};

#endif // SETTINGSWIDGET_H
