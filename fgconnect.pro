#-------------------------------------------------
#
# Project created by QtCreator 2019-08-26T11:26:00
#
#-------------------------------------------------

QT       += xml widgets serialport network

TARGET = fgconnect
TEMPLATE = app

FORMS    += mainwidget.ui

HEADERS += \
    net_fdm.hxx \
    interface.h \
    mainwidget.h \
    interfacehandler.h \
    interfacefdm.h \
    deviceitem.h \
    deviceitemdelegate.h \
    deviceitemmodel.h

SOURCES += \
    main.cpp \
    mainwidget.cpp \
    interfacehandler.cpp \
    interface.cpp \
    interfacefdm.cpp \
    deviceitem.cpp \
    deviceitemdelegate.cpp \
    deviceitemmodel.cpp
