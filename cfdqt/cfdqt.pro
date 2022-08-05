#-------------------------------------------------
#
# Project created by QtCreator 2019-03-20T11:30:37
#
#-------------------------------------------------

QT       += core gui widgets 3dcore 3drender 3dextras opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = CFDQt
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# No debug output
CONFIG(release, debug|release): DEFINES += QT_NO_DEBUG_OUTPUT

CONFIG += c++11

SOURCES += \
        $$PWD/Sources/main.cpp \
    $$PWD/Sources/mainwindow.cpp \
    $$PWD/Sources/qcustomcanvas.cpp \
    $$PWD/Sources/qcustomplot.cpp \
    $$PWD/Sources/customplotzoom.cpp \
    $$PWD/Sources/geometry.cpp \
    $$PWD/Sources/data.cpp \
    $$PWD/Sources/material.cpp \
    $$PWD/Sources/cell.cpp \
    $$PWD/Sources/tracking.cpp \
    $$PWD/Sources/cfd.cpp \
    cfdworker.cpp

HEADERS += \
    $$PWD/Headers/mainwindow.h \
    $$PWD/Headers/qcustomcanvas.h \
    $$PWD/Headers/qcustomplot.h \
    $$PWD/Headers/customplotzoom.h \
    ../include/qt3dwidget.h \
    $$PWD/Headers/geometry.h \
    $$PWD/Headers/data.h \
    $$PWD/Headers/material.h \
    $$PWD/Headers/cell.h \
    $$PWD/Headers/tracking.h \
    $$PWD/Headers/cfd.h \
    cfdworker.h

FORMS += \
    mainwindow.ui \
    plotcanvas.ui

gcc {
  QMAKE_LFLAGS += -fopenmp
  QMAKE_CXXFLAGS += -fopenmp
  #QMAKE_CXXFLAGS += -fsanitize=address
  #QMAKE_LFLAGS += -fsanitize=address
  LIBS += -fopenmp
}

msvc {
  QMAKE_CXXFLAGS += -openmp
}

INCLUDEPATH += \
    $$PWD/Headers

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    README.md

RESOURCES += \
    resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/release/ -lqt3dwidget
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/debug/ -lqt3dwidget
else:unix: LIBS += -L$$PWD/../lib/ -lqt3dwidget

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include
