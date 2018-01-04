#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

class DataInterface
{
public:
    virtual bool read_bit(int offset, int bit_idx) = 0;

    virtual uint8_t read_ubyte(int offset) = 0;
    virtual int8_t read_byte(int offset) = 0;

    virtual uint16_t read_word(int offset) = 0;
    virtual int16_t read_int(int offset) = 0;

    virtual uint32_t read_dword(int offset) = 0;

    virtual float read_real(int offset) = 0;
};

#endif // DATAINTERFACE_H
