#include "stlc.h"
#include "Stream.h"
#include <regex>

#define STL_WHITESPACES " \t\n\r;"
#define STL_DB_HEADER "DATA_BLOCK"

STLC* STLC::m_instance;

STLC::STLC()
{
    m_instance = this;
}

bool STLC::load_plain(std::string file_name, bool iec)
{
    printf("Loading sources from '%s'.\n", file_name.c_str());
    Stream stream(file_name, FILE_OPEN_READ_ST);
    if (!stream.opened())
        return false;
    if (iec)
    {
        printf("Unsupported mode - IEC ST!\n");
        return false;
    }
    else
        parse_stl_plain(&stream);
    printf("Loaded %i lines.\n", m_plain.size());
    return true;
}

bool STLC::compile(std::string file_name)
{
    (void)(file_name);
    return true;
}

bool STLC::parse_stl_plain(Stream* stream)
{
    m_plain.clear();
    EParseMainLocation location = EParseMainLocation::NONE;

    // read all texts from source file
    std::string in_buf;
    in_buf.resize(stream->getSize());
    stream->read(&in_buf[0], stream->getSize());

    std::regex reg_comm("(\\/\\/.*)", std::regex_constants::icase);
    std::regex reg_sp("([[:blank:]]{2,})", std::regex_constants::icase);

    size_t start_pos = 0;
    size_t end_pos = in_buf.size();
    int line_idx = 1;
    stl_plaint_line_t plain;
    while (start_pos < end_pos)
    {
        size_t sub_end = in_buf.find_first_of("\n\r", start_pos);
        std::string line_raw, line;
        line_raw = line = in_buf.substr(start_pos, sub_end - start_pos);

        line = std::regex_replace(line, reg_comm, "");
        line = std::regex_replace(line, reg_sp, " ");
        const auto strBegin = line.find_first_not_of(STL_WHITESPACES);
        // skip whitespaced lines
        if ((line.length() == 0) || (strBegin == std::string::npos))
        {
            start_pos = sub_end + 1;
            continue;
        }

        const auto strEnd = line.find_last_not_of(STL_WHITESPACES);
        const auto strRange = strEnd - strBegin + 1;
        line = line.substr(strBegin, strRange);

        line = std::regex_replace(line, std::regex("([[:blank:]]*:=[[:blank:]]*)", std::regex_constants::icase), ":=");
        line = std::regex_replace(line, std::regex("([[:graph:]]+)[[:blank:]]*=[[:blank:]]*", std::regex_constants::icase), "$1=");
        line = std::regex_replace(line, std::regex("([[:blank:]]*:[[:blank:]]*)", std::regex_constants::icase), ":");
        std::transform(line.begin(), line.end(), line.begin(), ::toupper);
        if (line.length() == 0)
        {
            start_pos = sub_end + 1;
            continue;
        }

        plain.line_raw = line;

        if (location == EParseMainLocation::NONE)
        {
            location = parse_stl_plain_location(line);
            if (location == EParseMainLocation::NONE)
            {
                printf("Unknown loation at line %i:\n%s\n", line_idx, line_raw.c_str());
                return false;
            }
            printf("Parse %s\n", line.c_str());
            // set location for a plain line
            plain.loc = location;
            plain.loc_sub = EParseSubLocation::HEADER;
        }

        EParseResult res = EParseResult::OK;
        switch (location)
        {
        case EParseMainLocation::NONE:
            // nothing
            break;
        case EParseMainLocation::DATA_BLOCK:
            res = parse_stl_plain_DB(line, plain);
            break;
        case EParseMainLocation::ORGANIZATION_BLOCK:
            //TODO
            break;
        case EParseMainLocation::FUNCTION:
            //TODO
            break;
        case EParseMainLocation::FUNCTION_BLOCK:
            //TODO
            break;
        }

        switch (res)
        {
        case EParseResult::OK:
            // nothing
            break;
        case EParseResult::PERROR:
            printf("Error at line %i:\n%s\n", line_idx, line_raw.c_str());
            return false;
        case EParseResult::SKIP:
        case EParseResult::NOT_APP:
            start_pos = sub_end + 1;
            ++line_idx;
            continue;
        case EParseResult::BLOCK_END:
            start_pos = sub_end + 1;
            ++line_idx;
            location = EParseMainLocation::NONE;
            plain.loc_sub = EParseSubLocation::NONE;
            continue;
        }

        m_plain.push_back(plain);

        start_pos = sub_end + 1;
        ++line_idx;
    }

    return true;
}

EParseMainLocation STLC::parse_stl_plain_location(std::string &line)
{
    size_t ws_pos = line.find_first_of(STL_WHITESPACES);
    if (ws_pos != std::string::npos)
    {
        std::string loc_str = line.substr(0, ws_pos);
        if (loc_str.compare(STL_DB_HEADER) == 0)
            return EParseMainLocation::DATA_BLOCK;
        else if (loc_str.compare("ORGANIZATION_BLOCK") == 0)
            return EParseMainLocation::ORGANIZATION_BLOCK;
        else if (loc_str.compare("FUNCTION") == 0)
            return EParseMainLocation::FUNCTION;
        else if (loc_str.compare("FUNCTION_BLOCK") == 0)
            return EParseMainLocation::FUNCTION_BLOCK;
    }
    return EParseMainLocation::NONE;
}

/*
 * Typical Data Block:
 *  DATA_BLOCK DB 1
 *  TITLE = Example block title
 *  VERSION : 0.1 // optional
 *
 *  STRUCT
 *   var1 : INT;
 *   var1 : BOOL;
 *   var3 : REAL;
 *  END_STRUCT
 *  BEGIN
 *   var1 := 1024;
 *   var2 := true;
 *   var3 := 0.01;
 *  END_DATA_BLOCK
 * */
EParseResult parse_stl_plain_DB_header(std::string &line, stl_plaint_line_t &plain)
{
    plain.header_type = EParseHeaderType::NONE;
    if (line.compare(0, 10, STL_DB_HEADER) == 0)
    {
        std::string db_str = line.substr(strlen(STL_DB_HEADER) + 1, 2);
        if (db_str.compare("DB"))
        {
            printf("Error: Invalid Data Block header!\n");
            return EParseResult::PERROR;
        }
        db_str = line.substr(strlen(STL_DB_HEADER) + 4);
        plain.block_no = std::stoi(db_str);
        return EParseResult::NOT_APP;
    }
    else if (line.compare(0, 5, "TITLE") == 0)
    {
        plain.header_type = EParseHeaderType::TITLE;
        size_t pos = line.find_first_not_of(5, '=');
        pos = line.find_first_not_of(STL_WHITESPACES, pos);
        if (pos != std::string::npos)
            plain.title = line.substr(pos);
        else
            plain.title = "";
        return EParseResult::OK;
    }
    else if (line.compare(0, 7, "VERSION") == 0)
    {
        // not applicable
        return EParseResult::NOT_APP;
    }
    else
        return EParseResult::SKIP;
}

EParseResult parse_stl_plain_DB_structure(std::string &line, stl_plaint_line_t &plain)
{
    if (line.compare("END_STRUCT") == 0)
    {
        plain.loc_sub = EParseSubLocation::NONE;
        return EParseResult::NOT_APP;
    }

    plain.var_name = line.substr(0, line.find_first_of(':'));
    plain.var_type = STLC::parse_data_type(line.substr(plain.var_name.length() + 1));
    if (plain.var_type == EDataType::NONE)
        return EParseResult::PERROR;
    else
        return EParseResult::OK;
}

EParseResult parse_stl_plain_DB_data_init(std::string &line, stl_plaint_line_t &plain)
{
    if (line.compare("END_DATA_BLOCK") == 0)
        return EParseResult::BLOCK_END;

    plain.var_name = line.substr(0, line.find_first_of(':'));
    stl_plaint_line_t *var_def = STLC::find_var_def(plain.var_name, plain.block_no, plain.loc);
    if (var_def == nullptr)
    {
        printf("Error: Unknown variable name!\n");
        return EParseResult::PERROR;
    }
    if (STLC::parse_value(line.substr(plain.var_name.length() + 2), var_def->var_type,
                          var_def->var_value) != EParseResult::OK)
        return EParseResult::PERROR;
    return EParseResult::NOT_APP;
}

bool parse_stl_plain_DB_check_sub(std::string &line, stl_plaint_line_t &plain)
{
    if (line.compare("STRUCT") == 0)
    {
        plain.loc_sub = EParseSubLocation::STRUCTURE;
        return true;
    }
    else if (line.compare("BEGIN") == 0)
    {
        plain.loc_sub = EParseSubLocation::DATA_INIT;
        return true;
    }
    else
    {
        printf("Error: Unknown Dat Block substitute!\n");
        return false;
    }
}

EParseResult STLC::parse_stl_plain_DB(std::string &line, stl_plaint_line_t &plain)
{
    while (1)
    {
        EParseResult res = EParseResult::SKIP;
        switch (plain.loc_sub)
        {
        case EParseSubLocation::HEADER:
            res = parse_stl_plain_DB_header(line, plain);
            break;
        case EParseSubLocation::STRUCTURE:
            res = parse_stl_plain_DB_structure(line, plain);
            break;
        case EParseSubLocation::DATA_INIT:
            res = parse_stl_plain_DB_data_init(line, plain);
            break;
        case EParseSubLocation::NONE:
            break;
        case EParseSubLocation::VAR_INPUT:
        case EParseSubLocation::VAR_OUTPUT:
        case EParseSubLocation::VAR_INOUT:
        case EParseSubLocation::VAR_TEMP:
        case EParseSubLocation::VAR_STATIC:
        case EParseSubLocation::NETWORK:
            printf("Error: Invalid location on Data Block!\n");
            return EParseResult::PERROR;
        }

        if (res != EParseResult::SKIP)
            return res;

        if (parse_stl_plain_DB_check_sub(line, plain))
            break;
    }
    return EParseResult::NOT_APP;
}

EDataType STLC::parse_data_type(std::string str)
{
    if (str.compare("BOOL") == 0)
        return EDataType::BOOL;
    else if (str.compare("BYTE") == 0)
        return EDataType::BYTE;
    else if (str.compare("CHAR") == 0)
        return EDataType::CHAR;
    else if (str.compare("WORD") == 0)
        return EDataType::WORD;
    else if (str.compare("DWORD") == 0)
        return EDataType::DWORD;
    else if (str.compare("INT") == 0)
        return EDataType::INT;
    else if (str.compare("DINT") == 0)
        return EDataType::DINT;
    else if (str.compare("REAL") == 0)
        return EDataType::REAL;
    else if (str.compare("S5TIME") == 0)
        return EDataType::S5TIME;
    else if (str.compare("TIME") == 0)
        return EDataType::TIME;
    else if (str.compare("DATE") == 0)
        return EDataType::DATE;
    else if (str.compare("TIME_OF_DAY") == 0)
        return EDataType::TIME_OF_DAY;
    else if (str.compare("DATE_AND_TIME") == 0)
        return EDataType::DATE_AND_TIME;
    else if (str.compare("STRING") == 0)
        return EDataType::STRING;
    else if (str.compare("ARRAY") == 0)
        return EDataType::ARRAY;
    else if (str.compare("ADDR") == 0)
        return EDataType::ADDR;
    else
    {
        printf("Error: Unknown data type:\n%s\n", str.c_str());
        return EDataType::NONE;
    }
}

EParseResult STLC::parse_value(std::string str, EDataType type, plc_data_t &value)
{
    switch (type)
    {
    case EDataType::NONE:
        break;
    case EDataType::BOOL:
        if (str.compare("TRUE") == 0)
            value.bool_val = true;
        else if (str.compare("FALSE") == 0)
            value.bool_val = false;
        break;
    case EDataType::BYTE:
        value.BYTE = std::stoi(str);
        break;
    case EDataType::CHAR:
        value.CHAR = str[0];    //TODO
        break;
    case EDataType::WORD:
        value.WORD = std::stoi(str);
        break;
    case EDataType::DWORD:
        value.DWORD = std::stoi(str);
        break;
    case EDataType::INT:
        value.INT = std::stoi(str);
        break;
    case EDataType::DINT:
        value.DINT = std::stoi(str);
        break;
    case EDataType::REAL:
        value.REAL = std::stof(str);
        break;
    case EDataType::S5TIME:
    case EDataType::TIME:
    case EDataType::DATE:
    case EDataType::TIME_OF_DAY:
    case EDataType::DATE_AND_TIME:
    case EDataType::STRING:
    case EDataType::ARRAY:
    case EDataType::ADDR:
        printf("Error: Unsupported value type!\n");
        return EParseResult::PERROR;
    }
    return EParseResult::OK;
}

stl_plaint_line_t* STLC::find_var_def(std::string name, int block_no, EParseMainLocation loc)
{
    for (stl_plaint_line_t &pl : m_instance->m_plain)
    {
        if (pl.loc != loc)
            continue;
        if (pl.block_no != block_no)
            continue;
        if (pl.loc_sub != EParseSubLocation::STRUCTURE)
            continue;
        if (pl.var_name.compare(name) == 0)
            return &pl;
    }
    return nullptr;
}
