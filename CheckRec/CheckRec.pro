#-------------------------------------------------
#
# Project created by QtCreator 2018-05-12T08:30:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CheckRec
TEMPLATE = app
CONFIG += C++11
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DISTFILES += $$PWD   #Build at current directory

#Call OPenCV with dll generated by CMake
INCLUDEPATH += D:/opencv/build/include \
               D:/opencv/build/include/opencv \
               D:/opencv/build/include/opencv2
#LIBS += $$PWD/bin/libopencv_*.dll \			   
LIBS += D:/Qt/OpenCV_Build/install/x86/mingw/bin/libopencv_*.dll \
        $$PWD/recognition/classification_dll.lib

SOURCES += main.cpp\
        mainwindow.cpp \
    subwindow.cpp \
    checkcode/checkcode.cpp \
    textproc.cpp \
    screencut/screencut.cpp \
    imageproc.cpp \
    screencut/dialog.cpp \
    recentopened/recentopened.cpp \
    histogram/histogram.cpp \
    drawdigit/drawdigit.cpp \
    drawdigit/trainwithtest.cpp

HEADERS  += mainwindow.h \
    subwindow.h \
    checkcode/checkcode.h \
    screencut/dialog.h \
    proc.h \
    recentopened/recentopened.h \
    common.h \
    recognition/classification.h \
    recognition/recognition.h \
    histogram/histogram.h \
    ui_mainwindow.h \
    drawdigit/drawdigit.h \
    recognition/support_common.h \
    recognition/classification_c.h
	
#qt designer -- no use
FORMS    += mainwindow.ui

#Add picture into this program
RESOURCES += \
          window.qrc
