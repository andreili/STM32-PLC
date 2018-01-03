#include "firmware.h"
#include "fwdb.h"
#include "io.h"

Firmware::Firmware()
{}

bool Firmware::init()
{
    FWDB::read();

    return true;
}

bool Firmware::run_cycle()
{
    //

    FWDB::db1.bit0 = IO::read_bit(EIOArea::INPUT, 0, 0);

    return true;
}
