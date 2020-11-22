#ifndef PDFDOCUMENT_H
#define PDFDOCUMENT_H

#include <QImage>
#include <QMutex>
#include <QString>
#include <QPdfDocument>

class PdfDocument final
{
private:
    PdfDocument();
    PdfDocument(const PdfDocument&);

    mutable QPdfDocument document;

    mutable QMutex mutex;

    QImage renderPageScaled(int pageIdx, int size, bool scaledToWidth) const;

public:
    explicit PdfDocument(const QString& fileName) noexcept;

    int pagesCount() const noexcept;

    QImage renderPageScaledToWidth(int pageIdx, int width) const
    {
        return renderPageScaled(pageIdx, width, true);
    }

    QImage renderPageScaledToHeight(int pageIdx, int height) const
    {
        return renderPageScaled(pageIdx, height, false);
    }

    QSize getPageSize(int pageIdx) const;
};

#endif // PDFDOCUMENT_H
