#include "plc_fw.h"
#include "plc_control.h"
#include "plc_platform.h"
#include "plc_hw.h"
#include <cstring>

#define PLC_PROG_SIGN "S3P"

PLCFW plc_fw;

#define OB1     0
#define OB10     1
#define OB11     2
#define OB12     3
#define OB13     4
#define OB14     5
#define OB15     6
#define OB16     7
#define OB17     8
#define OB20     9
#define OB21     10
#define OB22     11
#define OB23     12
#define OB30     13
#define OB31     14
#define OB32     15
#define OB33     16
#define OB34     17
#define OB35     17
#define OB36     18
#define OB37     19
#define OB38     20
#define OB40     21
#define OB41     22
#define OB42     23
#define OB43     24
#define OB44     25
#define OB45     26
#define OB46     27
#define OB47     28
#define OB48     29
#define OB60     30
#define OB80     31
#define OB81     32
#define OB82     33
#define OB83     34
#define OB84     35
#define OB85     36
#define OB86     37
#define OB87     38
#define OB90     39
#define OB100     40
#define OB101     41
#define OB102     42
#define OB121     43
#define OB122     44

OB_descr_t OB_descr[PLC_SYSTEM_OB_COUNT] = {
    {OB1, 1, "Cycle Execution"},
    {OB10, 10, "Time of Day Interrupt 0"},
    {OB11, 11, "Time of Day Interrupt 1"},
    {OB12, 12, "Time of Day Interrupt 2"},
    {OB13, 13, "Time of Day Interrupt 3"},
    {OB14, 14, "Time of Day Interrupt 4"},
    {OB15, 15, "Time of Day Interrupt 5"},
    {OB16, 16, "Time of Day Interrupt 6"},
    {OB17, 17, "Time of Day Interrupt 7"},
    {OB20, 20, "Time Delay Interrupt 0"},
    {OB21, 21, "Time Delay Interrupt 1"},
    {OB22, 22, "Time Delay Interrupt 2"},
    {OB23, 23, "Time Delay Interrupt 3"},
    {OB30, 30, "Cyclic Interrupt 0"},
    {OB31, 31, "Cyclic Interrupt 1"},
    {OB32, 32, "Cyclic Interrupt 2"},
    {OB33, 33, "Cyclic Interrupt 3"},
    {OB34, 34, "Cyclic Interrupt 4"},
    {OB35, 35, "Cyclic Interrupt 5"},
    {OB36, 36, "Cyclic Interrupt 6"},
    {OB37, 37, "Cyclic Interrupt 7"},
    {OB38, 38, "Cyclic Interrupt 8"},
    {OB40, 40, "Hardware Interrupt 0"},
    {OB41, 41, "Hardware Interrupt 1"},
    {OB42, 42, "Hardware Interrupt 2"},
    {OB43, 43, "Hardware Interrupt 3"},
    {OB44, 44, "Hardware Interrupt 4"},
    {OB45, 45, "Hardware Interrupt 5"},
    {OB46, 46, "Hardware Interrupt 6"},
    {OB47, 47, "Hardware Interrupt 7"},
    {OB60, 60, "Multicomputing Interrupt"},
    {OB80, 80, "Cycle Time Fault"},
    {OB81, 81, "Power Supply Fault"},
    {OB82, 82, "I/O Point Fault 1"},
    {OB83, 83, "I/O Point Fault 2"},
    {OB84, 84, "CPU Fault"},
    {OB85, 85, "OB Not Loaded Fault"},
    {OB86, 86, "Loss of Rack Fault"},
    {OB87, 87, "Communication Fault"},
    {OB90, 90, "Background Cycle"},
    {OB100, 100, "Complete Restart"},
    {OB101, 101, "Restart"},
    {OB102, 102, "Cold Restart"},
    {OB121, 121, "Programming Error"},
    {OB122, 122, "Module Access Error"},
};

void PLCFW::init()
{
    m_fw_ok = false;
}

bool PLCFW::load()
{
    m_blocks = nullptr;
    m_networks = nullptr;
    m_prog_data = nullptr;
    m_db = nullptr;
    m_fw_ok = false;
    
    PLC_CONTROL::print_message("+-----------------------------+\n"
                               "Firmware loading\n");
    
    m_fw_size = load_file("/usr/local/plc/fw.bin", m_fw);
    if (m_fw_size > 0)
    {
        m_header = (program_header_t*)&m_fw[0];
        int32_t offset = sizeof(program_header_t);
        
        if (strcmp(m_header->sign, PLC_PROG_SIGN))
            goto failed;
        
        m_blocks = (program_block_t*)&m_fw[offset];
        offset += sizeof(program_block_t) * m_header->block_count;
        
        m_networks = (program_network_t*)&m_fw[offset];
        offset += sizeof(program_network_t) * m_header->network_count;
        
        m_prog_data = (STL_CMD_TYPE*)&m_fw[offset];
        offset += sizeof(STL_CMD_TYPE) * m_header->program_size;
        
        m_db = (program_db_descr_t*)&m_fw[offset];
        offset += sizeof(program_db_descr_t) * m_header->db_count;
        
        m_fw_ok = true;
    }
    
    PLC_CONTROL::print_message("Statistics:\n\tBlocks: %i\n\tNetworks: %i\n\tCode size: %i\n\tDBs count: %i\n",
                               m_header->block_count, m_header->network_count, m_header->program_size,
                               m_header->db_count);
    PLC_CONTROL::print_message("+-----------------------------+\n");
    return true;
    
failed:
    PLC_CONTROL::print_message("!!!! FAILED !!!!\n");
    return false;
}

void PLCFW::read_inputs()
{
}

void PLCFW::execute_OB1()
{
    plc_hw.reset_status_word();
    exec_block(OB1);
}

void PLCFW::write_outputs()
{
}

int32_t PLCFW::set_current_db(int32_t db_no)
{
    if (db_no != -1)
        for (int32_t i=0 ; i<m_header->db_count ; ++i)
            if (m_db[i].no == db_no)
            {
                m_cur_db = i;
                return i;
            }
    m_cur_db = -1;
    return -1;
}

int32_t PLCFW::get_db_offset(int32_t db_no)
{
    if (db_no == 0)
        db_no = m_cur_db;
    if (db_no != -1)
        for (int32_t i=0 ; i<m_header->db_count ; ++i)
            if (m_db[i].no == db_no)
            {
                return m_db[i].offset;
                return i;
            }
    return 0;
}

void PLCFW::exec_block(int32_t block_idx, int local_db)
{
    program_block_t *block = &m_blocks[block_idx];

    if (block_idx < PLC_FB_OFFSET)
        local_db = 0;
    else
        local_db = set_current_db(local_db);

    int32_t nw_idx = block->network_first;
    while (nw_idx != PLC_NETWORK_END)
    {
        program_network_t *network = &m_networks[nw_idx];
        nw_idx = network->network_next;

        int32_t pc = network->start_addr;
        plc_hw.clear_status_bits(PLC_STATUS_NOT_FC_MASK);
        plc_hw.clear_status_bits(PLC_STATUS_RLO_MASK);
        while (m_prog_data[pc] != (STL_CMD_TYPE)ESTLCommand::NETWORK_END)
        {
            m_cur_db = local_db;

            int sz = STLCommands::exec_command(&m_prog_data[pc]);
            #ifdef PLC_DEBUG
            printf(" SW: %s\n", PLCUtils::byte_to_binary(plc_hw.get_status_word(), 8));
            #endif

            if (sz > 0)
                pc += sz;
            else if (sz == 0)
                // end of network
                break;
            else if (sz == -1)
                // end of block execution
                return;
            else
                pc = (-1) * sz;
        }
        //printf("\n\n");
    }

    plc_hw.set_reset_status_bits(PLC_STATUS_OS_MASK, 0);
}
