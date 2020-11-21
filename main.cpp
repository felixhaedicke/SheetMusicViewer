#include <QApplication>
#include <QIcon>
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

    MainWindow w;
    w.show();
    return a.exec();
}
