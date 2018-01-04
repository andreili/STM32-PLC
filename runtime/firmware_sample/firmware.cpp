#include "firmware.h"
#include "fwdb.h"
#include "io.h"
#include <iostream>
#include "plcstate.h"

#define proc_none() (true)

#define run_OB10()  proc_none()
#define run_OB11()  proc_none()
#define run_OB12()  proc_none()
#define run_OB13()  proc_none()
#define run_OB14()  proc_none()
#define run_OB15()  proc_none()
#define run_OB16()  proc_none()
#define run_OB17()  proc_none()
#define run_OB20()  proc_none()
#define run_OB21()  proc_none()
#define run_OB22()  proc_none()
#define run_OB23()  proc_none()
#define run_OB30()  proc_none()
#define run_OB30()  proc_none()
#define run_OB31()  proc_none()
#define run_OB32()  proc_none()
#define run_OB33()  proc_none()
#define run_OB34()  proc_none()
#define run_OB35()  proc_none()
#define run_OB36()  proc_none()
#define run_OB37()  proc_none()
#define run_OB38()  proc_none()
#define run_OB40()  proc_none()
#define run_OB41()  proc_none()
#define run_OB42()  proc_none()
#define run_OB43()  proc_none()
#define run_OB44()  proc_none()
#define run_OB45()  proc_none()
#define run_OB46()  proc_none()
#define run_OB47()  proc_none()
#define run_OB48()  proc_none()
#define run_OB60()  proc_none()
#define run_OB80()  proc_none()
#define run_OB81()  proc_none()
#define run_OB82()  proc_none()
#define run_OB83()  proc_none()
#define run_OB85()  proc_none()
#define run_OB86()  proc_none()
#define run_OB87()  proc_none()
#define run_OB90()  proc_none()
#define run_OB100() proc_none()
#define run_OB101() proc_none()
#define run_OB102() proc_none()
#define run_OB121() proc_none()
#define run_OB122() proc_none()

Firmware::Firmware()
{}

bool Firmware::init()
{
    //TODO: reset FW state

    FWDB::read();

    return true;
}

bool Firmware::run_OB(EOB ob)
{
    switch (ob)
    {
    case EOB::OB_CYCLE_EXEC:
        return run_OB1();
        break;
    case EOB::OB_TIME_OF_DAY_INT_0:
        return run_OB10();
        break;
    case EOB::OB_TIME_OF_DAY_INT_1:
        return run_OB11();
        break;
    case EOB::OB_TIME_OF_DAY_INT_2:
        return run_OB12();
        break;
    case EOB::OB_TIME_OF_DAY_INT_3:
        return run_OB13();
        break;
    case EOB::OB_TIME_OF_DAY_INT_4:
        return run_OB14();
        break;
    case EOB::OB_TIME_OF_DAY_INT_5:
        return run_OB15();
        break;
    case EOB::OB_TIME_OF_DAY_INT_6:
        return run_OB16();
        break;
    case EOB::OB_TIME_OF_DAY_INT_7:
        return run_OB17();
        break;
    case EOB::OB_TIME_DELAY_INT_0:
        return run_OB20();
        break;
    case EOB::OB_TIME_DELAY_INT_1:
        return run_OB21();
        break;
    case EOB::OB_TIME_DELAY_INT_2:
        return run_OB22();
        break;
    case EOB::OB_TIME_DELAY_INT_3:
        return run_OB23();
        break;
    case EOB::OB_CYCLIC_INT_0:
        return run_OB30();
        break;
    case EOB::OB_CYCLIC_INT_1:
        return run_OB31();
        break;
    case EOB::OB_CYCLIC_INT_2:
        return run_OB32();
        break;
    case EOB::OB_CYCLIC_INT_3:
        return run_OB33();
        break;
    case EOB::OB_CYCLIC_INT_4:
        return run_OB34();
        break;
    case EOB::OB_CYCLIC_INT_5:
        return run_OB35();
        break;
    case EOB::OB_CYCLIC_INT_6:
        return run_OB36();
        break;
    case EOB::OB_CYCLIC_INT_7:
        return run_OB37();
        break;
    case EOB::OB_CYCLIC_INT_8:
        return run_OB38();
        break;
    case EOB::OB_HW_INT_0:
        return run_OB40();
        break;
    case EOB::OB_HW_INT_1:
        return run_OB41();
        break;
    case EOB::OB_HW_INT_2:
        return run_OB42();
        break;
    case EOB::OB_HW_INT_3:
        return run_OB43();
        break;
    case EOB::OB_HW_INT_4:
        return run_OB44();
        break;
    case EOB::OB_HW_INT_5:
        return run_OB45();
        break;
    case EOB::OB_HW_INT_6:
        return run_OB46();
        break;
    case EOB::OB_HW_INT_7:
        return run_OB47();
        break;
    case EOB::OB_HW_INT_8:
        return run_OB48();
        break;
    case EOB::OB_MULTICOMP_INT:
        return run_OB60();
        break;
    case EOB::OB_CYCLE_TIME_FAULT:
        return run_OB80();
        break;
    case EOB::OB_POWER_SUP_FAULT:
        return run_OB81();
        break;
    case EOB::OB_IO_POINT_FAULT_1:
        return run_OB82();
        break;
    case EOB::OB_IO_POINT_FAULT_2:
        return run_OB83();
        break;
    case EOB::OB_CPU_FAULT:
        return run_OB84();
        break;
    case EOB::OB_OB_NOT_LOADED_FAULT:
        return run_OB85();
        break;
    case EOB::OB_LOSS_OF_RACK_FAULT:
        return run_OB86();
        break;
    case EOB::OB_COMM_FAULT:
        return run_OB87();
        break;
    case EOB::OB_BACKGROUND_CYCLE:
        return run_OB90();
        break;
    case EOB::OB_COMPLETE_RESTART:
        return run_OB100();
        break;
    case EOB::OB_RESTART:
        return run_OB101();
        break;
    case EOB::OB_COLD_RESTART:
        return run_OB102();
        break;
    case EOB::OB_PROGRAMMING_ERROR:
        return run_OB121();
        break;
    case EOB::OB_MOD_ACCESS_ERROR:
        return run_OB122();
        break;
    default:
        printf("Firmware: invalid OB index (%i)\n", (int)ob);
        return false;
    };
}

bool Firmware::run_OB1()
{
    //

    FWDB::db1.bit0 = plc_inputs.read_bit(0, 0);

    return true;
}

bool Firmware::run_OB84()
{
    PLCState::fault_relay_ON();

    //TODO: get switch value
    if (true)
    {
        printf("Software error, STOP\n");
        PLCState::to_fault();
    }
    else
    {
        printf("Software error, RESTART\n");
        PLCState::init();
    }
    return true;
}
