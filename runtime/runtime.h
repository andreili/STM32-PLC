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
    Firmware    *m_firmware;
    PLCBus      *m_bus;

    ModuleInfo  *m_modules;
    uint32_t    m_modules_count;

    bool load_hw_config();
    bool load_firmware();
};

#endif // RUNTIME_H
