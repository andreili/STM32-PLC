#include "plcbus.h"
#include "io.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <cstring>
#include "plcstate.h"

#ifdef FPGA_ALLOW
#include "hps_0_arm_a9_0.h"
#define RE_SET (*((char*)LEDS_BASE) |= (1 << 7))
#define RE_CLR (*((char*)LEDS_BASE) &= ~(1 << 7))
#else
#define RE_SET
#define RE_CLR
#endif

std::mutex mtx_IO;

bool PLCBus::init(ModuleInfo* modules, uint32_t count)
{
    m_modules_list = modules;
    m_count = count;

    if (!init_UART())
        return false;
    if (!search_modules())
        return false;
    return true;
}

void PLCBus::copy_inputs()
{
    mtx_IO.lock();
    IO::update_inputs(m_PIP);
    mtx_IO.unlock();
}

void PLCBus::copy_outputs()
{
    mtx_IO.lock();
    IO::update_outputs(m_POP);
    mtx_IO.unlock();
}

void PLCBus::bus_proc()
{
    mtx_IO.lock();

    // read inputs
    for (uint32_t i=0 ; i<m_count ; ++i)
    {
        ModuleInfo* module = &m_modules_list[i];
        if ((module->input_size == 0) &&
            (module->rack != 0))    // another racks - only from communications!
            continue;

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        RE_SET;

        m_send.from = 0;
        m_send.to = m_modules_list[i].rack_idx;
        m_send.request = EBusRequest::READ_INPUTS;
        m_send.data_size = 0;

        write(m_bus_dev, &m_send, sizeof(BusMessage));

        m_recv.request = EBusRequest::UNKNOWN;
        m_recv.reply = EBusReply::UNKNOWN;

        RE_CLR;
        //std::this_thread::sleep_for(std::chrono::milliseconds(BUS_WAIT_TIME_MS));
        read(m_bus_dev, &m_recv, sizeof(BusMessage));

        if (m_recv.request == EBusRequest::UNKNOWN)
        {
            PLCState::to_error();
            printf("Unable to read inputs from module #%i\n", module->rack_idx);
            break;
        }

        switch (m_recv.reply)
        {
        case EBusReply::UNKNOWN:
            PLCState::to_error();
            printf("Invlid reply (read inputs) from from module #%i\n", module->rack_idx);
            return;
        case EBusReply::OK:
            std::memcpy(&m_PIP[module->input_start], &m_recv.data, module->input_size);
            printf("Read inputs from module (index:%i):\n",
                   module->rack_idx);

            // check error on module
            if (m_recv.module_info.state.fault)
                PLCState::to_error();
            break;
        case EBusReply::FAIL:
            return;
        }
    }

    // write outputs
    for (uint32_t i=0 ; i<m_count ; ++i)
    {
        ModuleInfo* module = &m_modules_list[i];
        if ((module->output_size == 0) &&
            (module->rack != 0))    // another racks - only from communications!
            continue;

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        RE_SET;

        m_send.from = 0;
        m_send.to = module->rack_idx;
        m_send.request = EBusRequest::WRITE_OUTPUTS;
        m_send.data_size = module->output_size;
        std::memcpy(&m_send.data, &m_POP[module->output_start], module->output_size);

        write(m_bus_dev, &m_send, sizeof(BusMessage));

        m_recv.request = EBusRequest::UNKNOWN;
        m_recv.reply = EBusReply::UNKNOWN;

        RE_CLR;
        //std::this_thread::sleep_for(std::chrono::milliseconds(BUS_WAIT_TIME_MS));
        read(m_bus_dev, &m_recv, sizeof(BusMessage));

        if (m_recv.request == EBusRequest::UNKNOWN)
        {
            PLCState::to_error();
            printf("Unable to write outputs to module #%i\n", module->rack_idx);
            break;
        }

        switch (m_recv.reply)
        {
        case EBusReply::UNKNOWN:
            PLCState::to_error();
            printf("Invlid reply (write outputs) from from module #%i\n", module->rack_idx);
            return;
        case EBusReply::OK:
            // check error on module
            if (m_recv.module_info.state.fault)
                PLCState::to_error();
            break;
        case EBusReply::FAIL:
            return;
        }
    }

    mtx_IO.unlock();
}

bool PLCBus::init_UART()
{
    m_bus_dev = open(BUS_UART_DEVICE, O_RDWR | O_NOCTTY | O_NDELAY);
    if (m_bus_dev == -1)
    {
        return false;
    }
    struct termios options;
    tcgetattr(m_bus_dev, &options);
    options.c_cflag = BUS_UART_BRATE | CS8 | CLOCAL | CREAD;    //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(m_bus_dev, TCIFLUSH);
    tcsetattr(m_bus_dev, TCSANOW, &options);

    return true;
}

bool PLCBus::search_modules()
{
    m_send.from = 0;
    m_send.to = (uint32_t)-1;
    m_send.request = EBusRequest::FIND_DEVICE;
    m_send.data_size = 0;

    for (uint32_t i=0 ; i<m_count ; ++i)
    {
        ModuleInfo* module = &m_modules_list[i];
        // skip self
        if ((module->rack == 0) &&
            (module->rack_idx == 0) &&
            ((module->type & MODULE_TYPE_CPU) ==MODULE_TYPE_CPU))
        {
            module->finded = true;
            continue;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        //copy module info to send buffer
        m_send.to = m_modules_list[i].rack_idx;
        m_send.module_info = m_modules_list[i];

        RE_SET;
        write(m_bus_dev, &m_send, sizeof(BusMessage));

        m_recv.request = EBusRequest::UNKNOWN;
        m_recv.reply = EBusReply::UNKNOWN;

        RE_CLR;
        //std::this_thread::sleep_for(std::chrono::milliseconds(BUS_WAIT_TIME_MS));
        read(m_bus_dev, &m_recv, sizeof(BusMessage));

        if (m_recv.request == EBusRequest::UNKNOWN)
            break;

        switch (m_recv.reply)
        {
        case EBusReply::UNKNOWN:
            PLCState::to_error();
            return true;
        case EBusReply::OK:
            m_modules_list[i].finded = true;
            break;
        case EBusReply::FAIL:
            return false;
        }
    }
    return true;
}
