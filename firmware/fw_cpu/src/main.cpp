#include "plc_control.h"

int main()
{
    PLC_CONTROL::set_stop(1);
    PLC_CONTROL::set_run(1);
    while (1)
    {
    }
    
    return 0;
}
