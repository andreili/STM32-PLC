#include "runtime.h"
#include "firmware_sample/firmware.h"
#include <plcstate.h>
#include "plcbus.h"
#include <thread>
#include <mutex>

Firmware    Runtime::m_firmware;
PLCBus      Runtime::m_bus;

std::mutex  mtx_run_cycle;

Runtime::Runtime()
{}

void Runtime::run()
{
    PLCState::init();
    PLCState::reset_fault_relay();
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
            fw_loaded = m_bus.load_config();
            if (fw_loaded)
                fw_loaded = load_firmware();

            if (fw_loaded)
                PLCState::to_bus_init();
            else
                PLCState::to_wait_fw_in_plc();
            break;

        case EPLCState::WAIT_FW:
            printf("State: Waiting firmware\n");
            printf("Unable to loading firmware, wait uploading from remote device\n");
            //TODO: uploading wait
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            break;

        case EPLCState::BUS_INIT:
            printf("State: BUS initialization\n");
            if (!m_bus.init())
            {
                printf("Failed initialize BUS\n");
                PLCState::to_fault();
            }
            else
                PLCState::to_run();
            break;

        case EPLCState::RUN:
            mtx_run_cycle.lock();
            //TODO: to STOP (switch, command)
            if (false)
            {
                PLCState::to_stop();
                break;
            }

            //TODO: to STOP (download)
            if (false)
            {
                PLCState::to_stop();
                PLCState::to_load_fw_in_plc();
                break;
            }
            mtx_run_cycle.unlock();

            //TODO: to comm thread
            m_bus.bus_proc();

            //TODO: to cycle thread
            mtx_run_cycle.lock();
            m_bus.copy_inputs();
            if (!m_firmware.run_OB(EOB::OB_CYCLE_EXEC))
            {
                m_firmware.run_OB(EOB::OB_CPU_FAULT);
            }
            m_bus.copy_outputs();
            mtx_run_cycle.unlock();
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

bool Runtime::load_firmware()
{
    //TODO
    // added to runtime project.
    if (!m_firmware.init())
        return false;
    return true;
}
