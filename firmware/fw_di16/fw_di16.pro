QT -= core gui opengl

APP = fw_di16
TARGET = fw_di16

CONFIG += c++11

QMAKE_LFLAGS = -T$$PWD/../cortex-m3/stm32f103c8.ld

DEFINES += STM32F10X_MD STM32F103xB

DESTDIR = ./
#unix:EXT = elf
#win32:EXT = elf.exe

win32:QMAKE_LFLAGS += -LC:/msys64/mingw32/lib/gcc/arm-none-eabi/5.4.1
!win32:QMAKE_LFLAGS += -L/usr/lib/gcc/arm-none-eabi/7.1.0/

createlist.target = all
createlist.commands += arm-none-eabi-objdump -S $${OUT_PWD}/$${APP}$${EXT} > $$join(APP,,,".lst")
createlist.commands += && arm-none-eabi-objcopy -Obinary $${OUT_PWD}/$${APP}$${EXT} $${OUT_PWD}/$${APP}.bin
!win32:createlist.commands += && arm-none-eabi-nm -nalS --size-sort $${OUT_PWD}/$${APP}$${EXT} | tail
createlist.commands += && arm-none-eabi-size $${OUT_PWD}/$${APP}$${EXT}
QMAKE_EXTRA_TARGETS += createlist

INCLUDEPATH += ../shared/CMSIS/ ../shared/HAL/inc/ ../shared/inc/ ./inc/

HEADERS += \
    ../shared/CMSIS/arm_common_tables.h \
    ../shared/CMSIS/arm_const_structs.h \
    ../shared/CMSIS/arm_math.h \
    ../shared/CMSIS/cmsis_armcc.h \
    ../shared/CMSIS/cmsis_armcc_V6.h \
    ../shared/CMSIS/cmsis_gcc.h \
    ../shared/CMSIS/core_cm0.h \
    ../shared/CMSIS/core_cm0plus.h \
    ../shared/CMSIS/core_cm3.h \
    ../shared/CMSIS/core_cm4.h \
    ../shared/CMSIS/core_cm7.h \
    ../shared/CMSIS/core_cmFunc.h \
    ../shared/CMSIS/core_cmInstr.h \
    ../shared/CMSIS/core_cmSimd.h \
    ../shared/CMSIS/core_sc000.h \
    ../shared/CMSIS/core_sc300.h \
    ../shared/CMSIS/stm32f1xx.h \
    ../shared/CMSIS/stm32f103xb.h \
    ../shared/CMSIS/system_stm32f1xx.h \
    ../shared/HAL/inc/stm32f1xx_hal.h \
    ../shared/HAL/inc/stm32f1xx_hal_cortex.h \
    ../shared/HAL/inc/stm32f1xx_hal_def.h \
    ../shared/HAL/inc/stm32f1xx_hal_dma.h \
    ../shared/HAL/inc/stm32f1xx_hal_dma_ex.h \
    ../shared/HAL/inc/stm32f1xx_hal_flash.h \
    ../shared/HAL/inc/stm32f1xx_hal_flash_ex.h \
    ../shared/HAL/inc/stm32f1xx_hal_gpio.h \
    ../shared/HAL/inc/stm32f1xx_hal_gpio_ex.h \
    ../shared/HAL/inc/stm32f1xx_hal_iwdg.h \
    ../shared/HAL/inc/stm32f1xx_hal_pwr.h \
    ../shared/HAL/inc/stm32f1xx_hal_rcc.h \
    ../shared/HAL/inc/stm32f1xx_hal_rcc_ex.h \
    ../shared/HAL/inc/stm32f1xx_hal_spi.h \
    ../shared/HAL/inc/stm32f1xx_hal_tim.h \
    ../shared/HAL/inc/stm32f1xx_hal_tim_ex.h \
    ../shared/HAL/inc/Legacy/stm32_hal_legacy.h \
    inc/main.h \
    inc/stm32f1xx_hal_conf.h \
    inc/stm32f1xx_it.h \
    ../shared/inc/plc_mod.h \
    inc/plc_conf.h

SOURCES += \
    ../shared/HAL/src/stm32f1xx_hal.c \
    ../shared/HAL/src/stm32f1xx_hal_cortex.c \
    ../shared/HAL/src/stm32f1xx_hal_dma.c \
    ../shared/HAL/src/stm32f1xx_hal_flash.c \
    ../shared/HAL/src/stm32f1xx_hal_flash_ex.c \
    ../shared/HAL/src/stm32f1xx_hal_gpio.c \
    ../shared/HAL/src/stm32f1xx_hal_gpio_ex.c \
    ../shared/HAL/src/stm32f1xx_hal_iwdg.c \
    ../shared/HAL/src/stm32f1xx_hal_pwr.c \
    ../shared/HAL/src/stm32f1xx_hal_rcc.c \
    ../shared/HAL/src/stm32f1xx_hal_rcc_ex.c \
    ../shared/HAL/src/stm32f1xx_hal_spi.c \
    ../shared/HAL/src/stm32f1xx_hal_spi_ex.c \
    ../shared/HAL/src/stm32f1xx_hal_tim.c \
    ../shared/HAL/src/stm32f1xx_hal_tim_ex.c \
    src/main.c \
    src/stm32f1xx_hal_msp.c \
    src/stm32f1xx_it.c \
    src/system_stm32f1xx.c \
    src/startup_stm32f103xb.s \
    ../shared/src/plc_mod.cpp

DISTFILES +=
