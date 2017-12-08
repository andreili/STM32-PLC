TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/include/jsoncpp

LIBS += -ljsoncpp

SOURCES += main.cpp \
    runtime.cpp \
    firmware_sample/firmware.cpp \
    firmware_sample/fwdb.cpp \
    io.cpp \
    gpio.cpp \
    led.cpp \
    plcbus.cpp \
    gpio_lib.c

HEADERS += \
    runtime.h \
    firmware_sample/firmware.h \
    firmware_sample/fwdb.h \
    io.h \
    gpio.h \
    led.h \
    plcbus.h \
    settings.h \
    gpio_lib.h
