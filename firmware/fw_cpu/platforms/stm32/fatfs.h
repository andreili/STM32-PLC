#ifndef FATFS_H
#define FATFS_H

#include "ffconf.h"
#include "integer.h"

#define _USE_WRITE	1	/* 1: Enable disk_write function */
#define _USE_IOCTL	1	/* 1: Enable disk_ioctl fucntion */

typedef BYTE	DSTATUS;

/* Results of Disk Functions */
typedef enum {
    RES_OK = 0,		/* 0: Successful */
    RES_ERROR,		/* 1: R/W Error */
    RES_WRPRT,		/* 2: Write Protected */
    RES_NOTRDY,		/* 3: Not Ready */
    RES_PARERR		/* 4: Invalid Parameter */
} DRESULT;

/* Disk Status Bits (DSTATUS) */

#define STA_NOINIT		0x01	/* Drive not initialized */
#define STA_NODISK		0x02	/* No medium in the drive */
#define STA_PROTECT		0x04	/* Write protected */

/* Command code for disk_ioctrl fucntion */

/* Generic command (Used by FatFs) */
#define CTRL_SYNC		0	/* Complete pending write process (needed at _FS_READONLY == 0) */
#define GET_SECTOR_COUNT	1	/* Get media size (needed at _USE_MKFS == 1) */
#define GET_SECTOR_SIZE		2	/* Get sector size (needed at _MAX_SS != _MIN_SS) */
#define GET_BLOCK_SIZE		3	/* Get erase block size (needed at _USE_MKFS == 1) */
#define CTRL_TRIM		4	/* Inform device that the data on the block of sectors is no longer used (needed at _USE_TRIM == 1) */

/* Generic command (Not used by FatFs) */
#define CTRL_POWER			5	/* Get/Set power status */
#define CTRL_LOCK			6	/* Lock/Unlock media removal */
#define CTRL_EJECT			7	/* Eject media */
#define CTRL_FORMAT			8	/* Create physical format on the media */

/* MMC/SDC specific ioctl command */
#define MMC_GET_TYPE		10	/* Get card type */
#define MMC_GET_CSD			11	/* Get CSD */
#define MMC_GET_CID			12	/* Get CID */
#define MMC_GET_OCR			13	/* Get OCR */
#define MMC_GET_SDSTAT		14	/* Get SD status */

/* ATA/CF specific ioctl command */
#define ATA_GET_REV			20	/* Get F/W revision */
#define ATA_GET_MODEL		21	/* Get model name */
#define ATA_GET_SN			22	/* Get serial number */

#ifdef __cplusplus

class DiskIODriver;

typedef struct
{
    uint32_t        is_initialized[_VOLUMES];
    DiskIODriver*   drv[_VOLUMES];
    uint8_t         lun[_VOLUMES];
    uint8_t         nbr;
} DiskDrv;

class FAT_FS
{
public:
    static void init();
    static DSTATUS disk_status(BYTE pdrv);
    static DSTATUS disk_initialize(BYTE pdrv);
    static DSTATUS disk_read(BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
    #if _USE_WRITE == 1
    static DSTATUS disk_write(BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
    #endif
    #if _USE_IOCTL == 1
    static DSTATUS disk_ioctl(BYTE pdrv, BYTE cmd, void *buff);
    #endif
private:
    static DiskDrv  m_disk;

    static void link_driver(DiskIODriver *drv, char *path, uint8_t lun);
};

#endif

#endif // FATFS_H
