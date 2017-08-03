QT -= core gui opengl

APP = fw_cpu
TARGET = fw_cpu

CONFIG += c++11

QMAKE_LFLAGS += -T$$PWD/../cortex-m4/stm32f429xx.ld

DEFINES += STM32F429xx DATA_IN_ExtSDRAM

DESTDIR = ./
#win32:EXT = .exe

#QMAKE_CXXFLAGS += -O0
win32:QMAKE_LFLAGS += -L/mingw32/lib/gcc/arm-none-eabi/5.4.1/
linux:QMAKE_LFLAGS += -L/usr/lib/gcc/arm-none-eabi/7.1.0/

createlist.target = all
createlist.commands += arm-none-eabi-objdump -S $${OUT_PWD}/$${APP}$${EXT} > $$join(APP,,,".lst")
#createlist.commands += && arm-none-eabi-objcopy --remove-section .got* $${OUT_PWD}/$${APP}$${EXT}
createlist.commands += && arm-none-eabi-objcopy -Obinary $${OUT_PWD}/$${APP}$${EXT} $${OUT_PWD}/$${APP}.bin
createlist.commands += && arm-none-eabi-nm -nalS --size-sort $${OUT_PWD}/$${APP}$${EXT} | grep " T " | tail && arm-none-eabi-size $${OUT_PWD}/$${APP}$${EXT}
QMAKE_EXTRA_TARGETS += createlist

INCLUDEPATH += ../shared/inc/ ../shared/CMSIS/ ./inc/
INCLUDEPATH += ./Third_Party/FatFs/src/ ./Third_Party/FatFs/src/option/ ./Third_Party/FatFs/src/drivers

HEADERS += \
    ../shared/CMSIS/core_cm4.h \
    ../shared/CMSIS/core_cmFunc.h \
    ../shared/CMSIS/core_cmInstr.h \
    ../shared/CMSIS/stm32f429xx.h \
    ../shared/CMSIS/stm32f4xx.h \
    ../shared/CMSIS/ISRstm32f429xx.h \
    inc/init.h \
    ../shared/inc/stm32_uart.h \
    ../shared/inc/stm32_rcc.h \
    ../shared/inc/stm32_gpio.h \
    ../shared/inc/my_func.h \
    ../shared/inc/bitbanding.h \
    ../shared/inc/stm32_pwr.h \
    ../shared/inc/stm32_flash.h \
    ../shared/inc/stm32_systick.h \
    ../shared/inc/xprintf.h \
    ../shared/inc/stm32_sdram.h \
    ../shared/inc/stm32_nvic.h \
    ../shared/inc/stm32_inc.h \
    ../shared/inc/stm32_sd.h \
    ../shared/inc/stm32_spi.h \
    inc/plc_io.h \
    inc/stm32_conf.h \
    inc/plc_control.h \
    Third_Party/FatFs/src/ff.h \
    Third_Party/FatFs/src/integer.h \
    inc/ffconf.h \
    inc/fatfs.h \
    inc/diskiodriver.h \
    inc/sddriver.h \
    inc/diskio.h \
    inc/memmanager.h \
    inc/plc_mod.h \
    inc/plc_conf.h

SOURCES += \
    ../shared/CMSIS/ISRstm32f429xx.cpp \
    src/main.cpp \
    src/init.cpp \
    ../shared/src/stm32_uart.cpp \
    ../shared/src/stm32_rcc.cpp \
    ../shared/src/stm32_gpio.cpp \
    ../shared/src/my_func.cpp \
    ../shared/src/stm32_pwr.cpp \
    ../shared/src/stm32_flash.cpp \
    ../shared/src/stm32_systick.cpp \
    ../shared/src/xprintf.cpp \
    ../shared/src/stm32_sdram.cpp \
    ../shared/src/stm32_nvic.cpp \
    ../shared/src/stm32_sd.cpp \
    ../shared/src/stm32_spi.cpp \
    src/plc_io.cpp \
    src/plc_control.cpp \
    Third_Party/FatFs/src/ff.c \
    Third_Party/FatFs/src/option/syscall.c \
    Third_Party/FatFs/src/option/ccsbcs.c \
    src/fatfs.cpp \
    src/sddriver.cpp \
    src/memmanager.cpp \
    src/plc_mod.cpp

