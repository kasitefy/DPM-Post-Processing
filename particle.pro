#-------------------------------------------------
#
# Project created by QtCreator 2017-03-31T15:11:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = particle
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    point.cpp \
    qcustomplot.cpp \
    cplot.cpp \
    exportdata.cpp \
    multifilter.cpp \
    range.cpp

HEADERS  += mainwindow.h \
    judge.h \
    point.h \
    qcustomplot.h \
    cplot.h \
    exportdata.h \
    multifilter.h \
    range.h

FORMS    += mainwindow.ui \
    cplot.ui \
    exportdata.ui \
    multifilter.ui

RC_FILE += qtrc.rc

RESOURCES += \
    res.qrc
