#ifndef PLCC_H
#define PLCC_H

#include <string>
#include <deque>
#include "plc_data_types.h"
#include "stl_commands.h"

enum class EParseMainLocation
{
    NONE,
    DATA_BLOCK,
    ORGANIZATION_BLOCK,
    FUNCTION,
    FUNCTION_BLOCK,
};

enum class EParseSubLocation
{
    NONE,
    HEADER,
    // DB only
    STRUCTURE,
    DATA_INIT,
    // code blocks only
    VAR_INPUT,
    VAR_OUTPUT,
    VAR_INOUT,
    VAR_TEMP,
    VAR_STATIC,
    NETWORK,
    CALL_PARAMETERS,
};

enum class EParseHeaderType
{
    NONE,
    LABEL,
    TITLE,
    COMMENT,
};

typedef struct
{
    std::string     line_raw;
    std::string     label;
    std::string     title;
    std::string     comment;
    EParseHeaderType header_type;

    int             block_no;

    // DB
    EDataType       var_type;
    std::string     var_name;
    EBlockType      inst_block;
    int             inst_no;
    union
    {
        plc_data_t      var_value;
        plc_data_t      par_addr;
    };

    // network
    STL_CMD_t       *cmd;
    int             nw_idx;
    EParameterLocation  par_loc;

    // CALL parameters
    std::string     par_name;

    EParseMainLocation  loc;
    EParseSubLocation   loc_sub;
} stl_plaint_line_t;

enum class EParseResult
{
    OK = 0,
    PERROR = 1,
    SKIP = 2,
    NOT_APP = 3,
    BLOCK_END = 10,
};

class Stream;

class STLC
{
public:
    STLC();

    bool load_plain(std::string, bool iec);
    bool compile(std::string);
private:
    static STLC*                    m_instance;
    std::deque<stl_plaint_line_t>   m_plain;

    bool parse_stl_plain(Stream*);
    EParseMainLocation parse_stl_plain_location(std::string &line);
    EParseResult parse_stl_plain_DB(std::string &line, stl_plaint_line_t &plain);
    EParseResult parse_stl_plain_OB(std::string &line, stl_plaint_line_t &plain);

    EDataType parse_data_type(std::string str);
    EParseResult parse_value(std::string str, EDataType type, plc_data_t &value);
    stl_plaint_line_t* find_var_def(std::string name, int block_no, EParseMainLocation loc);

    std::string parse_title(std::string &str);
    EParseResult parse_stl_plain_block_header(std::string &line, stl_plaint_line_t &plain, const char* header, const char* block_abbr);
    EParseResult parse_stl_plain_var_struct(std::string &line, stl_plaint_line_t &plain, const char* end_label);
    EParseResult parse_stl_plain_DB_data_init(std::string &line, stl_plaint_line_t &plain);
    bool parse_stl_plain_DB_check_sub(std::string &line, stl_plaint_line_t &plain);
    bool parse_stl_plain_OB_check_sub(std::string &line, stl_plaint_line_t &plain);
    EParseResult parse_stl_plain_network(std::string &line, stl_plaint_line_t &plain, const char* end_block_label);
    EParseResult parse_stl_plain_call_parameter(std::string &line, stl_plaint_line_t &plain);
    EParseResult parse_stl_parameter_location(std::string str, stl_plaint_line_t &plain);
};

#endif // PLCC_H
