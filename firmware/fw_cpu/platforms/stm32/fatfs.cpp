#include "fatfs.h"
#include "stm32_conf.h"
#include STM32_FATFS_DRIVER_INC

DiskDrv FAT_FS::m_disk;

extern STM32_FATFS_DRIVER sd_driver;
char SD_path[4];

void FAT_FS::init()
{
    m_disk.nbr = 0;
    link_driver((DiskIODriver*)&sd_driver, SD_path, 0);
}

DSTATUS FAT_FS::disk_status(BYTE pdrv)
{
    return ((STM32_FATFS_DRIVER*)m_disk.drv[pdrv])->status(m_disk.lun[pdrv]);
}

DSTATUS FAT_FS::disk_initialize(BYTE pdrv)
{
    if (m_disk.is_initialized[pdrv] == 0)
    {
        if (((STM32_FATFS_DRIVER*)m_disk.drv[pdrv])->init(m_disk.lun[pdrv]) == RES_OK)
        {
            m_disk.is_initialized[pdrv] = 1;
            return RES_OK;
        }
    }
    return RES_ERROR;
}

DSTATUS FAT_FS::disk_read(BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
    return ((STM32_FATFS_DRIVER*)m_disk.drv[pdrv])->read(m_disk.lun[pdrv], buff, sector, count);
}

#if _USE_WRITE == 1
DSTATUS FAT_FS::disk_write(BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
    return ((STM32_FATFS_DRIVER*)m_disk.drv[pdrv])->write(m_disk.lun[pdrv], buff, sector, count);
}
#endif
#if _USE_IOCTL == 1
DSTATUS FAT_FS::disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
    return((STM32_FATFS_DRIVER*)m_disk.drv[pdrv])->ioctl(m_disk.lun[pdrv], cmd, buff);
}
#endif

void FAT_FS::link_driver(DiskIODriver *drv, char *path, uint8_t lun)
{
    if (m_disk.nbr <= _VOLUMES)
    {
        m_disk.is_initialized[m_disk.nbr] = 0;
        m_disk.drv[m_disk.nbr] = drv;
        m_disk.lun[m_disk.nbr] = lun;
        uint8_t num = m_disk.nbr++;
        path[0] = num + '0';
        path[1] = ':';
        path[2] = '/';
        path[3] = 0;
    }
}

extern "C"
{

DSTATUS disk_status(BYTE pdrv)
{
    return FAT_FS::disk_status(pdrv);
}

DSTATUS disk_initialize(BYTE pdrv)
{
    return FAT_FS::disk_initialize(pdrv);
}

DSTATUS disk_read(BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
    return FAT_FS::disk_read(pdrv, buff, sector, count);
}

#if _USE_WRITE == 1
DSTATUS disk_write(BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
    return FAT_FS::disk_write(pdrv, buff, sector, count);
}

#endif
#if _USE_IOCTL == 1
DSTATUS disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
    return FAT_FS::disk_ioctl(pdrv, cmd, buff);
}
#endif

}
