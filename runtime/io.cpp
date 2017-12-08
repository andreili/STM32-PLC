#include "io.h"
#include <cstring>

uint8_t  IO::m_inputs[IO_AREA_SIZE];
uint8_t  IO::m_outputs[IO_AREA_SIZE];

void IO::update_inputs(uint8_t* PIP)
{
    std::memcpy(m_inputs, PIP, IO_AREA_SIZE);
}

void IO::update_outputs(uint8_t* POP)
{
    std::memcpy(POP, m_outputs, IO_AREA_SIZE);
}

#define SELECT_AREA() \
    uint8_t *data; \
    if (area == EIOArea::INPUT) \
        data = m_inputs; \
    else \
        data = m_outputs;

bool IO::read_bit(EIOArea area, int offset, int bit_idx)
{
    SELECT_AREA();
    return (data[offset] & (1 << bit_idx));
}

uint8_t IO::read_ubyte(EIOArea area, int offset)
{
    SELECT_AREA();
    return data[offset];
}

int8_t IO::read_byte(EIOArea area, int offset)
{
    SELECT_AREA();
    return data[offset];
}

uint16_t IO::read_word(EIOArea area, int offset)
{
    SELECT_AREA();
    return *((uint16_t*)&data[offset]);
}

int16_t IO::read_int(EIOArea area, int offset)
{
    SELECT_AREA();
    return *((int16_t*)&data[offset]);
}

uint32_t IO::read_dword(EIOArea area, int offset)
{
    SELECT_AREA();
    return *((uint32_t*)&data[offset]);
}

float IO::read_real(EIOArea area, int offset)
{
    SELECT_AREA();
    return *((float*)&data[offset]);
}
