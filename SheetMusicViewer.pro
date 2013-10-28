# -------------------------------------------------
# Project created by QtCreator 2012-04-14T21:50:01
# -------------------------------------------------
QT += widgets
TARGET = SheetMusicViewer
TEMPLATE = app

INCLUDEPATH += mupdf \
    mupdf/thirdparty/jbig2dec \
    mupdf/thirdparty/openjpeg/libopenjpeg \
    mupdf/thirdparty/openjpeg/thirdparty/liblcms2/include \
    mupdf/thirdparty/jpeg \
    mupdf/thirdparty/zlib \
    mupdf/thirdparty/freetype/include \
    mupdf/draw \
    mupdf/fitz \
    mupdf/pdf \
    mupdf/xps \
    mupdf/cbz \
    mupdf/scripts

QMAKE_CFLAGS += \
    -DFT2_BUILD_LIBRARY -DDARWIN_NO_CARBON -DHAVE_STDINT_H \
    \'-DFT_CONFIG_MODULES_H=\"slimftmodules.h\"\' \
    \'-DFT_CONFIG_OPTIONS_H=\"slimftoptions.h\"\' \
    -Wa,-I$$IN_PWD/mupdf

SOURCES += main.cpp \
    mainwindow.cpp \
    sheetswidget.cpp \
    sheetselectwidget.cpp \
    settingswidget.cpp \
    mupdfdocument.cpp \
    mupdf/cbz/mucbz.c \
    mupdf/xps/xps_zip.c \
    mupdf/xps/xps_util.c \
    mupdf/xps/xps_outline.c \
    mupdf/xps/xps_gradient.c \
    mupdf/xps/xps_path.c \
    mupdf/xps/xps_tile.c \
    mupdf/xps/xps_image.c \
    mupdf/xps/xps_glyphs.c \
    mupdf/xps/xps_resource.c \
    mupdf/xps/xps_doc.c \
    mupdf/xps/xps_common.c \
    mupdf/draw/draw_affine.c \
    mupdf/draw/draw_mesh.c \
    mupdf/draw/draw_edge.c \
    mupdf/draw/draw_path.c \
    mupdf/draw/draw_device.c \
    mupdf/draw/draw_unpack.c \
    mupdf/draw/draw_glyph.c \
    mupdf/draw/draw_blend.c \
    mupdf/draw/draw_scale.c \
    mupdf/draw/draw_paint.c \
    mupdf/pdf/pdf_xobject.c \
    mupdf/pdf/pdf_font.c \
    mupdf/pdf/pdf_parse.c \
    mupdf/pdf/pdf_interpret.c \
    mupdf/pdf/pdf_repair.c \
    mupdf/pdf/pdf_device.c \
    mupdf/pdf/pdf_store.c \
    mupdf/pdf/pdf_colorspace.c \
    mupdf/pdf/pdf_page.c \
    mupdf/pdf/pdf_crypt.c \
    mupdf/pdf/pdf_outline.c \
    mupdf/pdf/pdf_annot.c \
    mupdf/pdf/pdf_stream.c \
    mupdf/pdf/pdf_type3.c \
    mupdf/pdf/pdf_cmap_table.c \
    mupdf/pdf/pdf_xref.c \
    mupdf/pdf/pdf_cmap_load.c \
    mupdf/pdf/pdf_nametree.c \
    mupdf/pdf/pdf_encoding.c \
    mupdf/pdf/pdf_xref_aux.c \
    mupdf/pdf/pdf_write.c \
    mupdf/pdf/pdf_function.c \
    mupdf/pdf/pdf_lex.c \
    mupdf/pdf/pdf_cmap.c \
    mupdf/pdf/pdf_fontfile.c \
    mupdf/pdf/pdf_event.c \
    mupdf/pdf/pdf_form.c \
    mupdf/pdf/pdf_image.c \
    mupdf/pdf/pdf_shade.c \
    mupdf/pdf/pdf_pattern.c \
    mupdf/pdf/pdf_unicode.c \
    mupdf/pdf/pdf_js_none.c \
    mupdf/pdf/pdf_object.c \
    mupdf/pdf/pdf_cmap_parse.c \
    mupdf/pdf/pdf_metrics.c \
    mupdf/fitz/filt_predict.c \
    mupdf/fitz/crypt_sha2.c \
    mupdf/fitz/base_hash.c \
    mupdf/fitz/memento.c \
    mupdf/fitz/dev_text.c \
    mupdf/fitz/base_error.c \
    mupdf/fitz/doc_outline.c \
    mupdf/fitz/res_path.c \
    mupdf/fitz/filt_jbig2d.c \
    mupdf/fitz/dev_list.c \
    mupdf/fitz/res_text.c \
    mupdf/fitz/res_shade.c \
    mupdf/fitz/res_pixmap.c \
    mupdf/fitz/crypt_aes.c \
    mupdf/fitz/stm_open.c \
    mupdf/fitz/image_png.c \
    mupdf/fitz/base_time.c \
    mupdf/fitz/doc_interactive.c \
    mupdf/fitz/base_trans.c \
    mupdf/fitz/res_halftone.c \
    mupdf/fitz/filt_lzwd.c \
    mupdf/fitz/image_jpeg.c \
    mupdf/fitz/crypt_md5.c \
    mupdf/fitz/image_jpx.c \
    mupdf/fitz/crypt_arc4.c \
    mupdf/fitz/doc_search.c \
    mupdf/fitz/image_tiff.c \
    mupdf/fitz/stm_buffer.c \
    mupdf/fitz/res_colorspace.c \
    mupdf/fitz/doc_link.c \
    mupdf/fitz/base_geometry.c \
    mupdf/fitz/dev_bbox.c \
    mupdf/fitz/base_string.c \
    mupdf/fitz/filt_flate.c \
    mupdf/fitz/res_store.c \
    mupdf/fitz/doc_document.c \
    mupdf/fitz/base_context.c \
    mupdf/fitz/base_memory.c \
    mupdf/fitz/image_md5.c \
    mupdf/fitz/dev_null.c \
    mupdf/fitz/filt_dctd.c \
    mupdf/fitz/stm_read.c \
    mupdf/fitz/stm_output.c \
    mupdf/fitz/stm_comp_buf.c \
    mupdf/fitz/filt_basic.c \
    mupdf/fitz/base_xml.c \
    mupdf/fitz/image_save.c \
    mupdf/fitz/dev_trace.c \
    mupdf/fitz/base_getopt.c \
    mupdf/fitz/filt_faxd.c \
    mupdf/fitz/res_bitmap.c \
    mupdf/fitz/res_font.c \
    mupdf/thirdparty/jbig2dec/jbig2.c \
    mupdf/thirdparty/jbig2dec/jbig2_arith.c \
    mupdf/thirdparty/jbig2dec/jbig2_arith_iaid.c \
    mupdf/thirdparty/jbig2dec/jbig2_arith_int.c \
    mupdf/thirdparty/jbig2dec/jbig2_generic.c \
    mupdf/thirdparty/jbig2dec/jbig2_halftone.c \
    mupdf/thirdparty/jbig2dec/jbig2_huffman.c \
    mupdf/thirdparty/jbig2dec/jbig2_image.c \
    mupdf/thirdparty/jbig2dec/jbig2_metadata.c \
    mupdf/thirdparty/jbig2dec/jbig2_mmr.c \
    mupdf/thirdparty/jbig2dec/jbig2_page.c \
    mupdf/thirdparty/jbig2dec/jbig2_refinement.c \
    mupdf/thirdparty/jbig2dec/jbig2_segment.c \
    mupdf/thirdparty/jbig2dec/jbig2_symbol_dict.c \
    mupdf/thirdparty/jbig2dec/jbig2_text.c \
    mupdf/thirdparty/jpeg/jaricom.c \
    mupdf/thirdparty/jpeg/jcomapi.c \
    mupdf/thirdparty/jpeg/jdapimin.c \
    mupdf/thirdparty/jpeg/jdapistd.c \
    mupdf/thirdparty/jpeg/jdarith.c \
    mupdf/thirdparty/jpeg/jdatadst.c \
    mupdf/thirdparty/jpeg/jdatasrc.c \
    mupdf/thirdparty/jpeg/jdcoefct.c \
    mupdf/thirdparty/jpeg/jdcolor.c \
    mupdf/thirdparty/jpeg/jddctmgr.c \
    mupdf/thirdparty/jpeg/jdhuff.c \
    mupdf/thirdparty/jpeg/jdinput.c \
    mupdf/thirdparty/jpeg/jdmainct.c \
    mupdf/thirdparty/jpeg/jdmarker.c \
    mupdf/thirdparty/jpeg/jdmaster.c \
    mupdf/thirdparty/jpeg/jdmerge.c \
    mupdf/thirdparty/jpeg/jdpostct.c \
    mupdf/thirdparty/jpeg/jdsample.c \
    mupdf/thirdparty/jpeg/jdtrans.c \
    mupdf/thirdparty/jpeg/jerror.c \
    mupdf/thirdparty/jpeg/jfdctflt.c \
    mupdf/thirdparty/jpeg/jfdctfst.c \
    mupdf/thirdparty/jpeg/jfdctint.c \
    mupdf/thirdparty/jpeg/jidctflt.c \
    mupdf/thirdparty/jpeg/jidctfst.c \
    mupdf/thirdparty/jpeg/jidctint.c \
    mupdf/thirdparty/jpeg/jmemmgr.c \
    mupdf/thirdparty/jpeg/jmemnobs.c \
    mupdf/thirdparty/jpeg/jquant1.c \
    mupdf/thirdparty/jpeg/jquant2.c \
    mupdf/thirdparty/jpeg/jutils.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/bio.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/cidx_manager.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/cio.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/dwt.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/event.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/image.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/j2k.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/j2k_lib.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/jp2.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/jpt.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/mct.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/mqc.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/openjpeg.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/phix_manager.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/pi.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/ppix_manager.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/raw.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/t1.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/t2.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/tcd.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/tgt.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/thix_manager.c \
    mupdf/thirdparty/openjpeg/libopenjpeg/tpix_manager.c \
    mupdf/thirdparty/openjpeg/thirdparty/libz/adler32.c \
    mupdf/thirdparty/openjpeg/thirdparty/libz/compress.c \
    mupdf/thirdparty/openjpeg/thirdparty/libz/crc32.c \
    mupdf/thirdparty/openjpeg/thirdparty/libz/deflate.c \
    mupdf/thirdparty/openjpeg/thirdparty/libz/inffast.c \
    mupdf/thirdparty/openjpeg/thirdparty/libz/inflate.c \
    mupdf/thirdparty/openjpeg/thirdparty/libz/inftrees.c \
    mupdf/thirdparty/openjpeg/thirdparty/libz/trees.c \
    mupdf/thirdparty/openjpeg/thirdparty/libz/uncompr.c \
    mupdf/thirdparty/openjpeg/thirdparty/libz/zutil.c

HEADERS += mainwindow.h \
    mupdfdocument.h \
    sheetswidget.h \
    sheetselectwidget.h \
    settingswidget.h

FORMS += \ 
    settingswidget.ui

OTHER_FILES += \
    android/res/values-id/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values/strings.xml \
    android/res/values/libs.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/layout/splash.xml \
    android/res/values-fr/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/AndroidManifest.xml \
    android/version.xml \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java

android|!unix {
    SOURCES += mupdf/thirdparty/freetype/src/base/ftbase.c \
        mupdf/thirdparty/freetype/src/base/ftbbox.c \
        mupdf/thirdparty/freetype/src/base/ftbitmap.c \
        mupdf/thirdparty/freetype/src/base/ftgasp.c \
        mupdf/thirdparty/freetype/src/base/ftglyph.c \
        mupdf/thirdparty/freetype/src/base/ftinit.c \
        mupdf/thirdparty/freetype/src/base/ftstroke.c \
        mupdf/thirdparty/freetype/src/base/ftsynth.c \
        mupdf/thirdparty/freetype/src/base/ftsystem.c \
        mupdf/thirdparty/freetype/src/base/fttype1.c \
        mupdf/thirdparty/freetype/src/base/ftxf86.c \
        mupdf/thirdparty/freetype/src/cff/cff.c \
        mupdf/thirdparty/freetype/src/psaux/psaux.c \
        mupdf/thirdparty/freetype/src/pshinter/pshinter.c \
        mupdf/thirdparty/freetype/src/psnames/psnames.c \
        mupdf/thirdparty/freetype/src/raster/raster.c \
        mupdf/thirdparty/freetype/src/sfnt/sfnt.c \
        mupdf/thirdparty/freetype/src/smooth/smooth.c \
        mupdf/thirdparty/freetype/src/truetype/truetype.c \
        mupdf/thirdparty/freetype/src/type1/type1.c \
        mupdf/thirdparty/freetype/src/cid/type1cid.c
}

RESOURCES += \
    SheetMusicViewer.qrc

unix {
    !android {
        LIBS += -lfreetype
    }
}
