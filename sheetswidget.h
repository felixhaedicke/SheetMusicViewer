#ifndef SHEETSWIDGET_H
#define SHEETSWIDGET_H

#include <memory>

#include <QBitmap>
#include <QFileInfo>
#include <QKeyEvent>
#include <QPalette>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QMutex>
#include <QResizeEvent>
#include <QRunnable>
#include <QSet>
#include <QSettings>
#include <QStackedLayout>
#include <QThreadPool>
#include <QTimer>
#include <QWaitCondition>
#include <QWidget>

#include "pdfdocument.h"
#include "settingswidget.h"

class PagesRenderer;

class SheetsWidget : public QWidget
{
Q_OBJECT
public:
    explicit SheetsWidget(QWidget* parent,
                          const QFileInfo& documentFileInfo);
    virtual ~SheetsWidget();

    const QFileInfo& getDocumentFileInfo() const noexcept { return documentFileInfo; }

    void setScaleFactor(double scaleFactor) { setScaleFactor(scaleFactor, true); }
    void setCustomXOffset(int customXOffset) { setCustomXOffset(customXOffset, true); }
    void setCustomYOffset(int customYOffset) { setCustomYOffset(customYOffset, true); }

    double getScaleFactor() const noexcept { return scaleFactor; }
    int getCustomXOffset() const noexcept { return customXOffset; }
    int getCustomYOffset() const noexcept { return customYOffset; }

    void setShownPagesCount(int preferedPagesCount) { setShownPagesCount(preferedPagesCount, true); }

public slots:
    void decreaseShownPagesCount();
    void increaseShownPagesCount();
    void zoomIn();
    void zoomOut();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void resetZoomAndOffset();
    void nextPagesSet();
    void previousPagesSet();
    void toPreviousMarkedPage();
    void toFirstPage();
    void toLastPage();
    void markOrUnmarkLastClickedPage();

signals:
    void requestShowSheetSelection();
    void requestExit();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);

private:
    void reRenderPixmaps();

    void markOrUnmarkPage(int pageNo);

    QString getLayoutSettingGroupName();
    QString serializeIntSet(const QSet<int>& intSet);
    QSet<int> deserializeIntSet(const QString& intSetStr);

    void setScaleFactor(double scaleFactor, bool updateViewAndSettings);
    void setCustomXOffset(int customXOffset, bool updateViewAndSettings);
    void setCustomYOffset(int customYOffset, bool updateViewAndSettings);

    void setShownPagesCount(int preferedPagesCount, bool updateViewAndSettings);
    
    friend class PagesRenderer;

    QStackedLayout* stackedLayout;
    SettingsWidget* settingsWidget;

    QFileInfo documentFileInfo;
    QString documentBaseName;
    std::unique_ptr<PdfDocument> document;

    mutable QMutex documentPagesPixmapsMutex;
    std::unique_ptr<QImage[]> documentPagesImages;
    std::unique_ptr<QPixmap[]> documentPagesPixmaps;
    QSize documentPagesPixmapsRenderSize;
    bool documentPagesScaleToWidth;

    QThreadPool* pagesRendererWorker;
    std::unique_ptr<PagesRenderer> pagesRenderer;

    int rowsCount;
    int columnsCount;
    int firstRenderedPageNo;
    double scaleFactor;
    int customXOffset;
    int customYOffset;
    QSet<int> markedPageNumbers;
    qint64 lastMarkOrUnmarkTime;

    QTimer* longClickTimer;
    int lastMousePressPageNo;

    QTimer* settingsControlsInactivityTimer;
};

class PagesRenderer : public QObject, public QRunnable
{
Q_OBJECT
public:
    PagesRenderer(SheetsWidget* sheetsWidget, const QSet<int>& pagesToRenderFirst);
    virtual ~PagesRenderer();

    void run();
    void stopAndWait();
    int getRenderedPagesCount() const noexcept;

signals:
    void pageRendered(int pageNo);

private:
    PagesRenderer();
    PagesRenderer(const PagesRenderer&);
    PagesRenderer& operator=(const PagesRenderer&);

    SheetsWidget* sheetsWidget;

    volatile bool stop;

    mutable QMutex renderedPagesMutex;
    QSet<int> renderedPages;
    QSet<int> pagesToRenderFirst;
};

#endif // SHEETSWIDGET_H
