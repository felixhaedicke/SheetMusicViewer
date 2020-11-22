#include "mainwindow.h"

#include <QApplication>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    stackedLayout = new QStackedLayout(this);
    setLayout(stackedLayout);

    setWindowState(Qt::WindowFullScreen);

    QSettings settings;
    if (settings.value("hideCursor", false).toBool())
    {
        setCursor(Qt::BlankCursor);
    }
    else
    {
        settings.setValue("hideCursor", false);
    }

    sheetSelectWidget = new SheetSelectWidget(this);
    stackedLayout->addWidget(sheetSelectWidget);

    connect(sheetSelectWidget, &SheetSelectWidget::sheetSelected, this, &MainWindow::sheetSelected);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->modifiers() == Qt::ControlModifier)
    {
        if (event->key() == Qt::Key_C)
        {
            QApplication::exit(130);
            return;
        }
        else if (event->key() == Qt::Key_Q)
        {
            QApplication::quit();
            return;
        }
    }

    if (sheetsWidget && (stackedLayout->currentWidget() == sheetsWidget.get()))
    {
        switch (event->key())
        {
        case Qt::Key_Plus:
            sheetsWidget->zoomIn();
            break;

        case Qt::Key_Minus:
            sheetsWidget->zoomOut();
            break;

        case Qt::Key_Up:
            sheetsWidget->moveUp();
            break;

        case Qt::Key_Down:
            sheetsWidget->moveDown();
            break;

        case Qt::Key_Left:
            sheetsWidget->moveLeft();
            break;

        case Qt::Key_Right:
            sheetsWidget->moveRight();
            break;

        case Qt::Key_0:
            sheetsWidget->resetZoomAndOffset();
            break;

        case Qt::Key_PageUp:
        case Qt::Key_1:
            if (!event->isAutoRepeat()) sheetsWidget->previousPagesSet();
            break;

        case Qt::Key_PageDown:
        case Qt::Key_3:
            if (!event->isAutoRepeat()) sheetsWidget->nextPagesSet();
            break;

        case Qt::Key_Meta:
        case Qt::Key_2:
            if (!event->isAutoRepeat()) sheetsWidget->toPreviousMarkedPage();
            break;

        case Qt::Key_Home:
            sheetsWidget->toFirstPage();
            break;

        case Qt::Key_End:
            sheetsWidget->toLastPage();
            break;

        case Qt::Key_F1:
            sheetsWidget->setShownPagesCount(1);
            break;

        case Qt::Key_F2:
            sheetsWidget->setShownPagesCount(2);
            break;

        case Qt::Key_F3:
            sheetsWidget->setShownPagesCount(3);
            break;

        case Qt::Key_F4:
            sheetsWidget->setShownPagesCount(4);
            break;

        case Qt::Key_F5:
            sheetsWidget->setShownPagesCount(5);
            break;

        case Qt::Key_F6:
            sheetsWidget->setShownPagesCount(6);
            break;

        case Qt::Key_F7:
            sheetsWidget->setShownPagesCount(7);
            break;

        case Qt::Key_F8:
            sheetsWidget->setShownPagesCount(8);
            break;

        case Qt::Key_Space:
            stackedLayout->setCurrentWidget(sheetSelectWidget);
            break;

        case Qt::Key_Q:
            close();
            break;

        default:
            QWidget::keyPressEvent(event);
        }
    }
    else
    {
        QWidget::keyPressEvent(event);
    }
}

void MainWindow::sheetSelected(const QFileInfo& sheetFile)
{
    if (!sheetsWidget || (sheetsWidget->getDocumentFileInfo() != sheetFile))
    {
        if (sheetsWidget)
        {
            stackedLayout->removeWidget(sheetsWidget.get());
        }

        sheetsWidget = std::unique_ptr<SheetsWidget>(new SheetsWidget(this, sheetFile));
        stackedLayout->addWidget(sheetsWidget.get());

        connect(sheetsWidget.get(), &SheetsWidget::requestShowSheetSelection, this, &MainWindow::handleRequestShowSheetSelection);
        connect(sheetsWidget.get(), &SheetsWidget::requestExit, this, &QWidget::close);
    }

    stackedLayout->setCurrentWidget(sheetsWidget.get());
}

void MainWindow::handleRequestShowSheetSelection()
{
    stackedLayout->setCurrentWidget(sheetSelectWidget);
}
