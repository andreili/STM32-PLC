#include "runtime.h"
#include "firmware_sample/firmware.h"
#include <plcstate.h>
#include "plcbus.h"
#include <thread>
#include <mutex>
#include <chrono>

Firmware    Runtime::m_firmware;
PLCBus      Runtime::m_bus;

std::mutex  mtx_run_cycle;

#define CYCLE_TIME_START \
    auto cycle_start = std::chrono::high_resolution_clock::now();

#define CYCLE_TIME_END(time) \
    auto cycle_finish = std::chrono::high_resolution_clock::now(); \
    auto cycle_time = std::chrono::duration_cast<std::chrono::milliseconds>(cycle_finish - cycle_start).count(); \
    cycle_time = time - cycle_time; \
    if (cycle_time > 0) \
        std::this_thread::sleep_for(std::chrono::milliseconds(cycle_time));

Runtime::Runtime()
{}

void Runtime::run()
{
    PLCState::init();
    PLCState::reset_fault_relay();
    PLCState::to_full_stop();

    //TODO: start server (debug + monitoring)

    std::thread th_comm(&Runtime::comm_thread, this);
    std::thread th_main(&Runtime::main_thread, this);

    bool fw_loaded;
    // one cycle on 100ms
    while (1)
    {
        CYCLE_TIME_START;

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
                //PLCState::to_fault();
                PLCState::to_run(); //TODO: debug
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
            break;

        case EPLCState::STOP:
            //TODO: to wait FW
            //TODO: to run
            break;

        case EPLCState::FAULT:
            //TODO: to initialize (stop switch changed to RUN)
            break;
        }

        CYCLE_TIME_END(CYCLE_TIME_MMAIN);
    }

    th_comm.join();
    th_main.join();
}

bool Runtime::load_firmware()
{
    //TODO
    // added to runtime project.
    if (!m_firmware.init())
        return false;
    return true;
}

void Runtime::comm_thread()
{
    // one cycle on 100ms
    while (1)
    {
        CYCLE_TIME_START;
        if (PLCState::get_state() == EPLCState::RUN)
        {
            m_bus.bus_proc();
            // TODO: CP modules
        }
        CYCLE_TIME_END(CYCLE_TIME_COMM);
    }
}

void Runtime::main_thread()
{
    int cycle_counter = 0;
    while (1)
    {
        auto cycle_start = std::chrono::high_resolution_clock::now();

        mtx_run_cycle.lock();
        if (PLCState::get_state() == EPLCState::RUN)
        {
            m_bus.copy_inputs();
            if (!m_firmware.run_OB(EOB::OB_CYCLE_EXEC))
            {
                m_firmware.run_OB(EOB::OB_CPU_FAULT);
            }
            m_bus.copy_outputs();
        }
        mtx_run_cycle.unlock();

        if (PLCState::get_state() == EPLCState::RUN)
        {
            auto cycle_finish = std::chrono::high_resolution_clock::now();
            auto cycle_time = std::chrono::duration_cast<std::chrono::nanoseconds>(cycle_finish - cycle_start).count();
            if (++cycle_counter > 1000*10)
            {
                printf("\rCycle time: %lli ns.", cycle_time);
                cycle_counter = 0;
            }
        }
    }
}
