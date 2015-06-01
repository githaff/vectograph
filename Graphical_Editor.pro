#-------------------------------------------------
#
# Project created by QtCreator 2015-05-29T15:54:20
#
#-------------------------------------------------

QT       += core gui
QT       += core
QT       += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Graphical_Editor
TEMPLATE = app

CONFIG   += c++14
SOURCES += main.cpp\
        MainWindow.cpp \
    object_transform.cpp \
    svg_reader.cpp \
    svg_writer.cpp \
    svgobjects.cpp \
    logic.cpp \
    colorselect.cpp \
    mygraphicsview.cpp \
    header.cpp

HEADERS  += MainWindow.h \
    header.h \
    svg_reader.h \
    svg_writer.h \
    svgobjects.h \
    colorselect.h \
    mygraphicsview.h

FORMS    += MainWindow.ui \
    colorselect.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    appresources.qrc

