#include "fwdb.h"

DB1 FWDB::db1;
DBHeader FWDB::m_header;

void FWDB::init()
{
    db1.bit0 = false;
    db1.bit1 = false;
    db1.bit2 = false;
    db1.bit3 = false;
    db1.bit4 = false;
    db1.bit5 = false;
    db1.bit6 = false;
    db1.bit7 = false;
    db1.byte1 = false;
    db1.bit8 = false;
}

void FWDB::read()
{
    //TODO: read from flash

    if (!m_header.initialized)
    {
        init();
        save();
    }
}

void FWDB::save()
{
    //TODO: save to flash
}
