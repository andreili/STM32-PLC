#include "plc_hw.h"

PLCHW plc_hw;

PLCHW::PLCHW()
	: m_status_word (0)
	, m_local_top (0)
	, m_MCR (true)
	, m_nested_size (0)
{
    m_MCR = true;
    m_local_top = 0;
    reset_status_word();

    m_memory_area = new uint8_t[PLC_MEM_SIZE];
    m_local_area = new uint8_t[PLC_LOCAL_SIZE];
    m_dbs_area = new uint8_t[PLC_DB_MEM_SIZE];
    m_inputs_area = new uint8_t[PLC_IO_SIZE];
    m_outputs_area = new uint8_t[PLC_IO_SIZE];

    m_memory = new PLCDataInterface(m_memory_area);
    m_local = new PLCDataInterface(m_local_area);
    m_dbs = new PLCDataInterface(m_dbs_area);
    m_inputs = new PLCDataInterface(m_inputs_area);
    m_outputs = new PLCDataInterface(m_outputs_area);;
}

void PLCHW::push_ACCU(int32_t val)
{
    m_ACCU4.set(m_ACCU3.get());
    m_ACCU3.set(m_ACCU2.get());
    m_ACCU2.set(m_ACCU1.get());
    m_ACCU1.set(val);
}

void PLCHW::push_ACCU(parameter_t* param)
{
    m_ACCU4.set(m_ACCU3.get());
    m_ACCU3.set(m_ACCU2.get());
    m_ACCU2.set(m_ACCU1.get());
    m_ACCU1.set(0);
    switch (param->get_size())
    {
    case EDataSize::BIT:
        // unsupported
        break;
    case EDataSize::BYTE:
        m_ACCU1.set_lolo(param->read_byte());
        break;
    case EDataSize::WORD:
        m_ACCU1.set_lo(param->read_word());
        break;
    case EDataSize::DWORD:
        m_ACCU1.set(param->read_dword());
        break;
    }
}

int32_t PLCHW::pop_ACCU()
{
    int32_t val = m_ACCU1.get();
    m_ACCU1.set(m_ACCU2.get());
    m_ACCU2.set(m_ACCU3.get());
    m_ACCU3.set(m_ACCU4.get());
    return val;
}

void PLCHW::pop_ACCU(parameter_t* param)
{
    switch (param->get_size())
    {
    case EDataSize::BIT:
        // unsupported
        break;
    case EDataSize::BYTE:
        param->write_byte(m_ACCU1.get_lolo());
        break;
    case EDataSize::WORD:
        param->write_word(m_ACCU1.get_lo());
        break;
    case EDataSize::DWORD:
        param->write_dword(m_ACCU1.get());
        break;
    }
    m_ACCU1.set(m_ACCU2.get());
    m_ACCU2.set(m_ACCU3.get());
    m_ACCU3.set(m_ACCU4.get());
}

int32_t PLCHW::get_status_bit(uint32_t bit_mask)
{
    return (m_status_word & bit_mask);
}

void PLCHW::set_status_bits(uint32_t bit_mask)
{
    m_status_word |= bit_mask;
}

void PLCHW::set_reset_status_bits(uint32_t bit_mask, uint32_t values)
{
    m_status_word = (m_status_word & (~(bit_mask & (~values)))) | (bit_mask & values);
}

void PLCHW::clear_status_bits(uint32_t bit_mask)
{
    m_status_word &= ~bit_mask;
}

void PLCHW::reset_status_word()
{
    m_status_word = 0;
}

void PLCHW::nested_push(ESTLCommand cmd)
{
    if ((m_nested_size + 1) >= PLC_NESTED_DEPTH)
    {
        //TODO: exception
        return;
    }
    if (!PLCHW::get_status_bit(PLC_STATUS_NOT_FC_MASK))
        m_nested_buf[m_nested_size++] = {cmd,
                                         PLC_STATUS_RLO_MASK | (m_status_word & PLC_STATUS_OR_MASK)};
    else
        m_nested_buf[m_nested_size++] = {cmd,
                                         m_status_word & (PLC_STATUS_RLO_MASK | PLC_STATUS_OR_MASK)};
}

nested_t PLCHW::nested_pop()
{
    if (m_nested_size == 0)
    {
        //TODO: exception
			return {ESTLCommand::NOP_0, 0};
    }
    return m_nested_buf[--m_nested_size];
}
