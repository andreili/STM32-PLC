#ifndef PLC_FW_H
#define PLC_FW_H

#include <inttypes.h>
#include "plc_conf.h"
#include "stl_commands.h"

enum class EBlockType
{
    NONE,
    OB,
    FB,
    FC,
    SFB,
    SFC,
};

enum class EParameterLocation : uint8_t
{
    NONE,
    LINK,   //link to parameters area (if size > 2)
    INPUT,
    OUTPUT,
    MEMORY,
    LOCAL,
    IDX,
    DB,
    TIMER,
    COUNTER,
    CONSTANT,
    BLOCK_INPUT,
    BLOCK_OUTPUT,
    BLOCK_TEMP,
    FB,
    FC,
    SFB,
    SFC,
    STATUS_WORLD,
    AR1_OFFSET,
    AR2_OFFSET,
};

enum class EDataType : uint8_t
{
    BOOL,
    BYTE,
    CHAR,
    WORD,
    DWORD,
    INT,
    DINT,
    REAL,   //float
    S5TIME, //16bit, word
    TIME,   //32bit
    DATE,   //16bit
    TIME_OF_DAY,    //32 bit
    // complex types
    DATE_AND_TIME,
    STRING,
    ARRAY,
    ADDR,   // for jump instructions
};

enum class EDataSize
{
    BIT,
    BYTE,
    WORD,
    DWORD,
};

typedef struct
{
    int32_t     start;
    int32_t     length;
} symbol_t;

typedef struct
{
    EBlockType  type;
    int32_t     idx;
    symbol_t    name;
    symbol_t    title;
    symbol_t    comment;
    int32_t     nw_used;
    int32_t     network_first;
    //TODO: local variables
} program_block_t;

typedef struct
{
    symbol_t    title;
    symbol_t    comment;
    int32_t     start_addr;
    int32_t     network_next;
} program_network_t;

typedef struct
{
    int16_t     idx;
    int16_t     no;
    int16_t     size_bytes;
    int32_t     offset;
    symbol_t    title;
} program_db_descr_t;

typedef struct
{
    char        sign[4];
    int32_t     version;
    int32_t     block_count;
    int32_t     network_count;
    int32_t     program_size;
    int32_t     db_count;
    int32_t     symbols_size;
} program_header_t;

class PLCFW
{
public:
    void init();
    bool load();
    
    bool is_ok() { return m_fw_ok; }
private:
    uint8_t             m_fw[PLC_FW_MAX_SIZE];
    uint32_t            m_fw_size;
    bool                m_fw_ok;
    
    program_header_t    m_header;
    program_block_t*    m_blocks;
    program_network_t*  m_networks;
    STL_CMD_TYPE*       m_prog_data;
    program_db_descr_t* m_db;
    int32_t             m_pc;
    int32_t             m_cur_db;
};

extern PLCFW plc_fw;

#endif
