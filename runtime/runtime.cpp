#include "runtime.h"
#include "firmware_sample/firmware.h"
#include "plcbus.h"
#include <json/config.h>
#include <json/json.h>
#include <iostream>
#include <fstream>

Runtime::Runtime()
{}

void Runtime::run()
{
    //TODO: run server

    if (!load_hw_config())
        return;
    if (!load_firmware())
        return;

    m_bus = new PLCBus();
    if (!m_bus->init(m_modules, m_modules_count))
        return;

    while (1)
    {
        //TODO: to comm thread
        m_bus->bus_proc();

        //TODO: to cycle thread
        m_bus->copy_inputs();
        m_firmware->run_cycle();
        m_bus->copy_outputs();
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
    m_modules = new ModuleInfo[BUS_MAX_MODULES];
    m_modules_count = modules.size();
    for (uint32_t i=0 ; i<m_modules_count ; ++i)
    {
        Json::Value &module = modules[i];
        m_modules[i].type = module["type"].asUInt();
        m_modules[i].sub_type = module["sub_type"].asUInt();
        m_modules[i].rack_idx = module["rack_idx"].asUInt();

        m_modules[i].input_start = module["istart"].asUInt();
        m_modules[i].input_size = module["isize"].asUInt();
        m_modules[i].output_start = module["ostart"].asUInt();
        m_modules[i].output_size = module["osize"].asUInt();

        //TODO: module-specific parameters
    }

    return true;
}

bool Runtime::load_firmware()
{
    //TODO
    // added to runtime project.
    m_firmware = new Firmware();
    if (!m_firmware->init())
        return false;
    return true;
}
