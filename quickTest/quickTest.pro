QT += testlib
QT += gui \
    core \
    quickwidgets \
    quick


CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  tst_test1.cpp \
    ../quick_v1/filerw.cpp \
    ../quick_v1/mainwindow.cpp

HEADERS += \
    ../quick_v1/filerw.h \
    ../quick_v1/mainwindow.h

DISTFILES +=

RESOURCES += \
    ../quick_v1/qml.qrc
