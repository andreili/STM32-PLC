#include "plc_mod.h"
#ifdef PLATFORM_STM32
#include "my_func.h"
#include "stm32_spi.h"
#else
#include "plc_conf.h"
#include <cstring>
#endif
#include "plc_control.h"
#include "plc_platform.h"

PLC_MOD plc_mod_com;
PLC_MOD plc_mod_ext;

#define PLC_MOD_RESET_TIMEOUT 100
#define PLC_MOD_DATA_TIMEOUT 500
#define PLC_MOD_PKT_PAUSE 50

void PLC_MOD::init(bool is_com)
{
    m_bus_com = is_com;

    m_bus.init(m_bus_com);
}

void PLC_MOD::find_modules()
{
    m_mod_count = 0;


    memset((uint8_t*)&m_pkt_send, 0, sizeof(plc_mod_pkt_t));
    m_pkt_send.data_size = 0;
    m_pkt_send.request = EModRequest::GET_INFO;

    PLC_CONTROL::print_message("Get modules info (%s bus):\n", (m_bus_com ? "COM" : "EXT"));

    while (1)
    {
        // set target ID
        m_pkt_send.target_id = m_mod_count;
        m_pkt_recv.request = EModRequest::NONE;

        m_bus.unselect();
        PLATFORM_DELAY_MS(PLC_MOD_RESET_TIMEOUT);
        m_bus.select();
        m_bus.transmit((uint8_t*)&m_pkt_send, sizeof(plc_mod_pkt_t),
                       PLC_MOD_DATA_TIMEOUT);
        PLATFORM_DELAY_MS(PLC_MOD_PKT_PAUSE);
        if ((m_bus.transmit_recieve((uint8_t*)&m_pkt_send, (uint8_t*)&m_pkt_recv, sizeof(plc_mod_pkt_t),
                                    PLC_MOD_DATA_TIMEOUT) != PLC_RESULT_OK) ||
            (m_pkt_recv.request != EModRequest::GET_INFO))
            break;

        print_module_info(&m_pkt_recv.info);
        // fill module info
        memcpy((uint8_t*)&m_modules[m_mod_count], (uint8_t*)&m_pkt_recv.info,
               sizeof(plc_mod_info_t));

        ++m_mod_count;
    }
#ifdef PLATFORM_STM32
    PLC_CONTROL::print_message("\tFinded %U modules\n", m_mod_count);
#else
    PLC_CONTROL::print_message("\tFinded %i modules\n", m_mod_count);
#endif
}

void PLC_MOD::print_module_info(plc_mod_info_t *mod)
{

    PLC_CONTROL::print_message("\tModule: STM32-%08X-%02X:%02X-%02X:%02X\n", mod->type,
                               mod->hw_version, mod->hw_version,
                               mod->inputs, mod->outputs);
}
