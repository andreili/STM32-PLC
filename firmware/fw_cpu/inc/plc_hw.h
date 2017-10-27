#ifndef PLC_HW_H
#define PLC_HW_H

#include "plc_data_types.h"
#include "plc_conf.h"
#include "plc_data_interface.h"
#include "stl_commands.h"

#define PLC_STATUS_NOT_FC_BIT 0
#define PLC_STATUS_RLO_BIT 1
#define PLC_STATUS_STA_BIT 2
#define PLC_STATUS_OR_BIT 3
#define PLC_STATUS_OS_BIT 4
#define PLC_STATUS_OV_BIT 5
#define PLC_STATUS_CC0_BIT 6
#define PLC_STATUS_CC1_BIT 7
#define PLC_STATUS_BR_BIT 8

#define PLC_STATUS_NOT_FC_MASK (1 << PLC_STATUS_NOT_FC_BIT)
#define PLC_STATUS_RLO_MASK (1 << PLC_STATUS_RLO_BIT)
#define PLC_STATUS_STA_MASK (1 << PLC_STATUS_STA_BIT)
#define PLC_STATUS_OR_MASK (1 << PLC_STATUS_OR_BIT)
#define PLC_STATUS_OS_MASK (1 << PLC_STATUS_OS_BIT)
#define PLC_STATUS_OV_MASK (1 << PLC_STATUS_OV_BIT)
#define PLC_STATUS_CC0_MASK (1 << PLC_STATUS_CC0_BIT)
#define PLC_STATUS_CC1_MASK (1 << PLC_STATUS_CC1_BIT)
#define PLC_STATUS_BR_MASK (1 << PLC_STATUS_BR_BIT)

typedef struct
{
    ESTLCommand cmd;
    uint32_t    status;
} nested_t;

class PLCHW
{
public:
    PLCHW();

    PLCDataInterface* get_mem_obj() { return m_memory; }
    PLCDataInterface* get_local_obj() { return m_local; }
    PLCDataInterface* get_dbs_obj() { return m_dbs; }
    PLCDataInterface* get_inputs_obj() { return m_inputs; }
    PLCDataInterface* get_outputs_obj() { return m_outputs; }

    void push_ACCU(int32_t val);
    void push_ACCU(parameter_t* param);
    int32_t pop_ACCU();
    void pop_ACCU(parameter_t* param);

    PLCRegister* get_ACCU1() { return &m_ACCU1; }
    PLCRegister* get_ACCU2() { return &m_ACCU2; }

    PLCRegister* get_AR1() { return &m_AR1; }
    PLCRegister* get_AR2() { return &m_AR2; }

    int32_t get_status_bit(uint32_t bit_mask);
    void set_status_bits(uint32_t bit_mask);
    void set_reset_status_bits(uint32_t bit_mask, uint32_t values);
    void clear_status_bits(uint32_t bit_mask);

    uint32_t get_status_word() { return m_status_word; }
    void reset_status_word();

    uint32_t get_local_top() { return m_local_top; }
    void set_local_top(uint32_t val) { m_local_top = val; }

    void nested_push(ESTLCommand cmd);
    nested_t nested_pop();

    bool get_MCR() { return m_MCR; }
private:
    PLCDataInterface *m_memory;
    PLCDataInterface *m_local;
    PLCDataInterface *m_dbs;
    PLCDataInterface *m_inputs;
    PLCDataInterface *m_outputs;
    uint8_t*         m_memory_area;
    uint8_t*         m_local_area;
    uint8_t*         m_dbs_area;
    uint8_t*         m_inputs_area;
    uint8_t*         m_outputs_area;
    uint32_t         m_status_word;
    uint32_t         m_local_top;
    bool             m_MCR;
    PLCRegister      m_ACCU1;
    PLCRegister      m_ACCU2;
    PLCRegister      m_ACCU3;
    PLCRegister      m_ACCU4;
    PLCRegister      m_AR1;
    PLCRegister      m_AR2;

    uint32_t         m_nested_size;
    nested_t         m_nested_buf[PLC_NESTED_DEPTH];
};

extern PLCHW plc_hw;

#endif // PLC_HW_H
