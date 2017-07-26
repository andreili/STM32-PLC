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

INCLUDEPATH += ./CMSIS/ ./inc/

HEADERS += \
    CMSIS/core_cm4.h \
    CMSIS/core_cmFunc.h \
    CMSIS/core_cmInstr.h \
    CMSIS/stm32f429xx.h \
    CMSIS/stm32f4xx.h \
    CMSIS/ISRstm32f429xx.h \
	inc/init.h \
	inc/stm32_uart.h \
	inc/stm32_rcc.h \
	inc/stm32_gpio.h \
	inc/my_func.h \
    inc/bitbanding.h \
    inc/plc_io.h \
    inc/stm32_pwr.h \
    inc/stm32_flash.h \
    inc/stm32_systick.h \
    inc/stm32_conf.h \
    inc/xprintf.h \
    inc/stm32_sdram.h \
    inc/plc_control.h \
    inc/stm32_nvic.h \
    inc/stm32_inc.h

SOURCES += \
    CMSIS/ISRstm32f429xx.cpp \
    src/main.cpp \
	src/init.cpp \
	src/stm32_uart.cpp \
	src/stm32_rcc.cpp \
	src/stm32_gpio.cpp \
	src/my_func.cpp \
    src/plc_io.cpp \
    src/stm32_pwr.cpp \
    src/stm32_flash.cpp \
    src/stm32_systick.cpp \
    src/xprintf.cpp \
    src/stm32_sdram.cpp \
    src/plc_control.cpp \
    src/stm32_nvic.cpp

