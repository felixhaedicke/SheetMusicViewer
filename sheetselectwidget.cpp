#include "sheetselectwidget.h"

#include <QDir>
#include <QGridLayout>
#include <QPushButton>
#include <QSettings>

SheetSelectWidget::SheetSelectWidget(QWidget* parent) :
    QWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    QGridLayout* layout = new QGridLayout();
    this->setLayout(layout);

    QSettings settings;
#ifdef Q_OS_ANDROID
    QDir sheetsDir = settings.value("sheetsDir", "/sdcard/sheetMusic").toString();
#else
    QDir sheetsDir = settings.value("sheetsDir", QDir::home().filePath(".sheetMusic")).toString();
#endif

    QStringList pdfFilters;
    pdfFilters << "*.pdf";

    if (sheetsDir.exists())
    {
        pdfFiles = sheetsDir.entryInfoList(pdfFilters);
    }

    if (pdfFiles.count() > 0)
    {
        twoColumns = (pdfFiles.count() > 15);
        rowsCount = (twoColumns ? (pdfFiles.count() + 1) / 2 : pdfFiles.count());
        for (int i = 0; i < pdfFiles.count(); ++i)
        {
            bool secondCol = (twoColumns && (i >= rowsCount));
            QString baseName = pdfFiles.at(i).baseName();
            QPushButton* docButton = new QPushButton(baseName, this);
            docButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
            layout->addWidget(docButton, secondCol ? i - rowsCount : i, secondCol ? 1 : 0);
            buttons.insert(docButton, pdfFiles.at(i));
            connect(docButton, SIGNAL(clicked()), this, SLOT(triggerSheetSelected()));
        }
    }
}

void SheetSelectWidget::resizeEvent(QResizeEvent* event)
{
    int maxTextWidth = (twoColumns ? (width() / 2) : width()) * 3 / 4;

    QFont font;
    font.setPixelSize(50);
    QFontMetrics fontMetrics(font);

    for (int i = 0; i < pdfFiles.count(); ++i)
    {
        QString baseName = pdfFiles.at(i).baseName();
        while (fontMetrics.width(baseName) > maxTextWidth)
        {
            font.setPixelSize(font.pixelSize() - 2);
            fontMetrics = QFontMetrics(font);
        }
    }

    Q_FOREACH(QPushButton* docButton, buttons.keys())
    {
        docButton->setFont(font);
    }

    QWidget::resizeEvent(event);
}

void SheetSelectWidget::triggerSheetSelected()
{
    QPushButton* senderPushButton = dynamic_cast<QPushButton*>(sender());
    if (senderPushButton)
    {
        emit sheetSelected(buttons[dynamic_cast<QPushButton*>(sender())]);
    }
}
