#ifndef PLC_DATA_INTERFACE_H
#define PLC_DATA_INTERFACE_H

#include <inttypes.h>
#include "plc_conf.h"

typedef struct
{
    uint8_t b0 : 1;
    uint8_t b1 : 1;
    uint8_t b2 : 1;
    uint8_t b3 : 1;
    uint8_t b4 : 1;
    uint8_t b5 : 1;
    uint8_t b6 : 1;
    uint8_t b7 : 1;
} mem_byte_t;

#ifdef PLC_BE
static __inline uint16_t __bswap16(uint16_t _x)
{

    return ((uint16_t)((_x >> 8) | ((_x << 8) & 0xff00)));
}

static __inline uint32_t __bswap32(uint32_t _x)
{

    return ((uint32_t)((_x >> 24) | ((_x >> 8) & 0xff00) |
        ((_x << 8) & 0xff0000) | ((_x << 24) & 0xff000000)));
}

#define BO_WORD(x) __builtin_bswap16(x)
#define BO_DWORD(x) __builtin_bswap32(x)
#else
#define BO_WORD(x) x
#define BO_DWORD(x) x
#endif

class PLCDataInterface
{
public:
    PLCDataInterface(uint8_t *mem) { m_data = mem; }

    bool read_bit(int addr, int bit) { return ( (m_data[addr] >> bit) & 1 ); }
    void set_bit(int addr, int bit) { m_data[addr] |= 1 << bit; }
    void set_bit(int addr, int bit, bool val) { m_data[addr] ^= (-val ^ m_data[addr]) & (1 << bit); }
    void clr_bit(int addr, int bit) { m_data[addr] &= ~(1 << bit); }

    uint8_t read_byte(int addr) { return m_data[addr]; }
    void write_byte(int addr, uint8_t val) { m_data[addr] = val; }

    int16_t read_word(int addr) { return BO_WORD(*((int16_t*)&m_data[addr])); }
    void write_word(int addr, int16_t val) { *((int16_t*)&m_data[addr]) = BO_WORD(val); }

    int32_t read_dword(int addr) { return BO_DWORD(*((int32_t*)&m_data[addr])); }
    void write_dword(int addr, int32_t val) { *((int32_t*)&m_data[addr]) = BO_DWORD(val); }

    float read_real(int addr) { return (float)BO_DWORD(*((int32_t*)&m_data[addr])); }
    void write_real(int addr, float val) { *((int32_t*)&m_data[addr]) = BO_DWORD((int32_t)val); }
private:
    uint8_t *m_data;
    int32_t     m_top_addr;
    int32_t     m_top_bit;
};

#endif // PLC_DATA_INTERFACE_H
