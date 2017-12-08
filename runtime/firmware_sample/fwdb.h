#ifndef FWDB_H
#define FWDB_H

#include <inttypes.h>

#pragma pack(push, 1)
struct DB1
{
    bool bit0: 1;
    bool bit1: 1;
    bool bit2: 1;
    bool bit3: 1;
    bool bit4: 1;
    bool bit5: 1;
    bool bit6: 1;
    bool bit7: 1;
    uint8_t byte1;
    bool bit8: 1;
};

struct DBHeader
{
    bool        initialized;
    uint32_t    last_update;
};
#pragma pack(pop)

class FWDB
{
public:
    static void init();
    static void read();
    static void save();

    static DB1 db1;
private:
    static DBHeader m_header;
};

#endif // FWDB_H
