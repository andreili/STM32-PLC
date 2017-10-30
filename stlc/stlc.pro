QT -= core gui

CONFIG += c++11

TARGET = stlc
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../firmware/fw_cpu/inc/

DEFINES += STLC_DEF

QMAKE_CXXFLAGS_RELEASE += -s
win32:QMAKE_LFLAGS_RELEASE += -s -static -static-libgcc -static-libstdc++

SOURCES += main.cpp \
    CmdlineParser.cpp \
    Stream.cpp \
    stlc.cpp \
    ../firmware/fw_cpu/src/stl_commands.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    CmdlineParser.h \
    Stream.h \
    stlc.h \
    ../firmware/fw_cpu/inc/plc_data_types.h \
    ../firmware/fw_cpu/inc/stl_commands.h \
    ../firmware/fw_cpu/inc/plc_hw.h
