#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>

namespace Ui {
    class ControlWidget;
}

class ControlWidget : public QWidget {
    Q_OBJECT
public:
    ControlWidget(QWidget *parent = 0);
    ~ControlWidget();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ControlWidget *ui;
};

#endif // CONTROLWIDGET_H
