#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#ifndef STLC
#include "plc_conf.h"
#endif
#include <inttypes.h>
#include <time.h>

enum class EBlockType
{
    NONE,
    OB,
    FB,
    FC,
    SFB,
    SFC,
};

typedef struct
{
    int32_t     idx;
    int32_t     no;
    char 				title[PLC_COMMENT_SIZE];
} OB_descr_t;

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
    NONE,
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

#define S5T_TB_10_MS 0x00
#define S5T_TB_100_MS 0x01
#define S5T_TB_1_S 0x10
#define S5T_TB_10_S 0x11

typedef struct
{
    uint8_t     BCD_0: 4;
    uint8_t     BCD_1: 4;
    uint8_t     BCD_2: 4;
    uint8_t     time_base: 2;
} S5TIME_t;

union plc_data_t
{
    uint16_t    counter_idx;
    uint16_t    timer_idx;

    struct
    {
        uint8_t     bit;
        uint16_t    addr;
    } BOOL;
    bool       bool_val;
    struct
    {
        uint8_t     bit;
        uint16_t    addr;
        uint16_t    db_idx;
    } ADDR_DB;
    uint16_t    addr;
    uint8_t     BYTE;
    char        CHAR;
    int16_t     WORD;
    int32_t     DWORD;
    int16_t     INT;
    int32_t     DINT;
    float       REAL;
    S5TIME_t    S5TIME;
    uint32_t    TIME;   //TODO
    uint16_t    DATE;   //TODO
    uint32_t    TIME_OF_DAY;

    uint32_t    ADDR;   // for jump instructions
};

#ifndef STLC
typedef struct parameter_t
{
    EParameterLocation  location;
    EDataType           type;
    plc_data_t          data;

    bool read_bool();
    void write_bool(bool val);

    int8_t read_byte();
    void write_byte(int8_t val);

    int16_t read_word();
    void write_word(int16_t val);

    int32_t read_dword();
    void write_dword(int32_t val);

    EDataSize get_size();
} parameter_t;
#endif

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

union reg_union_t
{
    struct
    {
        uint8_t b1;
        uint8_t b2;
        uint8_t b3;
        uint8_t b4;
    };
    struct
    {
        uint16_t s1;
        uint16_t s2;
    };
    uint32_t u32;
    float fl;
};

#ifndef STLC
typedef struct
{
    int8_t get_lolo();
    void set_lolo(int8_t val);
    int16_t get_lo();
    void set_lo(int16_t val);
    int16_t get_hi();
    void set_hi(int16_t val);

    int32_t get();
    void set(int32_t val);

    float get_real();
    void set_real(float val);

    void load_from_param(parameter_t* from);
    void load_to_param(parameter_t* to);
private:
    reg_union_t m_reg;
    //uint32_t    data;
} PLCRegister;
#endif

#endif // DATA_TYPES_H
