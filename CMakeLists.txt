cmake_minimum_required(VERSION 3.5)

project(SheetMusicViewer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 Qt4 COMPONENTS Gui Widgets Svg Pdf REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Gui Widgets Svg Pdf REQUIRED)

if(ANDROID)
    add_library(SheetMusicViewer SHARED
        main.cpp
        mainwindow.cpp
        mainwindow.h
        pdfdocument.cpp
        pdfdocument.h
        sheetswidget.cpp
        sheetswidget.h
        sheetselectwidget.cpp
        sheetselectwidget.h
        settingswidget.cpp
        settingswidget.h
        settingswidget.ui
        SheetMusicViewer.qrc
    )
else()
    add_executable(SheetMusicViewer
        main.cpp
        mainwindow.cpp
        mainwindow.h
        pdfdocument.cpp
        pdfdocument.h
        sheetswidget.cpp
        sheetswidget.h
        sheetselectwidget.cpp
        sheetselectwidget.h
        settingswidget.cpp
        settingswidget.h
        settingswidget.ui
        SheetMusicViewer.qrc
    )
endif()

target_link_libraries(SheetMusicViewer PRIVATE Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Svg Qt${QT_VERSION_MAJOR}::Pdf)
