TEMPLATE = app
CONFIG += c++11 console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/include/jsoncpp ./include/

LIBS += -ljsoncpp

target.path = /_cross/tmp
INSTALLS += target

SOURCES += main.cpp \
    runtime.cpp \
    firmware_sample/firmware.cpp \
    firmware_sample/fwdb.cpp \
    plcbus.cpp \
    plcstate.cpp \
    plcdata.cpp

HEADERS += \
    runtime.h \
    firmware_sample/firmware.h \
    firmware_sample/fwdb.h \
    plcbus.h \
    settings.h \
    include/hps_0_arm_a9_0.h \
    include/hps_0_arm_a9_1.h \
    include/hps_0_bridges.h \
    include/hps_0.h \
    include/hps.h \
    plcstate.h \
    datainterface.h \
    plcdata.h
