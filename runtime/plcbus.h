#ifndef PLCBUS_H
#define PLCBUS_H

#include "settings.h"

#define MODULE_TYPE_DI 0x00000001
#define MODULE_TYPE_DO 0x00000002
#define MODULE_TYPE_AI 0x00000004
#define MODULE_TYPE_AO 0x00000008
#define MODULE_TYPE_PN 0x00000010
#define MODULE_TYPE_PB 0x00000020
#define MODULE_TYPE_CPU 0x00000040
//#define MODULE_TYPE_ 0x00000000

struct ModuleInfo
{
    uint32_t    type;
    uint32_t    sub_type;

    uint32_t    rack;
    uint32_t    rack_idx;

    uint32_t    input_start;
    uint32_t    input_size;
    uint32_t    output_start;
    uint32_t    output_size;

    // only on PLC
    bool        finded;
};

enum class EBusRequest
{
    UNKNOWN         = 0x00,
    FIND_DEVICE     = 0x01,
    SET_RACK_IDX    = 0x02,
    READ_INPUTS     = 0x03,
    WRITE_OUTPUTS   = 0x04,
};

enum class EBusReply
{
    UNKNOWN,
    OK,
    FAIL,
};

struct BusMessage
{
    uint32_t    from;   // index on rack
    uint32_t    to;     // index on rack
    union
    {
        uint32_t    request_uint;
        EBusRequest request;
    };
    union
    {
        uint32_t    reply_uint;
        EBusReply   reply;
    };
    uint32_t    data_size;
    union
    {
        uint8_t     data[BUS_MSG_DATA_SIZE];
        ModuleInfo  module_info;
        uint32_t    rack_idx;
    };
};

class PLCBus
{
public:
    bool init(ModuleInfo *modules, uint32_t count);
    void copy_inputs();
    void copy_outputs();
    void bus_proc();
private:
    int         m_bus_dev;
    BusMessage  m_send;
    BusMessage  m_recv;
    uint8_t     m_PIP[IO_AREA_SIZE];
    uint8_t     m_POP[IO_AREA_SIZE];
    ModuleInfo  *m_modules_list;
    uint32_t    m_count;

    bool init_UART();
    bool search_modules();
};

#endif // PLCBUS_H
