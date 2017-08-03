QT -= core gui opengl

APP = fw_di16
TARGET = fw_di16

CONFIG += c++11

QMAKE_LFLAGS = -T$$PWD/../cortex-m3/stm32f103c8.ld

DEFINES += STM32F10X_MD STM32F103xB

DESTDIR = ./
#unix:EXT = elf
#win32:EXT = elf.exe

win32:QMAKE_LFLAGS += -L/mingw32/usr/lib/gcc/arm-none-eabi/5.4.1
linux:QMAKE_LFLAGS += -L/usr/lib/gcc/arm-none-eabi/7.1.0/

createlist.target = all
createlist.commands += arm-none-eabi-objdump -S $${OUT_PWD}/$${APP}$${EXT} > $$join(APP,,,".lst")
createlist.commands += && arm-none-eabi-objcopy -Obinary $${OUT_PWD}/$${APP}$${EXT} $${OUT_PWD}/$${APP}.bin
createlist.commands += && arm-none-eabi-nm -nalS --size-sort $${OUT_PWD}/$${APP}$${EXT} | tail && arm-none-eabi-size $${OUT_PWD}/$${APP}$${EXT}
QMAKE_EXTRA_TARGETS += createlist

INCLUDEPATH += ../shared/inc ../shared/CMSIS

HEADERS += \
    ../shared/CMSIS/core_cm3.h \
    ../shared/CMSIS/core_cmFunc.h \
    ../shared/CMSIS/core_cmInstr.h \
    ../shared/inc/ISRstm32f10x_md.h \
    stm32_conf.h \
    ../shared/inc/bitbanding.h \
    ../shared/inc/my_func.h \
    ../shared/inc/stm32_flash.h \
    ../shared/inc/stm32_gpio.h \
    ../shared/inc/stm32_inc.h \
    ../shared/inc/stm32_nvic.h \
    ../shared/inc/stm32_pwr.h \
    ../shared/inc/stm32_rcc.h \
    ../shared/inc/stm32_spi.h \
    ../shared/inc/stm32_systick.h \
    ../shared/CMSIS/stm32f1xx.h

SOURCES += \
    ../shared/src/ISRstm32f10x_md.cpp \
    main.cpp \
    init.cpp \
    ../shared/src/stm32_spi.cpp \
    ../shared/src/stm32_systick.cpp \
    ../shared/src/my_func.cpp \
    ../shared/src/stm32_flash.cpp \
    ../shared/src/stm32_gpio.cpp \
    ../shared/src/stm32_nvic.cpp \
    ../shared/src/stm32_pwr.cpp \
    ../shared/src/stm32_rcc.cpp

