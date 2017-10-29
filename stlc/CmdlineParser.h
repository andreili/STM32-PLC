#ifndef CMDLINEPARSER_H
#define CMDLINEPARSER_H

#include <vector>
#include <string>

/**
 * @brief Перечисление типов параметров командной строки
 */
enum class EParamType
{
    STRING,
    BOOLEAN,
    INT,
};

/** @brief Структура для хранения сведений о параметре коммандной строки */
typedef struct
{
    /** @brief Тип параметра */
    EParamType      type;
    /** @brief Имя параметра */
    std::string    name;
    /** @brief Описание параметра */
    std::string    desc;
    /** @brief Ссылка на строковое значение параметра */
    std::string*    val_str;
    /** @brief Ссылка на булевое значение параметра */
    bool*           val_bool;
    /** @brief Ссылка на численное значение параметра */
    int*            val_int;
} param_rec_t;

/**
 * @brief Класс для работы с параметрами командной строки
 */
class CmdlineParser
{
public:
    CmdlineParser(int argc, char** argv);
    virtual ~CmdlineParser();
    /** @brief Получение текущего экземпляра объекта */
    static CmdlineParser* get_instance() { return m_instance; }

    static void add_string_param(const std::string &name, std::string* val, const std::string &val_def, const std::string &desc);
    static void add_bool_param(const std::string &name, bool* val, const bool &val_def, const std::string &desc);
    static void add_int_param(const std::string &name, int* val, const int &val_def, const std::string &desc);

    /** @brief Установка имени приложения (для вывода в справке) */
    static void set_name(std::string name) { m_instance->m_name = name; }
    static void show_desc();
    static void parse();
protected:
private:
    static CmdlineParser*       m_instance;
    int                         m_argc;
    std::string                 m_name;
    std::vector<std::string>    m_argv;
    std::vector<param_rec_t>    m_params;
};

#endif // CMDLINEPARSER_H
