#ifndef DISKIODRIVER_H
#define DISKIODRIVER_H

#include "fatfs.h"
#include <inttypes.h>

class DiskIODriver
{
public:
    void init_gpio();
    uint8_t init(uint8_t);
    uint8_t status(uint8_t);
    uint8_t read(uint8_t, const uint8_t*, uint32_t, uint32_t);
    #if _USE_WRITE == 1
    uint8_t write(uint8_t, const uint8_t*, uint32_t, uint32_t);
    #endif
    #if _USE_IOCTL == 1
    uint8_t ioctl(uint8_t, uint8_t, void*);
    #endif
};

#endif // DISKIODRIVER_H
