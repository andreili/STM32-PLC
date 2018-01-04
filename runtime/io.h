#ifndef IO_H
#define IO_H

#include "settings.h"
#include "datainterface.h"

class IO : public DataInterface
{
public:
    bool read_bit(int offset, int bit_idx);

    uint8_t read_ubyte(int offset);
    int8_t read_byte(int offset);

    uint16_t read_word(int offset);
    int16_t read_int(int offset);

    uint32_t read_dword(int offset);

    float read_real(int offset);

    void update_inputs(uint8_t* PIP);
    void update_outputs(uint8_t* POP);
private:
    uint8_t  m_io_data[IO_AREA_SIZE];
};

extern IO plc_inputs;
extern IO plc_outputs;

#endif // IO_H
