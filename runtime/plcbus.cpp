#include "plcbus.h"
#include "gpio.h"
#include "io.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <cstring>

std::mutex mtx_IO;

bool PLCBus::init(ModuleInfo* modules, uint32_t count)
{
    m_modules_list = modules;
    m_count = count;

    if (!init_UART())
        return false;
    if (!search_modules())
        return false;
    if (!search_comms())
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
        if (m_modules_list[i].rack_idx < BUS_COMM_START_IDX)
            GPIO::RE_modules_unset();
        else
            GPIO::RE_comms_unset();

        m_send.from = 0;
        m_send.to = m_modules_list[i].rack_idx;
        m_send.request = EBusRequest::READ_INPUTS;
        m_send.data_size = 0;

        if (m_modules_list[i].rack_idx < BUS_COMM_START_IDX)
            write(m_modules, &m_send, sizeof(BusMessage));
        else
            write(m_comms, &m_send, sizeof(BusMessage));

        m_recv.request = EBusRequest::UNKNOWN;
        m_recv.reply = EBusReply::UNKNOWN;

        if (m_modules_list[i].rack_idx < BUS_COMM_START_IDX)
            GPIO::RE_modules_set();
        else
            GPIO::RE_comms_set();
        std::this_thread::sleep_for(std::chrono::milliseconds(BUS_WAIT_TIME_MS));
        if (m_modules_list[i].rack_idx < BUS_COMM_START_IDX)
            read(m_modules, &m_recv, sizeof(BusMessage));
        else
            read(m_comms, &m_recv, sizeof(BusMessage));

        if (m_recv.request == EBusRequest::UNKNOWN)
            break;

        switch (m_recv.reply)
        {
        case EBusReply::UNKNOWN:
            return;
        case EBusReply::OK:
            std::memcpy(&m_PIP[m_modules_list[i].input_start], &m_recv.data, m_modules_list[i].input_size);
            break;
        case EBusReply::FAIL:
            return;
        }
    }

    // write outputs
    for (uint32_t i=0 ; i<m_count ; ++i)
    {
        if (m_modules_list[i].rack_idx < BUS_COMM_START_IDX)
            GPIO::RE_modules_unset();
        else
            GPIO::RE_comms_unset();

        m_send.from = 0;
        m_send.to = m_modules_list[i].rack_idx;
        m_send.request = EBusRequest::WRITE_OUTPUTS;
        m_send.data_size = m_modules_list[i].output_size;
        std::memcpy(&m_send.data, &m_POP[m_modules_list[i].output_start], m_modules_list[i].output_size);

        if (m_modules_list[i].rack_idx < BUS_COMM_START_IDX)
            write(m_modules, &m_send, sizeof(BusMessage));
        else
            write(m_comms, &m_send, sizeof(BusMessage));

        m_recv.request = EBusRequest::UNKNOWN;
        m_recv.reply = EBusReply::UNKNOWN;

        if (m_modules_list[i].rack_idx < BUS_COMM_START_IDX)
            GPIO::RE_modules_set();
        else
            GPIO::RE_comms_set();
        std::this_thread::sleep_for(std::chrono::milliseconds(BUS_WAIT_TIME_MS));
        if (m_modules_list[i].rack_idx < BUS_COMM_START_IDX)
            read(m_modules, &m_recv, sizeof(BusMessage));
        else
            read(m_comms, &m_recv, sizeof(BusMessage));

        if (m_recv.request == EBusRequest::UNKNOWN)
            break;

        switch (m_recv.reply)
        {
        case EBusReply::UNKNOWN:
            return;
        case EBusReply::OK:
            break;
        case EBusReply::FAIL:
            return;
        }
    }

    mtx_IO.unlock();
}

bool PLCBus::init_UART()
{
    m_modules = open(BUS_1_UART_DEVICE, O_RDWR | O_NOCTTY | O_NDELAY);
    if (m_modules == -1)
    {
        return false;
    }
    struct termios options;
    tcgetattr(m_modules, &options);
    options.c_cflag = BUS_UART_BRATE | CS8 | CLOCAL | CREAD;    //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(m_modules, TCIFLUSH);
    tcsetattr(m_modules, TCSANOW, &options);

    m_comms = open(BUS_2_UART_DEVICE, O_RDWR | O_NOCTTY | O_NDELAY);
    if (m_comms == -1)
    {
        return false;
    }
    tcgetattr(m_comms, &options);
    options.c_cflag = BUS_UART_BRATE | CS8 | CLOCAL | CREAD;    //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(m_comms, TCIFLUSH);
    tcsetattr(m_comms, TCSANOW, &options);

    return true;
}

bool PLCBus::search_modules()
{
    m_send.from = 0;
    m_send.to = (uint32_t)-1;
    m_send.request = EBusRequest::FIND_DEVICE;
    m_send.data_size = 0;

    GPIO::CS_modules_unset();

    for (uint32_t i=0 ; i<m_count ; ++i)
    {
        if (m_modules_list[i].rack_idx >= BUS_COMM_START_IDX)
            continue;

        //copy module info to send buffer
        m_send.module_info = m_modules_list[i];

        GPIO::CS_modules_set();
        GPIO::RE_modules_unset();
        write(m_modules, &m_send, sizeof(BusMessage));

        m_recv.request = EBusRequest::UNKNOWN;
        m_recv.reply = EBusReply::UNKNOWN;

        GPIO::RE_modules_set();
        std::this_thread::sleep_for(std::chrono::milliseconds(BUS_WAIT_TIME_MS));
        read(m_modules, &m_recv, sizeof(BusMessage));

        if (m_recv.request == EBusRequest::UNKNOWN)
            break;

        switch (m_recv.reply)
        {
        case EBusReply::UNKNOWN:
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

bool PLCBus::search_comms()
{
    m_send.from = 0;
    m_send.to = (uint32_t)-1;
    m_send.request = EBusRequest::FIND_DEVICE;
    m_send.data_size = 0;

    GPIO::CS_comms_unset();

    for (uint32_t i=0 ; i<m_count ; ++i)
    {
        if (m_modules_list[i].rack_idx < BUS_COMM_START_IDX)
            continue;

        //copy module info to send buffer
        m_send.module_info = m_modules_list[i];

        GPIO::CS_comms_set();
        GPIO::RE_comms_unset();
        write(m_comms, &m_send, sizeof(BusMessage));

        m_recv.request = EBusRequest::UNKNOWN;
        m_recv.reply = EBusReply::UNKNOWN;

        GPIO::RE_comms_set();
        std::this_thread::sleep_for(std::chrono::milliseconds(BUS_WAIT_TIME_MS));
        read(m_comms, &m_recv, sizeof(BusMessage));

        if (m_recv.request == EBusRequest::UNKNOWN)
            break;

        switch (m_recv.reply)
        {
        case EBusReply::UNKNOWN:
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
