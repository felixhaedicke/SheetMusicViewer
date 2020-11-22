#include <QApplication>
#include <QIcon>
#include <QProcess>
#include <QSettings>
#include <QString>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("SheetMusicViewer");
    QCoreApplication::setApplicationName("SheetMusicViewer");

    QSettings settings;
    QString customIconTheme = settings.value("customIconTheme").toString();
    if (!customIconTheme.isNull() && !customIconTheme.isEmpty())
    {
        QIcon::setThemeName(customIconTheme);
    }
    else
    {
        settings.setValue("customIconTheme", QString());
    }

    QString customStyle = settings.value("customStyle").toString();
    if (!customStyle.isNull() && !customStyle.isEmpty())
    {
        QApplication::setStyle(customStyle);
    }
    else
    {
        settings.setValue("customStyle", QString());
    }

    QString quitCommand = settings.value("quitCommand").toString();
    if (quitCommand.isNull() || quitCommand.isEmpty())
    {
        settings.setValue("quitCommand", QString());
    }
    else
    {
        QObject::connect(&a, &QApplication::lastWindowClosed, [&quitCommand]()
        {
            if ((QApplication::queryKeyboardModifiers() & Qt::ShiftModifier) == 0)
            {
                QProcess::execute(quitCommand);
            }
        });
    }

    MainWindow w;
    w.show();
    return a.exec();
}
