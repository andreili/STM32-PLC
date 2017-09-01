#ifndef PLC_MOD_H
#define PLC_MOD_H

#include <inttypes.h>
#include "plc_conf.h"

#define PLC_MOD_TYPE_CPU    (0x01 << 0x01)
#define PLC_MOD_TYPE_PN     (0x01 << 0x02)
#define PLC_MOD_TYPE_PB     (0x01 << 0x03)
#define PLC_MOD_TYPE_DI     (0x01 << 0x04)
#define PLC_MOD_TYPE_DO     (0x01 << 0x05)
#define PLC_MOD_TYPE_AI     (0x01 << 0x06)
#define PLC_MOD_TYPE_AO     (0x01 << 0x07)

#define PLC_MOD_ADDR_MAX_DISCRET    16
#define PLC_MOD_ADDR_MAX_ANALOG     8
#define PLC_MOD_ADDR_MAX_COM        200

#define PLC_MOD_TARGET_ALL          0xffff

#define PLC_MOD_MAX_MODULES         32

typedef struct
{
    uint32_t type;
    uint32_t hw_version;
    uint32_t fw_version;
    uint32_t inputs;
    uint32_t outputs;
} plc_mod_info_t;

class STM32_SPI;

enum class EModRequest : uint32_t
{
    NONE = 0,
    GET_INFO = 1,
};

typedef struct
{
    uint32_t    target_id;
    uint32_t    data_size;
    EModRequest request;
    union
    {
        uint8_t             discret[PLC_MOD_ADDR_MAX_DISCRET];
        uint16_t            analog[PLC_MOD_ADDR_MAX_ANALOG];
        uint8_t             com[PLC_MOD_ADDR_MAX_COM];
        plc_mod_info_t      info;
    };
} plc_mod_pkt_t;

class PLC_MOD
{
public:
    #ifdef PLC_CPU
    void init(STM32_SPI *spi, bool is_com);
    void find_modules();
    #else
    void init(STM32_SPI *spi);
    #endif
private:
    STM32_SPI       *m_spi;
    plc_mod_pkt_t   m_pkt_send;
    plc_mod_pkt_t   m_pkt_recv;

    #ifdef PLC_CPU
    bool            m_bus_com;
    uint32_t        m_mod_count;

    plc_mod_info_t  m_modules[PLC_MOD_MAX_MODULES];

    void CS_on();
    void CS_off();

    void print_module_info(plc_mod_info_t *mod);

    #else

    bool is_CSin_on();
    void CSout_on();
    void CSout_off();

    #endif
};

#ifdef PLC_CPU
extern PLC_MOD plc_mod_com;
extern PLC_MOD plc_mod_ext;
#endif

#endif // PLC_MOD_H
