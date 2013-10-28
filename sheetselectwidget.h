#ifndef SHEETSELECTWIDGET_H
#define SHEETSELECTWIDGET_H

#include <QFileInfo>
#include <QMap>
#include <QPushButton>
#include <QString>
#include <QWidget>

class SheetSelectWidget : public QWidget
{
Q_OBJECT
public:
    explicit SheetSelectWidget(QWidget* parent);

private:
    bool twoColumns;
    int rowsCount;
    QFileInfoList pdfFiles;
    QMap<QPushButton*, QFileInfo> buttons;

protected:
    void resizeEvent(QResizeEvent* event);

signals:
    void sheetSelected(const QFileInfo& sheetFile);

private slots:
    void triggerSheetSelected();
};

#endif // SHEETSELECTWIDGET_H
