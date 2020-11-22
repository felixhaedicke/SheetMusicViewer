#include "pdfdocument.h"

extern "C"
{
    #include "fitz.h"
    #include "mupdf.h"
}

PdfDocument::PdfDocument(const QString& fileName) throw()
{
    documentContext = (void*) fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    document = (void*) fz_open_document((fz_context*) documentContext, fileName.toLocal8Bit().data());
}

PdfDocument::~PdfDocument() throw()
{
    fz_close_document((fz_document*) document);
    fz_free_context((fz_context*) documentContext);
}

int PdfDocument::pagesCount() const throw()
{
    QMutexLocker mutexLocker(&mutex);
    return fz_count_pages((fz_document*) document);
}

QImage PdfDocument::renderPageScaled(int pageIdx, int size, bool scaledToWidth) const
{
    QMutexLocker mutexLocker(&mutex);

    fz_page* page = fz_load_page((fz_document*) document, pageIdx);

    fz_rect pageBound;
    fz_bound_page((fz_document*) document, page, &pageBound);

    float scaleFactor;
    if (scaledToWidth)
    {
        scaleFactor = (float) size / (pageBound.x1 - pageBound.x0);
    }
    else
    {
        scaleFactor = (float) size / (pageBound.y1 - pageBound.y0);
    }

    fz_matrix transform;
    fz_scale(&transform, scaleFactor, scaleFactor);
    fz_transform_rect(&pageBound, &transform);
    fz_irect bbox;
    fz_round_rect(&bbox, &pageBound);

    fz_pixmap* pixmap = fz_new_pixmap_with_bbox((fz_context*) documentContext, fz_device_bgr, &bbox);
    fz_clear_pixmap_with_value((fz_context*) documentContext, pixmap, 0xff);
    fz_device* device = fz_new_draw_device((fz_context*) documentContext, pixmap);
    fz_run_page((fz_document*) document, page, device, &transform, NULL);

    unsigned char* samples = fz_pixmap_samples((fz_context*) documentContext, pixmap);
    int width = fz_pixmap_width((fz_context*) documentContext, pixmap);
    int height = fz_pixmap_height((fz_context*) documentContext, pixmap);
    QImage pageImage = QImage(samples, width, height, QImage::Format_ARGB32).copy();

    fz_free_device(device);
    fz_drop_pixmap((fz_context*) documentContext, pixmap);
    fz_free_page((fz_document*) document, page);

    return pageImage;
}

QSize PdfDocument::getPageSize(int pageIdx) const
{
    QMutexLocker mutexLocker(&mutex);

    fz_page* page = fz_load_page((fz_document*) document, pageIdx);
    fz_rect pageBound;
    fz_bound_page((fz_document*) document, page, &pageBound);
    fz_free_page((fz_document*) document, page);

    return QSize(pageBound.x1 - pageBound.x0, pageBound.y1 - pageBound.y0);
}
