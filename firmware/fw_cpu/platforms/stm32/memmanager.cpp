#include "memmanager.h"
#include "stm32_sdram.h"
#include "my_func.h"

uint8_t* MemManager::m_data;
uint32_t MemManager::m_bitmap[BITMAP_SIZE];

#define IS_MAP_BIT_SET(pos) (m_bitmap[(pos) / INT_SIZE] & (1 << ((pos) % INT_SIZE)))
#define MEM_BANK_ADDR SDRAM_BASE_BANK1

void MemManager::init()
{
    m_data = (uint8_t*)MEM_BANK_ADDR;
    memset((uint8_t*)m_bitmap, 0, sizeof(uint32_t) * BITMAP_SIZE);
}

void* MemManager::mem_allocate(int32_t size)
{
    //caclulate blocks count need to allocate
    uint32_t need_chains = (size / RAM_SECTOR_SIZE) + ((size % RAM_SECTOR_SIZE) > 0 ? 1 : 0);
    //find free blocks chain
    for (uint32_t i=0 ; i<BITMAP_SIZE_BITS ; ++i)
    {
        if ((i) && IS_MAP_BIT_SET(i - 1))
            continue;
        int32_t start_chain = get_free_block_count(i, need_chains);
        if (start_chain != -1)
        {
            set_blocks_used(i, need_chains);
            memset(&m_data[i * RAM_SECTOR_SIZE], 0, need_chains * RAM_SECTOR_SIZE);
            return &m_data[i * RAM_SECTOR_SIZE];
        }
    }
    Error_Handler();;
    return 0;
}

void MemManager::map_set_bit(int32_t pos, int32_t flag)
{
    int32_t elementNo = pos / INT_SIZE;
    int32_t bitNo = pos % INT_SIZE;
    if (flag)
        m_bitmap[elementNo] |= (1 << bitNo);
    else
        m_bitmap[elementNo] &= ~(1 << bitNo);
}

int32_t MemManager::get_free_block_count(uint32_t position, int32_t needed)
{
    int32_t ret = -1;

    while (!IS_MAP_BIT_SET(position))
    {
        ++ret;

        if (ret == needed)
            return ret;
        ++position;
    }
    return ret;
}

void MemManager::set_blocks_used(uint32_t position, uint32_t count)
{
    while (count--)
    {
        map_set_bit(position, 1);
        ++position;
    }
    map_set_bit(position, 0);
}

void MemManager::mem_deallocate(void* object)
{
    if (((uint32_t)object < MEM_BANK_ADDR) || ((uint32_t)object > (MEM_BANK_ADDR + STM32_SDRAM_SIZE_MB)))
        return;

    //calculate block index
    int block_idx = ((uint32_t)object - MEM_BANK_ADDR) / RAM_SECTOR_SIZE;
    while (IS_MAP_BIT_SET(block_idx))
    {
        map_set_bit(block_idx, 0);
        ++block_idx;
    }
}

uint32_t MemManager::NumberOfSetBits(uint32_t i)
{
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

uint32_t MemManager::get_used_size()
{
    uint32_t used = 0;
    for (uint32_t i=0 ; i<BITMAP_SIZE ; ++i)
        used += NumberOfSetBits(m_bitmap[i]);
    return used * RAM_SECTOR_SIZE;
}

uint32_t MemManager::get_free_size()
{
    return (STM32_SDRAM_SIZE_MB - get_used_size());
}
