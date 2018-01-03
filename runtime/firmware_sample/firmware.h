#ifndef FIRMWARE_H
#define FIRMWARE_H

enum class EOB
{
    OB_CYCLE_EXEC           = 1,
    OB_TIME_OF_DAY_INT_0    = 10,
    OB_TIME_OF_DAY_INT_1    = 11,
    OB_TIME_OF_DAY_INT_2    = 12,
    OB_TIME_OF_DAY_INT_3    = 13,
    OB_TIME_OF_DAY_INT_4    = 14,
    OB_TIME_OF_DAY_INT_5    = 15,
    OB_TIME_OF_DAY_INT_6    = 16,
    OB_TIME_OF_DAY_INT_7    = 17,
    OB_TIME_DELAY_INT_0     = 20,
    OB_TIME_DELAY_INT_1     = 21,
    OB_TIME_DELAY_INT_2     = 22,
    OB_TIME_DELAY_INT_3     = 23,
    OB_CYCLIC_INT_0         = 30,
    OB_CYCLIC_INT_1         = 31,
    OB_CYCLIC_INT_2         = 32,
    OB_CYCLIC_INT_3         = 33,
    OB_CYCLIC_INT_4         = 34,
    OB_CYCLIC_INT_5         = 35,
    OB_CYCLIC_INT_6         = 36,
    OB_CYCLIC_INT_7         = 37,
    OB_CYCLIC_INT_8         = 38,
    OB_HW_INT_0             = 40,
    OB_HW_INT_1             = 41,
    OB_HW_INT_2             = 42,
    OB_HW_INT_3             = 43,
    OB_HW_INT_4             = 44,
    OB_HW_INT_5             = 45,
    OB_HW_INT_6             = 46,
    OB_HW_INT_7             = 47,
    OB_HW_INT_8             = 48,
    OB_MULTICOMP_INT        = 60,
    OB_CYCLE_TIME_FAULT     = 80,
    OB_POWER_SUP_FAULT      = 81,
    OB_IO_POINT_FAULT_1     = 82,
    OB_IO_POINT_FAULT_2     = 83,
    OB_CPU_FAULT            = 84,
    OB_OB_NOT_LOADED_FAULT  = 85,
    OB_LOSS_OF_RACK_FAULT   = 86,
    OB_COMM_FAULT           = 87,
    OB_BACKGROUND_CYCLE     = 90,
    OB_COMPLETE_RESTART     = 100,
    OB_RESTART              = 101,
    OB_COLD_RESTART         = 102,
    OB_PROGRAMMING_ERROR    = 121,
    OB_MOD_ACCESS_ERROR     = 122,
};

class Firmware
{
public:
    Firmware();

    bool init();
    bool run_OB(EOB ob);
private:
    bool run_OB1();
    bool run_OB84();
};

#endif // FIRMWARE_H
