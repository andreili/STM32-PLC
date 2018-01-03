#ifndef RUNTIME_H
#define RUNTIME_H

#include "settings.h"

class Firmware;
class PLCBus;
struct ModuleInfo;

class Runtime
{
public:
    Runtime();

    void run();
private:
    static  Firmware    m_firmware;
    static  PLCBus      m_bus;

    static  ModuleInfo  m_modules[BUS_MAX_MODULES];
    uint32_t    m_modules_count;

    bool load_hw_config();
    bool load_firmware();
};

#endif // RUNTIME_H
