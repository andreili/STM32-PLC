#ifndef PLC_FW_H
#define PLC_FW_H

#include <inttypes.h>
#include "plc_conf.h"
#include "stl_commands.h"
#include "plc_data_types.h"

class PLCFW
{
public:
    void init();
    bool load();
    
    void read_inputs();
    void execute_OB1();
    void write_outputs();
    
    int32_t set_current_db(int32_t db_no);
    int32_t get_db_offset(int32_t db_no);
    
    int32_t get_current_db_offset() { return (m_cur_db == -1) ? 0 : m_db[m_cur_db].offset; }
    program_db_descr_t* get_cur_db_info() { return (m_cur_db == -1) ? nullptr : &m_db[m_cur_db]; }
    
    void exec_block(int32_t block_idx, int local_db = -1);
    
    bool is_ok() { return m_fw_ok; }
private:
    uint8_t             *m_fw;
    uint32_t            m_fw_size;
    bool                m_fw_ok;
    
    program_header_t   *m_header;
    program_block_t    *m_blocks;
    program_network_t  *m_networks;
    STL_CMD_TYPE       *m_prog_data;
    program_db_descr_t *m_db;
    char                m_symbols;
    
    int32_t             m_cur_db;
};

extern PLCFW plc_fw;

#endif
