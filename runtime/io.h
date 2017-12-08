#ifndef IO_H
#define IO_H

#include "settings.h"

enum class EIOArea
{
    INPUT,
    OUTPUT,
};

class IO
{
public:
    static void update_inputs(uint8_t* PIP);
    static void update_outputs(uint8_t* POP);

    static bool read_bit(EIOArea area, int offset, int bit_idx);

    static uint8_t read_ubyte(EIOArea area, int offset);
    static int8_t read_byte(EIOArea area, int offset);

    static uint16_t read_word(EIOArea area, int offset);
    static int16_t read_int(EIOArea area, int offset);

    static uint32_t read_dword(EIOArea area, int offset);

    static float read_real(EIOArea area, int offset);
private:
    static uint8_t  m_inputs[IO_AREA_SIZE];
    static uint8_t  m_outputs[IO_AREA_SIZE];
};

#endif // IO_H
