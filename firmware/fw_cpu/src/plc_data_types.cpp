#include "plc_data_types.h"
#include "plc_fw.h"
#include "plc_hw.h"

bool parameter_t::read_bool()
{
    if (type != EDataType::BOOL)
        return true;
    else
    {
        bool val = true;
        switch (location)
        {
        case EParameterLocation::INPUT:
            val = plc_hw.get_inputs_obj()->read_bit(data.BOOL.addr, data.BOOL.bit);
            break;
        case EParameterLocation::OUTPUT:
            val = plc_hw.get_outputs_obj()->read_bit(data.BOOL.addr, data.BOOL.bit);
            break;
        case EParameterLocation::MEMORY:
            val = plc_hw.get_mem_obj()->read_bit(data.BOOL.addr, data.BOOL.bit);
            break;
        case EParameterLocation::LOCAL:
            val = plc_hw.get_local_obj()->read_bit(data.BOOL.addr, data.BOOL.bit);
            break;
        case EParameterLocation::DB:
            {
                int32_t db_offset = plc_fw.get_db_offset(data.ADDR_DB.db_idx);
                return plc_hw.get_dbs_obj()->read_bit(db_offset + data.ADDR_DB.addr, data.ADDR_DB.bit);
            }
            break;
        case EParameterLocation::CONSTANT:
            val = data.BYTE;
            break;
        case EParameterLocation::STATUS_WORLD:
            val = plc_hw.get_status_word() & data.ADDR;
            break;
        default:
            break;
        }
        if (val)
            plc_hw.set_status_bits(PLC_STATUS_STA_MASK);
        else
            plc_hw.clear_status_bits(PLC_STATUS_STA_MASK);
        return val;
    }
}

void parameter_t::write_bool(bool val)
{
    if (type != EDataType::BOOL)
        return;
    else
    {
        if (val)
            plc_hw.set_status_bits(PLC_STATUS_STA_MASK);
        else
            plc_hw.clear_status_bits(PLC_STATUS_STA_MASK);

        if (!plc_hw.get_MCR())
            return;

        switch (location)
        {
        case EParameterLocation::INPUT:
            plc_hw.get_inputs_obj()->set_bit(data.BOOL.addr, data.BOOL.bit, val);
            break;
        case EParameterLocation::OUTPUT:
            plc_hw.get_outputs_obj()->set_bit(data.BOOL.addr, data.BOOL.bit, val);
            break;
        case EParameterLocation::MEMORY:
            plc_hw.get_mem_obj()->set_bit(data.BOOL.addr, data.BOOL.bit, val);
            break;
        case EParameterLocation::LOCAL:
            plc_hw.get_local_obj()->set_bit(data.BOOL.addr, data.BOOL.bit, val);
            break;
        case EParameterLocation::DB:
            {
                int32_t db_offset = plc_fw.get_db_offset(data.ADDR_DB.db_idx);
                plc_hw.get_dbs_obj()->set_bit(db_offset + data.ADDR_DB.addr, data.ADDR_DB.bit, val);
            }
            break;
        case EParameterLocation::CONSTANT:
            data.BYTE = val;
            break;
        default:
            break;
        }
    }
}

int8_t parameter_t::read_byte()
{
    if (get_size() != EDataSize::BYTE)
        return 0;
    else
    {
        int8_t val = 0;
        switch (location)
        {
        case EParameterLocation::INPUT:
            val = plc_hw.get_inputs_obj()->read_byte(data.addr);
            break;
        case EParameterLocation::OUTPUT:
            val = plc_hw.get_outputs_obj()->read_byte(data.addr);
            break;
        case EParameterLocation::MEMORY:
            val = plc_hw.get_mem_obj()->read_byte(data.addr);
            break;
        case EParameterLocation::LOCAL:
            val = plc_hw.get_local_obj()->read_byte(data.addr);
            break;
        case EParameterLocation::DB:
            {
                int32_t db_offset = plc_fw.get_db_offset(data.ADDR_DB.db_idx);
                return plc_hw.get_dbs_obj()->read_byte(db_offset + data.ADDR_DB.addr);
            }
            break;
        case EParameterLocation::CONSTANT:
            val = data.BYTE;
            break;
        default:
            break;
        }
        return val;
    }
}

void parameter_t::write_byte(int8_t val)
{
    if (get_size() != EDataSize::BYTE)
        return;
    else
    {
        if (!plc_hw.get_MCR())
            return;

        switch (location)
        {
        case EParameterLocation::INPUT:
            plc_hw.get_inputs_obj()->write_byte(data.addr, val);
            break;
        case EParameterLocation::OUTPUT:
            plc_hw.get_outputs_obj()->write_byte(data.addr, val);
            break;
        case EParameterLocation::MEMORY:
            plc_hw.get_mem_obj()->write_byte(data.addr, val);
            break;
        case EParameterLocation::LOCAL:
            plc_hw.get_local_obj()->write_byte(data.addr, val);
            break;
        case EParameterLocation::DB:
            {
                int32_t db_offset = plc_fw.get_db_offset(data.ADDR_DB.db_idx);
                plc_hw.get_dbs_obj()->write_byte(db_offset + data.ADDR_DB.addr, val);
            }
            break;
        case EParameterLocation::CONSTANT:
            data.BYTE = val;
            break;
        default:
            break;
        }
    }
}

int16_t parameter_t::read_word()
{
    if (get_size() != EDataSize::WORD)
        return 0;
    else
    {
        int16_t val = 0;
        switch (location)
        {
        case EParameterLocation::INPUT:
            val = plc_hw.get_inputs_obj()->read_word(data.addr);
            break;
        case EParameterLocation::OUTPUT:
            val = plc_hw.get_outputs_obj()->read_word(data.addr);
            break;
        case EParameterLocation::MEMORY:
            val = plc_hw.get_mem_obj()->read_word(data.addr);
            break;
        case EParameterLocation::LOCAL:
            val = plc_hw.get_local_obj()->read_word(data.addr);
            break;
        case EParameterLocation::DB:
            {
                int32_t db_offset = plc_fw.get_db_offset(data.ADDR_DB.db_idx);
                return plc_hw.get_dbs_obj()->read_word(db_offset + data.ADDR_DB.addr);
            }
            break;
        case EParameterLocation::CONSTANT:
            val = data.WORD;
            break;
        default:
            break;
        }
        return val;
    }
}

void parameter_t::write_word(int16_t val)
{
    if (get_size() != EDataSize::WORD)
        return;
    else
    {
        if (!plc_hw.get_MCR())
            return;

        switch (location)
        {
        case EParameterLocation::INPUT:
            plc_hw.get_inputs_obj()->write_word(data.addr, val);
            break;
        case EParameterLocation::OUTPUT:
            plc_hw.get_outputs_obj()->write_word(data.addr, val);
            break;
        case EParameterLocation::MEMORY:
            plc_hw.get_mem_obj()->write_word(data.addr, val);
            break;
        case EParameterLocation::LOCAL:
            plc_hw.get_local_obj()->write_word(data.addr, val);
            break;
        case EParameterLocation::DB:
            {
                int32_t db_offset = plc_fw.get_db_offset(data.ADDR_DB.db_idx);
                plc_hw.get_dbs_obj()->write_word(db_offset + data.ADDR_DB.addr, val);
            }
            break;
        case EParameterLocation::CONSTANT:
            data.WORD = val;
            break;
        default:
            break;
        }
    }
}

int32_t parameter_t::read_dword()
{
    if (get_size() != EDataSize::DWORD)
        return 0;
    else
    {
        int32_t val = 0;
        switch (location)
        {
        case EParameterLocation::INPUT:
            val = plc_hw.get_inputs_obj()->read_dword(data.addr);
            break;
        case EParameterLocation::OUTPUT:
            val = plc_hw.get_outputs_obj()->read_dword(data.addr);
            break;
        case EParameterLocation::MEMORY:
            val = plc_hw.get_mem_obj()->read_dword(data.addr);
            break;
        case EParameterLocation::LOCAL:
            val = plc_hw.get_local_obj()->read_dword(data.addr);
            break;
        case EParameterLocation::DB:
            {
                int32_t db_offset = plc_fw.get_db_offset(data.ADDR_DB.db_idx);
                return plc_hw.get_dbs_obj()->read_dword(db_offset + data.ADDR_DB.addr);
            }
            break;
        case EParameterLocation::CONSTANT:
            val = data.DWORD;
            break;
        default:
            break;
        }
        return val;
    }
}

void parameter_t::write_dword(int32_t val)
{
    if (get_size() != EDataSize::DWORD)
        return;
    else
    {
        if (!plc_hw.get_MCR())
            return;

        switch (location)
        {
        case EParameterLocation::INPUT:
            plc_hw.get_inputs_obj()->write_dword(data.addr, val);
            break;
        case EParameterLocation::OUTPUT:
            plc_hw.get_outputs_obj()->write_dword(data.addr, val);
            break;
        case EParameterLocation::MEMORY:
            plc_hw.get_mem_obj()->write_dword(data.addr, val);
            break;
        case EParameterLocation::LOCAL:
            plc_hw.get_local_obj()->write_dword(data.addr, val);
            break;
        case EParameterLocation::DB:
            {
                int32_t db_offset = plc_fw.get_db_offset(data.ADDR_DB.db_idx);
                plc_hw.get_dbs_obj()->write_dword(db_offset + data.ADDR_DB.addr, val);
            }
            break;
        case EParameterLocation::CONSTANT:
            data.DWORD = val;
            break;
        default:
            break;
        }
    }
}

EDataSize parameter_t::get_size()
{
    switch (type)
    {
    case EDataType::BOOL:
        return EDataSize::BIT;
    case EDataType::BYTE:
    case EDataType::CHAR:
        return EDataSize::BYTE;
    case EDataType::WORD:
    case EDataType::INT:
    case EDataType::S5TIME:
    case EDataType::DATE:
        return EDataSize::WORD;
    case EDataType::DWORD:
    case EDataType::DINT:
    case EDataType::REAL:
    case EDataType::TIME:
    case EDataType::TIME_OF_DAY:
    case EDataType::DATE_AND_TIME:
    case EDataType::ADDR:
        return EDataSize::DWORD;
    case EDataType::STRING:
    case EDataType::ARRAY:
        return EDataSize::BYTE; //TODO
    default:
        return EDataSize::BIT;
    }
}

int8_t PLCRegister::get_lolo()
{
    return m_reg.b1;
}

void PLCRegister::set_lolo(int8_t val)
{
    m_reg.b1 = val;
}

int16_t PLCRegister::get_lo()
{
    return __builtin_bswap16(m_reg.s1);
}

void PLCRegister::set_lo(int16_t val)
{
    m_reg.s1 = __builtin_bswap16(val);
}

int16_t PLCRegister::get_hi()
{
    return __builtin_bswap16(m_reg.s2);
}

void PLCRegister::set_hi(int16_t val)
{
    m_reg.s2 = __builtin_bswap16(val);
}

int32_t PLCRegister::get()
{
    return __builtin_bswap32(m_reg.u32);
}

void PLCRegister::set(int32_t val)
{
    m_reg.u32 = __builtin_bswap32(val);
}

float PLCRegister::get_real()
{
    reg_union_t d;
    d.u32 = __builtin_bswap32(m_reg.u32);
    return d.fl;
}

void PLCRegister::set_real(float val)
{
    reg_union_t d;
    d.fl = val;
    m_reg.u32 = __builtin_bswap32(d.u32);
}

void PLCRegister::load_from_param(parameter_t* from)
{
    switch (from->get_size())
    {
    case EDataSize::BIT:
        // unsupported
        break;
    case EDataSize::BYTE:
        set_lolo(from->read_byte());
        break;
    case EDataSize::WORD:
        set_lo(from->read_word());
        break;
    case EDataSize::DWORD:
        set(from->read_dword());
        break;
    }
}

void PLCRegister::load_to_param(parameter_t* to)
{
    switch (to->get_size())
    {
    case EDataSize::BIT:
        // unsupported
        break;
    case EDataSize::BYTE:
        to->write_byte(get_lolo());
        break;
    case EDataSize::WORD:
        to->write_word(get_lo());
        break;
    case EDataSize::DWORD:
        to->write_dword(get());
        break;
    }
}
