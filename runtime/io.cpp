#include "io.h"
#include <cstring>

IO plc_inputs;
IO plc_outputs;

void IO::update_inputs(uint8_t* PIP)
{
    std::memcpy(m_io_data, PIP, IO_AREA_SIZE);
}

void IO::update_outputs(uint8_t* POP)
{
    std::memcpy(POP, m_io_data, IO_AREA_SIZE);
}

bool IO::read_bit(int offset, int bit_idx)
{
    return (m_io_data[offset] & (1 << bit_idx));
}

uint8_t IO::read_ubyte(int offset)
{
    return m_io_data[offset];
}

int8_t IO::read_byte(int offset)
{
    return m_io_data[offset];
}

uint16_t IO::read_word(int offset)
{
    return *((uint16_t*)&m_io_data[offset]);
}

int16_t IO::read_int(int offset)
{
    return *((int16_t*)&m_io_data[offset]);
}

uint32_t IO::read_dword(int offset)
{
    return *((uint32_t*)&m_io_data[offset]);
}

float IO::read_real(int offset)
{
    return *((float*)&m_io_data[offset]);
}
