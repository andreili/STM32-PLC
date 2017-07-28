#ifndef MEMMANAGER_H
#define MEMMANAGER_H

#include "stm32_conf.h"

#define INT_SIZE 32

#define RAM_SECTOR_SIZE 1024
#define BITMAP_SIZE (STM32_SDRAM_SIZE_MB / RAM_SECTOR_SIZE / INT_SIZE)
#define BITMAP_SIZE_BITS (BITMAP_SIZE * INT_SIZE)

class MemManager
{
public:
    static void init();
    static void* mem_allocate(int32_t size);
    static void mem_deallocate(void* object);
private:
    static uint8_t* m_data;
    static uint32_t m_bitmap[BITMAP_SIZE];

    static void map_set_bit(int32_t pos, int32_t flag);
    static int32_t get_free_block_count(uint32_t position, int32_t needed);
    static void set_blocks_used(uint32_t position, uint32_t count);

    static uint32_t NumberOfSetBits(uint32_t i);
    static uint32_t get_used_size();
    static uint32_t get_free_size();
};

#endif // MEMMANAGER_H
