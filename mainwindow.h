#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGridLayout>
#include <QStackedLayout>
#include <QString>
#include <QWidget>

#include "sheetswidget.h"
#include "sheetselectwidget.h"

class MainWindow : public QWidget
{
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    
protected:
    void keyPressEvent(QKeyEvent* event);

private:
    QStackedLayout* stackedLayout;
    SheetsWidget* sheetsWidget;
    SheetSelectWidget* sheetSelectWidget;

private slots:
    void sheetSelected(const QFileInfo& sheetFile);
    void handleRequestShowSheetSelection();

};

#endif // MAINWINDOW_H
