#ifndef MUPDFDOCUMENT_H
#define MUPDFDOCUMENT_H

#include <QImage>
#include <QMutex>
#include <QString>

class MuPdfDocument
{
private:
    MuPdfDocument();
    MuPdfDocument(const MuPdfDocument&);

    void* documentContext;
    void* document;

    mutable QMutex mutex;

    QImage renderPageScaled(int pageIdx, int size, bool scaledToWidth) const;

public:
    explicit MuPdfDocument(const QString& fileName) throw();
    ~MuPdfDocument() throw();
    int pagesCount() const throw();

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

#endif // MUPDFDOCUMENT_H
