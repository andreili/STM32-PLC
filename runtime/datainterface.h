#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

#include <cstring>

template<int data_size>
class DataInterface
{
public:
    bool read_bit(int offset, int bit_idx)
        { return (m_data[offset] & (1 << bit_idx)); }
    void write_bit(int offset, int bit_idx, bool val)
        {
            if (val)
                m_data[offset] |= (1 << bit_idx);
            else
                m_data[offset] &= ~(1 << bit_idx);
        }

    uint8_t read_ubyte(int offset)
        { return m_data[offset]; }
    void write_ubyte(int offset, uint8_t val)
        { m_data[offset] = val; }

    int8_t read_byte(int offset)
        { return m_data[offset]; }
    void write_byte(int offset, int8_t val)
        { m_data[offset] = val; }

    uint16_t read_word(int offset)
        { return *((uint16_t*)&m_data[offset]); }
    void write_word(int offset, uint16_t val)
        { *((uint16_t*)&m_data[offset]) = val; }

    int16_t read_int(int offset)
        { return *((int16_t*)&m_data[offset]); }
    void write_int(int offset, int16_t val)
        { *((int16_t*)&m_data[offset]) = val; }

    uint32_t read_dword(int offset)
        { return *((uint32_t*)&m_data[offset]); }
    void write_dword(int offset, uint32_t val)
        { *((uint32_t*)&m_data[offset]) = val; }

    float read_real(int offset)
        { return *((float*)&m_data[offset]); }
    void write_real(int offset, float val)
        { *((float*)&m_data[offset]) = val; }


    void update_inputs(uint8_t* PIP)
        { std::memcpy(this->m_data, PIP, IO_AREA_SIZE); }
    void update_outputs(uint8_t* POP)
        { std::memcpy(POP, this->m_data, IO_AREA_SIZE); }
protected:
    char    m_data[data_size];
};

#endif // DATAINTERFACE_H
