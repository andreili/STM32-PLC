#ifndef _STREAM_H_
#define _STREAM_H_

#include <string>
#include <inttypes.h>
#include <vector>
#ifdef _WIN32
#include <windows.h>
typedef HANDLE fileHandle;
#define PATH_DELIMITER '\\'
#define PATH_DELIMITER_STR "\\"
#elif defined __unix__
#include <sys/types.h>
#include <malloc.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
typedef int fileHandle;
#define PATH_DELIMITER '/'
#define PATH_DELIMITER_STR "/"
#define INVALID_HANDLE_VALUE -1
#endif

#define SIZE_KB ((uint32_t)1024)
#define SIZE_MB (SIZE_KB*1024)
#define SIZE_GB (SIZE_MB*1024)

class Stream;

#ifdef WIN32
#define FILE_OPEN_READ          0x80000000
#define FILE_OPEN_WRITE         0x40000000
#define FILE_OPEN_READWRITE     0xC0000000

#define FILE_SHARE_EXCLUSIVE    0x00
#define FILE_SHARE_DENY_WRITE   0x01
#define FILE_SHARE_DENY_READ    0x02
#define FILE_SHARE_DENY_NONE    0x03

#define FILE_CREATE_NEW         0x100
#define FILE_CREATE_ALWAYS      0x200

#define FILE_OPEN_EXISTING      0x300
#define FILE_OPEN_ALWAYS        0x400
#define FILE_TRUNCATE_EXISTING  0x500
#elif defined __unix__
#define FILE_OPEN_READ          O_RDONLY
#define FILE_OPEN_WRITE         O_WRONLY
#define FILE_OPEN_READWRITE     O_RDWR

#define FILE_SHARE_EXCLUSIVE    0x10
#define FILE_SHARE_DENY_WRITE   0x20
#define FILE_SHARE_DENY_READ    0       // not supported
#define FILE_SHARE_DENY_NONE    0x30

#define FILE_CREATE_NEW         O_CREAT | O_TRUNC
#define FILE_CREATE_ALWAYS      O_CREAT

#define FILE_OPEN_EXISTING      0       // not supported
#define FILE_OPEN_ALWAYS        O_CREAT
#define FILE_TRUNCATE_EXISTING  O_TRUNC
#endif
#define FILE_OPEN_READ_ST       FILE_OPEN_READ | FILE_SHARE_DENY_WRITE | FILE_OPEN_EXISTING
#define FILE_OPEN_WRITE_ST      FILE_OPEN_WRITE | FILE_SHARE_DENY_NONE | FILE_CREATE_ALWAYS
#define FILE_OPEN_READWRITE_ST  FILE_OPEN_READWRITE | FILE_SHARE_DENY_NONE | FILE_OPEN_EXISTING

/**
 * @brief Тип для работы с размером потока, 64 бита
 */
typedef int64_t StreamSize;

/**
 * @brief Перечисление типов изменения позиции в потоке
 */
enum ESeekMethod
{
    spBegin,
    spCurrent,
    spEnd
};

/**
 * @brief Структура для хранения сведения о файловом потоке
 */
struct StreamData
{
    /** @brief Флаг наличия изменений в потоке */
    bool isChange;
    /** @brief Флаг расположения данных потока в памяти */
    bool isExMem;
    /** @brief Идентификатор файла, связанного с потоком */
    fileHandle handle;
    /** @brief Текущий размер потока */
    StreamSize size;
    /** @brief Текущая позиция в потоке */
    StreamSize position;
    /** @brief Смещение потока в контейнере */
    StreamSize offset;
    /** @brief Указатель на родительский объект-контейнер */
    void* package;
    /** @brief Указатель на экземпляр потока */
    Stream* stream;
};

/**
 * @brief Базовый класс с методами файлового потока
 *
 * Содержит виртуальные методы, которые следует перегружать у всех потомков
 */
class StreamMethods
{
public:
    /**
     * @brief Изменение текущей позиции в файле
     * @param stream Экземпляр потока
     * @param moveTo Целевая позиция в файле
     * @param method Тип изменения позиции (абсолютный, относительный, от конца файла)
     * @return Текущая позиция в файле после её изменения
     */
    virtual StreamSize seek(Stream* stream, StreamSize moveTo, ESeekMethod method) = 0;
    /**
     * @brief Получение размера потока
     * @param stream Экземпляр потока
     */
    virtual StreamSize getSize(Stream* stream) = 0;
    /**
     * @brief Установить размер потока
     * @param stream Экземпляр потока
     * @param newSize Новый размер потока
     */
    virtual void setSize(Stream* stream, StreamSize newSize) = 0;
    /**
     * @brief Чтение данных из потока
     * @param stream Экземпляр потока
     * @param buf Буффер для данных
     * @param count Размер считываемых данных
     * @return Количество прочитанных байт
     */
    virtual StreamSize read(Stream* stream, char* buf, StreamSize count) = 0;
    /**
     * @brief Запись данных в поток
     * @param stream Экземпляр потока
     * @param buf Буффер с данными
     * @param count Размер записываемых данных
     * @return Количество записанных байт
     */
    virtual StreamSize write(Stream* stream, const void* buf, StreamSize count) = 0;
    /**
     * @brief Закрытие потока
     * @param stream Экземпляр потока
     */
    virtual void close(Stream* stream) = 0;
};

/**
 * @brief Класс файлового потока
 */
class Stream
{
public:
    /**
     * @brief Указатель на структуру с методами для работы с потоком
     */
    StreamMethods *m_methods;
    /**
     * @brief Указатель память с данными потока
     */
    uint8_t* m_memory;
public:
    /**
     * @brief Создание "потока-в-потоке"
     * @param methods Указатель на методы работы с поток родительского потока
     */
    Stream(StreamMethods* methods);
    /**
     * @brief Создание файлового потока
     * @param fileName Имя файла
     * @param mode Режим открытия файла
     */
    Stream(std::string fileName, const uint32_t mode);
    /**
     * @brief Создание потока в памяти
     */
    Stream();
    /**
     * @brief Создание потока в памяти заданного начального размера
     * @param streamSize Размер потока
     */
    Stream(StreamSize streamSize);
    /**
     * @brief Создание потока в памяти на основе имеющейся области памяти
     * @param data Указатель на область памяти с данными потока
     * @param size Размер области с данными
     */
    Stream(void* data, StreamSize size);
    virtual ~Stream();

    bool init() { return true; }
public:
    /**
     * @brief Структура с данными о потоке
     */
    StreamData m_data;
public:
    /**
     * @brief Получение идентификатора файлового потока.
     */
    fileHandle getFileStreamHandle();

    /**
     * @brief Получение указателя на область памяти с данными потока
     */
    void* getMemory();

    /**
     * @brief Получение методов работы с текущим файловым потоком
     */
    StreamMethods* getMethods();

    /**
     * @brief Чтение буффера из потока
     * @param buf Указатель на буффер
     * @param count Размер считываемых данных
     * @return Количество прочитанных данных
     */
    StreamSize read(void* buf, StreamSize count);
    /**
     * @brief Чтение NULL-терминантной строки из потока
     */
    std::string readStrZ();
    /**
     * @brief Чтение строки заданной длины из потока
     */
    std::string readStrLen(int32_t len);
    /**
     * @brief Чтение Wide-NULL-терминантной строки из потока
     */
    std::wstring readWideStrZ();
    /**
     * @brief Чтение Wide-строки заданной длины из потока
     */
    std::wstring readWideStrLen(int32_t len);

    /**
     * @brief Запись данных в поток
     * @param buf Указатель на буффер с данными
     * @param count Размер записываемых данных
     * @return Количество записанных данных
     */
    StreamSize write(const void* buf, StreamSize count);
    /**
     * @brief Запись строки в поток
     */
    StreamSize writeStr(std::string str);
    /**
     * @brief Запись Wide-строки в поток
     */
    StreamSize writeWideStr(std::wstring str);

    /**
     * @brief Изменение позиции в потоке
     * @param moveTo Размер изменения
     * @param moveMethod Тип перемещения (абсолютный, относительный, от конца)
     * @return Текущая позиция после смены позиции
     */
    StreamSize seek(StreamSize moveTo, ESeekMethod moveMethod);

    /**
     * @brief Получение текущего размера потока
     */
    StreamSize getSize();
    /**
     * @brief Изменение размера потока
     */
    void setSize(StreamSize newSize);

    /**
     * @brief Получение текущей позиции в потоке
     */
    StreamSize getPosition();
    /**
     * @brief Установка текущей позиции в потоке
     */
    void setPosition(StreamSize newPosition);

    /**
     * @brief Проверка достижения конца потока
     */
    bool atEnd();
    /**
     * @brief Проверка успешности открытия потока
     */
    bool opened() { return getFileStreamHandle() != INVALID_HANDLE_VALUE; }

    /**
     * @brief Чтение одной строки из файла
     */
    std::string readLine();
    /**
     * @brief Чтение одной Wide-строки из файла
     */
    std::wstring readWideLine();
};

/**
 * @brief Метод переноса данных из "потока-в-памяти" в другой поток
 */
void StreamMem2Stream(Stream *src, Stream *dst);
/**
 * @brief Метод переноса данных из одного потока в другой
 */
void Stream2Stream(Stream *src, Stream *dst);

#endif
