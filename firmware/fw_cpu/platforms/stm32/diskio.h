#ifndef DISKIO_H
#define DISKIO_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "fatfs.h"

DSTATUS disk_status(BYTE pdrv);
DSTATUS disk_initialize(BYTE pdrv);
DSTATUS disk_read(BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
#if _USE_WRITE == 1
DSTATUS disk_write(BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
#endif
#if _USE_IOCTL == 1
DSTATUS disk_ioctl(BYTE pdrv, BYTE cmd, void *buff);
#endif

#ifdef __cplusplus
}
#endif

#endif // DISKIO_H
