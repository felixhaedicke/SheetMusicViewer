#include "sheetswidget.h"

#include <QBitmap>
#include <QDateTime>
#include <QKeyEvent>
#include <QMutexLocker>
#include <QPalette>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QPushButton>
#include <QResizeEvent>
#include <QSettings>
#include <QTimer>

#include <math.h>

SheetsWidget::SheetsWidget(QWidget* parent, const QFileInfo& documentFileInfo) :
    QWidget(parent),
    documentFileInfo(documentFileInfo),
    documentBaseName(documentFileInfo.baseName()),
    rowsCount(1),
    columnsCount(2),
    firstRenderedPageNo(0),
    scaleFactor(1.0),
    customXOffset(0),
    customYOffset(0),
    lastMarkOrUnmarkTime(0),
    lastMousePressPageNo(0)
{
    QSettings settings;

    pagesRendererWorker = new QThreadPool(this);
    pagesRendererWorker->setMaxThreadCount(1);

    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    setMouseTracking(true);
    installEventFilter(this);

    stackedLayout = new QStackedLayout(this);
    setLayout(stackedLayout);

    settingsWidget = new SettingsWidget(this);
    settingsWidget->setMouseTracking(true);
    stackedLayout->addWidget(settingsWidget);
    settingsWidget->hide();

    connect(settingsWidget->ui->selectSheetButton, SIGNAL(clicked()), this, SIGNAL(requestShowSheetSelection()));
    connect(settingsWidget->ui->restoreLayoutButton, SIGNAL(clicked()), this, SLOT(resetZoomAndOffset()));
    connect(settingsWidget->ui->zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(settingsWidget->ui->zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));
    connect(settingsWidget->ui->upButton, SIGNAL(clicked()), this, SLOT(moveUp()));
    connect(settingsWidget->ui->downButton, SIGNAL(clicked()), this, SLOT(moveDown()));
    connect(settingsWidget->ui->leftButton, SIGNAL(clicked()), this, SLOT(moveLeft()));
    connect(settingsWidget->ui->rightButton, SIGNAL(clicked()), this, SLOT(moveRight()));
    connect(settingsWidget->ui->increasePagesCountButton, SIGNAL(clicked()), this, SLOT(increaseShownPagesCount()));
    connect(settingsWidget->ui->decreasePagesCountButton, SIGNAL(clicked()), this, SLOT(decreaseShownPagesCount()));
    connect(settingsWidget->ui->previousPageButton, SIGNAL(clicked()), this, SLOT(previousPagesSet()));
    connect(settingsWidget->ui->nextPageButton, SIGNAL(clicked()), this, SLOT(nextPagesSet()));
    connect(settingsWidget->ui->previousMarkedPageButton, SIGNAL(clicked()), this, SLOT(toPreviousMarkedPage()));
    connect(settingsWidget->ui->firstPageButton, SIGNAL(clicked()), this, SLOT(toFirstPage()));
    connect(settingsWidget->ui->markButton, SIGNAL(clicked()), this, SLOT(markOrUnmarkLastClickedPage()));

    QPushButton* quitButton = settingsWidget->ui->quitButton;
    connect(quitButton, SIGNAL(clicked()), this, SIGNAL(requestExit()));
    if (settings.value("hideQuitButton", false).toBool())
    {
        quitButton->setVisible(false);
    }
    else
    {
        settings.setValue("hideQuitButton", false);
    }

    QPalette pal(palette());
    pal.setColor(QPalette::Window, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);

    longClickTimer = new QTimer(this);
    longClickTimer->setSingleShot(true);
    longClickTimer->setInterval(400);
    connect(longClickTimer, SIGNAL(timeout()), settingsWidget, SLOT(show()));
    connect(longClickTimer, SIGNAL(timeout()), this, SLOT(update()));

    settingsControlsInactivityTimer = new QTimer(this);
    settingsControlsInactivityTimer->setSingleShot(true);
    settingsControlsInactivityTimer->setInterval(5000);
    connect(settingsControlsInactivityTimer, SIGNAL(timeout()), settingsWidget, SLOT(hide()));
    connect(settingsControlsInactivityTimer, SIGNAL(timeout()), this, SLOT(update()));

    connect(longClickTimer, SIGNAL(timeout()), settingsControlsInactivityTimer, SLOT(start()));

    for (QPushButton* settingsWidgetButton : settingsWidget->findChildren<QPushButton*>())
    {
        settingsWidgetButton->setMouseTracking(true);
        settingsWidgetButton->installEventFilter(this);
        connect(settingsWidgetButton, SIGNAL(pressed()), settingsControlsInactivityTimer, SLOT(stop()));
        connect(settingsWidgetButton, SIGNAL(released()), settingsControlsInactivityTimer, SLOT(start()));
    }

    document = std::unique_ptr<PdfDocument>(new PdfDocument(documentFileInfo.absoluteFilePath()));
    int pagesCount = document->pagesCount();
    if (pagesCount < 1)
    {
        document.reset();
    }

    if (document)
    {
        documentPagesImages = std::unique_ptr<QImage[]>(new QImage[pagesCount]);
        documentPagesPixmaps = std::unique_ptr<QPixmap[]>(new QPixmap[pagesCount]);
    }

    settings.beginGroup(documentBaseName);
    setShownPagesCount(settings.value("shownPagesCount", 2).toInt(), false);
    markedPageNumbers = deserializeIntSet(settings.value("markedPageNumbers", QString("")).toString());
    settings.beginGroup(getLayoutSettingGroupName());
    setScaleFactor(settings.value("scaleFactor", 1.0).toDouble(), false);
    setCustomXOffset(settings.value("customXOffset", 0).toInt(), false);
    setCustomYOffset(settings.value("customYOffset", 0).toInt(), false);
    settings.endGroup();
    settings.endGroup();

    reRenderPixmaps();
}

SheetsWidget::~SheetsWidget()
{
    if (pagesRenderer)
    {
        pagesRenderer->stopAndWait();
    }
}

void SheetsWidget::decreaseShownPagesCount()
{
    switch (rowsCount * columnsCount)
    {
    case 2:
        setShownPagesCount(1);
        break;

    case 3:
        setShownPagesCount(2);
        break;

    case 6:
        setShownPagesCount(3);
        break;

    case 8:
        setShownPagesCount(6);
        break;
    }
}

void SheetsWidget::increaseShownPagesCount()
{
    switch (rowsCount * columnsCount)
    {
    case 1:
        setShownPagesCount(2);
        break;

    case 2:
        setShownPagesCount(3);
        break;

    case 3:
        setShownPagesCount(6);
        break;

    case 6:
        setShownPagesCount(8);
        break;
    }
}

void SheetsWidget::zoomIn()
{
    setScaleFactor(scaleFactor * 1.01);
}

void SheetsWidget::zoomOut()
{
    setScaleFactor(scaleFactor / 1.01);
}

void SheetsWidget::moveLeft()
{
    setCustomXOffset(customXOffset - 2);
}

void SheetsWidget::moveRight()
{
    setCustomXOffset(customXOffset + 2);
}

void SheetsWidget::moveUp()
{
    setCustomYOffset(customYOffset - 2);
}

void SheetsWidget::moveDown()
{
    setCustomYOffset(customYOffset + 2);
}

void SheetsWidget::resetZoomAndOffset()
{
    setScaleFactor(1);
    setCustomXOffset(0);
    setCustomYOffset(0);
}

void SheetsWidget::nextPagesSet()
{
    if (document)
    {
        int pagesCount = document->pagesCount();
        if ((firstRenderedPageNo + columnsCount * rowsCount) < pagesCount)
        {
            firstRenderedPageNo = qMin(firstRenderedPageNo + columnsCount * rowsCount, pagesCount - columnsCount * rowsCount);
            update();
        }
    }
}

void SheetsWidget::previousPagesSet()
{
    if (firstRenderedPageNo > 0)
    {
        firstRenderedPageNo = qMax(firstRenderedPageNo - columnsCount * rowsCount, 0);
        update();
    }
}

void SheetsWidget::toPreviousMarkedPage()
{
    if (document && (firstRenderedPageNo > 0))
    {
        bool found = false;
        for (int i = firstRenderedPageNo - 1; !found && (i >= 0); --i)
        {
            if (markedPageNumbers.contains(i))
            {
                firstRenderedPageNo = i;
                found = true;
            }
        }
        if (found)
        {
            update();
        }
    }
}

void SheetsWidget::toFirstPage()
{
    if (firstRenderedPageNo > 0)
    {
        firstRenderedPageNo = 0;
        update();
    }
}

void SheetsWidget::toLastPage()
{
    if (document)
    {
        firstRenderedPageNo = qMax(document->pagesCount() - columnsCount * rowsCount, 0);
        update();
    }
}

void SheetsWidget::markOrUnmarkLastClickedPage()
{
    markOrUnmarkPage(lastMousePressPageNo);
}

bool SheetsWidget::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::MouseMove:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
        settingsControlsInactivityTimer->start();
        break;

    default:
        break;
    }

    return QWidget::eventFilter(obj, event);
}

void SheetsWidget::mousePressEvent(QMouseEvent* event)
{
    if (!settingsWidget->isVisible())
    {
        longClickTimer->start();
    }

    int prevLastMousePressPageNo = lastMousePressPageNo;
    lastMousePressPageNo = firstRenderedPageNo
            + static_cast<int>(static_cast<double>(event->x()) * static_cast<double>(columnsCount) / static_cast<double>(width()))
            + (static_cast<int>(static_cast<double>(event->y()) * static_cast<double>(rowsCount) / static_cast<double>(height())) * columnsCount);
    if (prevLastMousePressPageNo != lastMousePressPageNo)
    {
        update();
    }

    QWidget::mousePressEvent(event);
}

void SheetsWidget::mouseReleaseEvent(QMouseEvent* event)
{
    longClickTimer->stop();

    QWidget::mouseReleaseEvent(event);
}

void SheetsWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (document)
    {
        markOrUnmarkLastClickedPage();
    }

    QWidget::mouseDoubleClickEvent(event);
}

void SheetsWidget::paintEvent(QPaintEvent* event)
{
    if (document)
    {
        QPainter painter(this);

        QMutexLocker renderedPagesMutexMutexLocker(&documentPagesPixmapsMutex);

        int beginPixmapXOffset = customXOffset + (width() / columnsCount - documentPagesPixmapsRenderSize.width()) / 2;
        int beginPixmapYOffset = customYOffset + (height() / rowsCount - documentPagesPixmapsRenderSize.height()) / 2;

        int pixmapXOffset = beginPixmapXOffset;
        int pixmapYOffset = beginPixmapYOffset;

        int renderedPagesCount = 0;

        int pagesCount = document->pagesCount();
        for (int i = 0; i < pagesCount; ++i)
        {
            if (!documentPagesImages[i].isNull())
            {
                documentPagesPixmaps[i] = QPixmap::fromImage(documentPagesImages[i]);
                documentPagesImages[i] = QImage();
                documentPagesPixmaps[i].setMask(documentPagesPixmaps[i].createMaskFromColor(Qt::white));
            }

            if (!documentPagesPixmaps[i].isNull())
            {
                ++renderedPagesCount;
            }
        }

        for (int i = firstRenderedPageNo; (i < pagesCount) && ((i - firstRenderedPageNo) < (rowsCount * columnsCount)); ++i)
        {
            painter.drawPixmap(pixmapXOffset, pixmapYOffset, documentPagesPixmapsRenderSize.width(), documentPagesPixmapsRenderSize.height(), documentPagesPixmaps[i]);

            bool showAsMarkedPage = (markedPageNumbers.contains(i) && ((settingsWidget->isVisible()) || ((QDateTime::currentMSecsSinceEpoch() - lastMarkOrUnmarkTime) < 2000)));
            bool showAsSelectedPage = ((i == lastMousePressPageNo) && settingsWidget->isVisible());

            if (showAsMarkedPage || showAsSelectedPage)
            {
                int xOffsetCorrection = qMax(-pixmapXOffset, 0);
                int yOffsetCorrection = qMax(-pixmapYOffset, 0);
                int widthCorrecion = qMax(pixmapXOffset + documentPagesPixmapsRenderSize.width() - width(), 0);
                int heightCorrecion = qMax(pixmapYOffset + documentPagesPixmapsRenderSize.height() - height(), 0);

                if (showAsSelectedPage)
                {
                    painter.setPen(QPen(Qt::blue, 4, Qt::DotLine));
                    painter.drawRect(pixmapXOffset + 2 + xOffsetCorrection,
                                     pixmapYOffset + 2 + yOffsetCorrection,
                                     documentPagesPixmapsRenderSize.width() - 4 - xOffsetCorrection - widthCorrecion,
                                     documentPagesPixmapsRenderSize.height() - 4 - yOffsetCorrection - heightCorrecion);

                    if (showAsMarkedPage)
                    {
                        painter.setPen(QPen(Qt::red, 4, Qt::DotLine));
                        painter.drawRect(pixmapXOffset + 6 + xOffsetCorrection,
                                         pixmapYOffset + 6 + yOffsetCorrection,
                                         documentPagesPixmapsRenderSize.width() - 12 - xOffsetCorrection - widthCorrecion,
                                         documentPagesPixmapsRenderSize.height() - 12 - yOffsetCorrection - heightCorrecion);
                    }
                }
                else
                {
                    painter.setPen(QPen(Qt::red, 4, Qt::DotLine));
                    painter.drawRect(pixmapXOffset + 2 + xOffsetCorrection,
                                     pixmapYOffset + 2 + yOffsetCorrection,
                                     documentPagesPixmapsRenderSize.width() - 4 - xOffsetCorrection - widthCorrecion,
                                     documentPagesPixmapsRenderSize.height() - 4 - yOffsetCorrection - heightCorrecion);
                }
            }

            if (((i + 1 - firstRenderedPageNo) % columnsCount) == 0)
            {
                pixmapXOffset = beginPixmapXOffset;
                pixmapYOffset += height() / rowsCount;
            }
            else
            {
                pixmapXOffset += width() / columnsCount;
            }
        }

        if (renderedPagesCount != pagesCount)
        {
            int progessBarXBegin = static_cast<int>((static_cast<double>(renderedPagesCount) / static_cast<double>(pagesCount)) * width());
            painter.fillRect(progessBarXBegin, 0, width(), height(), QColor(160, 160, 160, 160));
        }
    }
    else
    {
        QWidget::paintEvent(event);
    }
}

void SheetsWidget::resizeEvent(QResizeEvent* event)
{
    reRenderPixmaps();
    QWidget::resizeEvent(event);
}

void SheetsWidget::reRenderPixmaps()
{
    if (pagesRenderer)
    {
        pagesRenderer->stopAndWait();
        pagesRenderer.reset();
    }

    if (document)
    {
        QSize sizeInWidgetPerPage = QSize(width() / columnsCount, height() / rowsCount);
        QSize pageSize = document->getPageSize(0);
        double pageAspect = pageSize.width() / pageSize.height();

        if ((sizeInWidgetPerPage.width() / pageSize.width()) < (sizeInWidgetPerPage.height() / pageSize.height()))
        {
            documentPagesPixmapsRenderSize = QSize(qRound(static_cast<double>(sizeInWidgetPerPage.width()) * scaleFactor), qRound(static_cast<double>(sizeInWidgetPerPage.width()) * scaleFactor / pageAspect));
            documentPagesScaleToWidth = true;
        }
        else
        {
            documentPagesPixmapsRenderSize = QSize(qRound(static_cast<double>(sizeInWidgetPerPage.height()) * scaleFactor * pageAspect), qRound(static_cast<double>(sizeInWidgetPerPage.height()) * scaleFactor));
            documentPagesScaleToWidth = false;
        }

        QSet<int> pagesToRenderFirst;
        int pagesCount = document->pagesCount();
        for (int i = firstRenderedPageNo; ((i + firstRenderedPageNo) < (rowsCount * columnsCount)) && (i < pagesCount); ++i)
        {
            pagesToRenderFirst.insert(i);
        }
        pagesRenderer = std::unique_ptr<PagesRenderer>(new PagesRenderer(this, pagesToRenderFirst));
        connect(pagesRenderer.get(), SIGNAL(pageRendered(int)), this, SLOT(update()));
        pagesRendererWorker->start(pagesRenderer.get());
    }
}

void SheetsWidget::markOrUnmarkPage(int pageNo)
{
    if (document)
    {
        if (pageNo < document->pagesCount())
        {
            if (markedPageNumbers.contains(pageNo))
            {
                markedPageNumbers.remove(pageNo);
            }
            else
            {
                markedPageNumbers.insert(pageNo);
            }

            QSettings settings;
            settings.beginGroup(documentBaseName);
            settings.setValue("markedPageNumbers", serializeIntSet(markedPageNumbers));
            settings.endGroup();

            lastMarkOrUnmarkTime = QDateTime::currentMSecsSinceEpoch();
            QTimer::singleShot(3000, this, SLOT(update()));

            update();
        }
    }
}

QString SheetsWidget::getLayoutSettingGroupName()
{
    QString ret = QString::number(columnsCount);
    ret += "x";
    ret += QString::number(rowsCount);
    return ret;
}

QString SheetsWidget::serializeIntSet(const QSet<int>& intSet)
{
    QString ret = "";
    for (int value : intSet)
    {
        ret += QString::number(value);
        ret += ";";
    }

    return ret;
}

QSet<int> SheetsWidget::deserializeIntSet(const QString& intSetStr)
{
    QSet<int> ret;
    for (const QString& valueStr : intSetStr.split(";"))
    {
        if (!valueStr.isEmpty())
        {
            bool parseOk;
            int value = valueStr.toInt(&parseOk, 10);
            if (parseOk)
            {
                ret.insert(value);
            }
        }
    }

    return ret;
}

void SheetsWidget::setScaleFactor(double scaleFactor, bool updateViewAndSettings)
{
    this->scaleFactor = scaleFactor;
    if (updateViewAndSettings)
    {
        reRenderPixmaps();
        update();

        QSettings settings;
        settings.beginGroup(documentBaseName);
        settings.beginGroup(getLayoutSettingGroupName());
        settings.setValue("scaleFactor", scaleFactor);
        settings.endGroup();
        settings.endGroup();
    }
}

void SheetsWidget::setCustomXOffset(int customXOffset, bool updateViewAndSettings)
{
    this->customXOffset = customXOffset;
    if (updateViewAndSettings)
    {
        update();

        QSettings settings;
        settings.beginGroup(documentBaseName);
        settings.beginGroup(getLayoutSettingGroupName());
        settings.setValue("customXOffset", customXOffset);
        settings.endGroup();
        settings.endGroup();
    }
}

void SheetsWidget::setCustomYOffset(int customYOffset, bool updateViewAndSettings)
{
    this->customYOffset = customYOffset;
    if (updateViewAndSettings)
    {
        update();

        QSettings settings;
        settings.beginGroup(documentBaseName);
        settings.beginGroup(getLayoutSettingGroupName());
        settings.setValue("customYOffset", customYOffset);
        settings.endGroup();
        settings.endGroup();
    }
}

void SheetsWidget::setShownPagesCount(int preferedPagesCount, bool updateViewAndSettings)
{
    if (document)
    {
        preferedPagesCount = qMax(qMin(preferedPagesCount, qMin(document->pagesCount(), 8)), 1);
        switch (preferedPagesCount)
        {
        case 1:
            rowsCount = 1;
            columnsCount = 1;
            break;

        case 2:
            rowsCount = 1;
            columnsCount = 2;
            break;

        case 3:
            rowsCount = 1;
            columnsCount = 3;
            break;

        case 4:
        case 5:
        case 6:
            rowsCount = 2;
            columnsCount = 3;
            break;

        case 7:
        case 8:
            rowsCount = 2;
            columnsCount = 4;
            break;
        }

        if (updateViewAndSettings)
        {
            QSettings settings;
            settings.beginGroup(documentBaseName);
            settings.setValue("shownPagesCount", rowsCount * columnsCount);
            markedPageNumbers = deserializeIntSet(settings.value("markedPageNumbers", QString("")).toString());
            settings.beginGroup(getLayoutSettingGroupName());
            setScaleFactor(settings.value("scaleFactor", 1.0).toDouble(), false);
            setCustomXOffset(settings.value("customXOffset", 0).toInt(), false);
            setCustomYOffset(settings.value("customYOffset", 0).toInt(), false);
            settings.endGroup();
            settings.endGroup();

            reRenderPixmaps();
            update();
        }
    }
}

PagesRenderer::PagesRenderer(SheetsWidget* sheetsWidget, const QSet<int>& pagesToRenderFirst) :
    sheetsWidget(sheetsWidget),
    stop(false),
    pagesToRenderFirst(pagesToRenderFirst)
{
    setAutoDelete(false);
}

PagesRenderer::~PagesRenderer()
{
    stopAndWait();
}

void PagesRenderer::run()
{
    if (sheetsWidget->document)
    {
        int pagesCount = sheetsWidget->document->pagesCount();

        bool allPagesRendered = false;

        bool scaleToWidth;
        int targetWidthOrHeigh;
        {
            QMutexLocker renderedPagesMutexMutexLocker(&sheetsWidget->documentPagesPixmapsMutex);
            scaleToWidth = sheetsWidget->documentPagesScaleToWidth;
            targetWidthOrHeigh = (scaleToWidth) ? sheetsWidget->documentPagesPixmapsRenderSize.width() : sheetsWidget->documentPagesPixmapsRenderSize.height();
        }

        while (!stop && !allPagesRendered)
        {
            int pageToRender = -1;
            {
                QMutexLocker renderedPagesMutexMutexLocker(&renderedPagesMutex);
                if (renderedPages.count() == pagesCount)
                {
                    allPagesRendered = true;
                }
                else
                {
                    pagesToRenderFirst.subtract(renderedPages);
                    QSet<int>::iterator pagesToRenderFirstIter = pagesToRenderFirst.begin();
                    if (pagesToRenderFirstIter == pagesToRenderFirst.end())
                    {
                        for (int i = 0; (i < pagesCount) && !stop; ++i)
                        {
                            if (!renderedPages.contains(i))
                            {
                                pageToRender = i;
                            }
                        }
                    }
                    else
                    {
                        pageToRender = *pagesToRenderFirstIter;
                        pagesToRenderFirst.erase(pagesToRenderFirstIter);
                    }
                }
            }

            if (!allPagesRendered && !stop)
            {
                QImage renderedImage = (scaleToWidth) ? sheetsWidget->document->renderPageScaledToWidth(pageToRender, targetWidthOrHeigh) : sheetsWidget->document->renderPageScaledToHeight(pageToRender, targetWidthOrHeigh);
                if (!stop)
                {
                    if (!stop)
                    {
                        if (!stop)
                        {
                            {
                                QMutexLocker documentPagesPixmapsMutexLocker(&sheetsWidget->documentPagesPixmapsMutex);
                                sheetsWidget->documentPagesImages[pageToRender] = renderedImage;
                                sheetsWidget->documentPagesPixmapsRenderSize = renderedImage.size();
                            }

                            emit pageRendered(pageToRender);

                            if (!stop)
                            {
                                QMutexLocker renderedPagesMutexMutexLocker(&renderedPagesMutex);
                                renderedPages.insert(pageToRender);
                                pagesToRenderFirst.subtract(renderedPages);
                            }
                        }
                    }
                }
            }
        }
    }
}

void PagesRenderer::stopAndWait()
{
    stop = true;

    sheetsWidget->pagesRendererWorker->waitForDone();
}

int PagesRenderer::getRenderedPagesCount() const noexcept
{
    QMutexLocker renderedPagesMutexMutexLocker(&renderedPagesMutex);
    return renderedPages.count();
}
