SheetMusicViewer
================

Simple sheet music PDF viewer implemented in Qt (and thus for Linux, Embedded Linux, Android, Windows, probably Mac...)

Build instructions
------------------

Clone the repository using the git command line utility (or a tool of your choice). Make sure the submodules are (recursively) cloned as well:

    git clone https://github.com/felixhaedicke/SheetMusicViewer.git
    git submodule update --init --recursive

For the mupdf subproject, some files need to be generated:

    cd mupdf
    make generate
    cd ..

or alternatively, when building with MSVC on Windows:

    cd mupdf/win32
    generate.bat
    cd ../..

Generate icon png files (for Android):

    rsvg-convert icon.svg -w 32 -h 32 -o android/res/drawable-ldpi/icon.png
    rsvg-convert icon.svg -w 48 -h 48 -o android/res/drawable-mdpi/icon.png
    rsvg-convert icon.svg -w 72 -h 72 -o android/res/drawable-hdpi/icon.png

Build SheetMusicViewer using qmake (or an IDE of your choice):

    qmake

At least when compiling for Android, you can get errors like this:

    mupdf/fitz/memento.h:182:1: error: unknown type name 'size_t'

To fix it, you need to apply a patch on mupdf:

    cd mupdf
    patch -Np1 -i ../mupdf-stddef.diff
    cd ..

