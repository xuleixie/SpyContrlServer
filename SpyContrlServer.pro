QT += core
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#CONFIG -= qt

DESTDIR = $$PWD/../../bin

QMAKE_LFLAGS += /LARGEADDRESSAWARE:NO

SOURCES += \
        Calculator.cpp \
        SpyFunction.cpp \
        SpyManage.cpp \
        SpyManagePrivate.cpp \
        SpyMessage.cpp \
        SpyWorker.cpp \
        icsneo40DLLAPI.cpp \
        main.cpp

HEADERS += \
    Calculator.h \
    SpyFunction.h \
    SpyFunction.h \
    SpyManage.h \
    SpyManagePrivate.h \
    SpyMessage.h \
    SpyWorker.h \
    icsnVC40.h \
    icsneo40DLLAPI.h
