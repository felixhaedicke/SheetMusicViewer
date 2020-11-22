#include "pdfdocument.h"

PdfDocument::PdfDocument(const QString& fileName) noexcept
{
    document.load(fileName);
}

int PdfDocument::pagesCount() const noexcept
{
    QMutexLocker mutexLocker(&mutex);
    return document.pageCount();
}

QImage PdfDocument::renderPageScaled(int pageIdx, int size, bool scaledToWidth) const
{
    QMutexLocker mutexLocker(&mutex);

    QSizeF pageSize = document.pageSize(pageIdx);
    if (scaledToWidth)
    {
        return document.render(pageIdx, QSize(size, static_cast<int>(pageSize.height() * static_cast<qreal>(size) / pageSize.width())));
    }
    else
    {
        return document.render(pageIdx, QSize(static_cast<int>(pageSize.width() * static_cast<qreal>(size) / pageSize.height()), size));
    }
}

QSize PdfDocument::getPageSize(int pageIdx) const
{
    QMutexLocker mutexLocker(&mutex);
    return document.pageSize(pageIdx).toSize();
}
