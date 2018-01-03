#include "runtime.h"
#include "firmware_sample/firmware.h"
#include <plcstate.h>
#include "plcbus.h"
#include <json/config.h>
#include <json/json.h>
#include <iostream>
#include <fstream>
#include <thread>

Firmware    Runtime::m_firmware;
PLCBus      Runtime::m_bus;
ModuleInfo  Runtime::m_modules[BUS_MAX_MODULES];

Runtime::Runtime()
{}

void Runtime::run()
{
    PLCState::init();
    PLCState::to_full_stop();

    //TODO: start server (debug + monitoring)

    bool fw_loaded;
    while (1)
    {
        switch (PLCState::get_state())
        {
        case EPLCState::INIT:
            printf("State: Initialization\n");
            PLCState::to_load_fw_in_plc();
            break;

        case EPLCState::LOAD_FW:
            printf("State: Firmware loading\n");
            fw_loaded = load_hw_config();
            if (fw_loaded)
                fw_loaded = load_firmware();

            if (fw_loaded)
                PLCState::to_bus_init();
            else
                PLCState::to_wait_fw_in_plc();
            break;

        case EPLCState::WAIT_FW:
            printf("State: Waiting firmware\n");
            printf("Unable to loading firmware, wait uloading from remote device\n");
            //TODO: uploading wait
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            break;

        case EPLCState::BUS_INIT:
            printf("State: BUS initialization\n");
            if (!m_bus.init(m_modules, m_modules_count))
            {
                printf("Failed initialize BUS\n");
                PLCState::to_fault();
            }
            else
                PLCState::to_run();
            break;

        case EPLCState::RUN:
            //TODO: to comm thread
            m_bus.bus_proc();

            //TODO: to cycle thread
            m_bus.copy_inputs();
            if (!m_firmware.run_OB(EOB::OB_CYCLE_EXEC))
                PLCState::to_fault();
            m_bus.copy_outputs();

            //TODO: to stop
            break;

        case EPLCState::STOP:
            //TODO: to wait FW
            //TODO: to run
            break;

        case EPLCState::FAULT:
            //TODO: to initialize (stop switch changed to RUN)
            break;
        }
    }
}

bool Runtime::load_hw_config()
{
    std::ifstream str(RT_ROOT_PATH "hw.json", std::ifstream::binary);
    if (!str)
    {
        return false;
    }
    str.seekg (0, str.end);
    int length = str.tellg();
    str.seekg (0, str.beg);
    char* buf = new char[length];
    str.read(buf, length);
    str.close();

    Json::Value root;
    Json::CharReaderBuilder b;
    b.settings_["allowSingleQuotes"] = true;
    Json::CharReader* reader(b.newCharReader());
    JSONCPP_STRING errs;
    if (!reader->parse(buf, buf + length, &root, &errs))
    {
        std::cout << errs << std::endl;
        return false;
    }

    delete reader;
    delete[] buf;

    //TODO: load hardware
    Json::Value &modules = root["modules"];
    m_modules_count = modules.size();
    for (uint32_t i=0 ; i<m_modules_count ; ++i)
    {
        Json::Value &module = modules[i];
        m_modules[i].type = module["type"].asUInt();
        m_modules[i].sub_type = module["sub_type"].asUInt();

        m_modules[i].rack = module["rack"].asUInt();
        m_modules[i].rack_idx = module["rack_idx"].asUInt();

        m_modules[i].input_start = module["istart"].asUInt();
        m_modules[i].input_size = module["isize"].asUInt();
        m_modules[i].output_start = module["ostart"].asUInt();
        m_modules[i].output_size = module["osize"].asUInt();

        m_modules[i].state.initialized = false;
        m_modules[i].state.overrun = false;
        m_modules[i].state.fault = false;

        //TODO: module-specific parameters
    }

    return true;
}

bool Runtime::load_firmware()
{
    //TODO
    // added to runtime project.
    if (!m_firmware.init())
        return false;
    return true;
}
