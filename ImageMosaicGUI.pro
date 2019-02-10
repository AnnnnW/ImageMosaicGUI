#-------------------------------------------------
#
# Project created by QtCreator 2019-02-07T01:02:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageMosaicGUI
TEMPLATE = app

# Enable pkg-config (pkg-config is disabled by default in the Qt package for mac)
QT_CONFIG -= no-pkg-config
# pkg-config location if your brew installation is standard
PKG_CONFIG = /usr/local/Cellar/opencv/4.0.1/lib/pkgconfig

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        Tiler.cpp \
        TileAnalyser.cpp \
        ImageCutter.cpp \


HEADERS += \
        mainwindow.h \
        ImageCutter.hpp \
        TileAnalyser.hpp \
        Tiler.hpp

FORMS += \
        mainwindow.ui


INCLUDEPATH += \
        /usr/local/include/opencv4 \
        /usr/local/Cellar/opencv/4.0.1/lib \
        /usr/local/lib



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += /usr/local/Cellar/opencv/4.0.1/lib/*.4.0.1.dylib
