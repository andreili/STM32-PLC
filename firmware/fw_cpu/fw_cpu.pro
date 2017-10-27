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
linux:QMAKE_LFLAGS += -L/usr/lib/gcc/arm-none-eabi/7.2.0/

createlist.target = all
createlist.commands += arm-none-eabi-objdump -S $${OUT_PWD}/$${APP}$${EXT} > $$join(APP,,,".lst")
createlist.commands += && arm-none-eabi-objcopy -Obinary $${OUT_PWD}/$${APP}$${EXT} $${OUT_PWD}/$${APP}.bin
!win32:createlist.commands += && arm-none-eabi-nm -nalS --size-sort $${OUT_PWD}/$${APP}$${EXT} | grep " T " | tail
createlist.commands += && arm-none-eabi-size $${OUT_PWD}/$${APP}$${EXT}
QMAKE_EXTRA_TARGETS += createlist

INCLUDEPATH += ../shared/CMSIS/ ./inc/ ../shared/inc/ platforms/stm32/
INCLUDEPATH += ../shared/Third_Party/FatFs/src/ ../shared/Third_Party/FatFs/src/option/

HEADERS += \
    ../shared/CMSIS/core_cm4.h \
    ../shared/CMSIS/core_cmFunc.h \
    ../shared/CMSIS/core_cmInstr.h \
    ../shared/CMSIS/stm32f429xx.h \
    ../shared/CMSIS/stm32f4xx.h \
    ../shared/CMSIS/ISRstm32f429xx.h \
	inc/init.h \
        platforms/stm32/stm32_uart.h \
        platforms/stm32/stm32_rcc.h \
        platforms/stm32/stm32_gpio.h \
    ../shared/inc/my_func.h \
    inc/bitbanding.h \
    inc/plc_io.h \
    platforms/stm32/stm32_pwr.h \
    platforms/stm32/stm32_flash.h \
    platforms/stm32/stm32_systick.h \
    platforms/stm32/stm32_conf.h \
    platforms/stm32/xprintf.h \
    platforms/stm32/stm32_sdram.h \
    inc/plc_control.h \
    platforms/stm32/stm32_nvic.h \
    platforms/stm32/stm32_inc.h \
    ../shared/Third_Party/FatFs/src/ff.h \
    ../shared/Third_Party/FatFs/src/integer.h \
    platforms/stm32/ffconf.h \
    platforms/stm32/stm32_sd.h \
    platforms/stm32/fatfs.h \
    platforms/stm32/diskiodriver.h \
    platforms/stm32/sddriver.h \
    platforms/stm32/diskio.h \
    inc/memmanager.h \
    platforms/stm32/stm32_spi.h \
    ../shared/inc/plc_mod.h \
    platforms/stm32/stm32_rtc.h \
    inc/plc_state.h

SOURCES += \
    ../shared/CMSIS/ISRstm32f429xx.cpp \
    src/main.cpp \
	src/init.cpp \
        platforms/stm32/stm32_uart.cpp \
        platforms/stm32/stm32_rcc.cpp \
        platforms/stm32/stm32_gpio.cpp \
    ../shared/src/my_func.cpp \
    src/plc_io.cpp \
    platforms/stm32/stm32_pwr.cpp \
    platforms/stm32/stm32_flash.cpp \
    platforms/stm32/stm32_systick.cpp \
    platforms/stm32/stm32_sdram.cpp \
    src/plc_control.cpp \
    platforms/stm32/stm32_nvic.cpp \
    ../shared/Third_Party/FatFs/src/ff.c \
    ../shared/Third_Party/FatFs/src/option/syscall.c \
    ../shared/Third_Party/FatFs/src/option/ccsbcs.c \
    platforms/stm32/fatfs.cpp \
    platforms/stm32/sddriver.cpp \
    platforms/stm32/stm32_sd.cpp \
    src/memmanager.cpp \
    platforms/stm32/stm32_spi.cpp \
    ../shared/src/plc_mod.cpp \
    platforms/stm32/xprintf.cpp \
    platforms/stm32/stm32_rtc.cpp \
    src/plc_state.cpp

