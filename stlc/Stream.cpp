#include "Stream.h"

fileHandle inline fileCreate(const char* fileName, const uint32_t mode)
{
#ifdef WIN32
    uint32_t attr = (mode >> 16) & 0x1fff;
    if (attr == 0)
        attr = FILE_ATTRIBUTE_NORMAL;
    return CreateFileA(fileName, mode & 0xF0000000, mode & 0xF, nullptr, (mode >> 8) & 0xF, attr, 0);
#elif defined __unix__
    return open(fileName, mode, 0644);
#endif
}

StreamSize inline fileSeek(fileHandle handle, StreamSize moveTo, ESeekMethod method)
{
#ifdef WIN32
    uint32_t hiPtr = moveTo >> 32;
    uint32_t res = SetFilePointer(handle, uint32_t(moveTo), (LONG*)&hiPtr, method);
    if ((res == 0xFFFFFFFF) && (GetLastError() != NO_ERROR))
        return -1;
    else
        return (res | ((uint64_t)hiPtr << 32));
#elif defined __unix__
    return lseek(handle, moveTo, method);
#endif
}

StreamSize inline fileRead(fileHandle handle, void* buf, StreamSize count)
{
#ifdef WIN32
    uint32_t res = 0;
    if (!ReadFile(handle, buf, count, (DWORD*)&res, nullptr))
        return 0;
#elif defined __unix__
    int32_t res = read(handle, buf, count);
    if (res == -1)
        return 0;
#endif
    return res;
}

StreamSize inline fileWrite(fileHandle handle, const void* buf, StreamSize count)
{
#ifdef WIN32
    uint32_t res = 0;
    if (!WriteFile(handle, buf, count, (DWORD*)&res, nullptr))
        return 0;
#elif defined __unix__
    uint32_t res = write(handle, buf, count);
    if (res == (uint32_t)-1)
        return 0;
#endif
    return res;
}

void inline fileClose(fileHandle handle)
{
#ifdef WIN32
    CloseHandle(handle);
#elif defined __unix__
    close(handle);
#endif
}

/**
 * @brief Класс для работы с файловыми потоками
 */
class BaseFileMethods : public StreamMethods
{
public:
    StreamSize seek(Stream* stream, StreamSize moveTo, ESeekMethod method)
    {
        return fileSeek(stream->m_data.handle, moveTo, method);
    };
    StreamSize getSize(Stream* stream)
    {
#ifdef WIN32
        uint32_t sizeHi, res = GetFileSize(stream->m_data.handle, (DWORD*)&sizeHi);
        return (res | ((uint64_t)sizeHi << 32));
#elif defined __unix__
        StreamSize pos = stream->getPosition();
        StreamSize res = stream->seek(0, spEnd);
        stream->seek(pos, spBegin);
        return res;
#endif
    }
    void setSize(Stream* stream, StreamSize newSize)
    {
        uint32_t p = stream->getPosition();
        stream->setPosition(newSize);
#ifdef WIN32
        SetEndOfFile(stream->getFileStreamHandle());
#elif defined __unix__
#endif
        if (p < newSize)
            stream->setPosition(p);
    }
    StreamSize read(Stream* stream, char* buf, StreamSize count)
    {
        return fileRead(stream->m_data.handle, buf, count);
    }
    StreamSize write(Stream* stream, const void* buf, StreamSize count)
    {
        return fileWrite(stream->m_data.handle, buf, count);
    }
    void close(Stream* stream)
    {
        fileClose(stream->m_data.handle);
        stream->m_data.handle = INVALID_HANDLE_VALUE;
    }
};

static StreamMethods *baseFileMethods = new BaseFileMethods();

/**
 * @brief Класс для работы с потоками в памяти
 */
class BaseMemoryMethods : public StreamMethods
{
public:
    virtual StreamSize seek(Stream* stream, StreamSize moveTo, ESeekMethod method)
    {
        uint32_t newPos;
        switch (method)
        {
        case spBegin:
            newPos = moveTo;
            break;
        case spCurrent:
            newPos = stream->m_data.position + moveTo;
            break;
        case spEnd:
            newPos = stream->m_data.size + moveTo;
            break;
        default:
            newPos = stream->m_data.position;
        }
        if (newPos > stream->m_data.size)
            stream->setSize(newPos);
        stream->m_data.position = newPos;
        return newPos;
    }
    virtual StreamSize getSize(Stream* stream)
    {
        return stream->m_data.size;
    }
    virtual void setSize(Stream* stream, StreamSize newSize)
    {
        if (stream->m_data.size < newSize)
        {
            StreamSize newCap = newSize;
            if (stream->m_memory == nullptr)
            {
                if (newSize != 0)
                    stream->m_memory = new uint8_t[newCap];
            }
            else
            {
                void *tmp = realloc(stream->m_memory, newCap);
                if (tmp != nullptr)
                    stream->m_memory = (uint8_t*)tmp;
            }
            stream->m_data.size = newCap;
        }
        else
        {
            if ((newSize == 0) && (stream->getSize() > 0) && (stream->m_memory != nullptr))
            {
                delete (char*)stream->m_memory;
                stream->m_memory = nullptr;
                stream->m_data.size = 0;
            }
        }
        stream->m_data.size = newSize;
        if (stream->m_data.position > stream->m_data.size)
            stream->m_data.position = stream->m_data.size;
    }
    virtual StreamSize read(Stream* stream, char* buf, StreamSize count)
    {
        if ((stream->m_data.position + count) > stream->m_data.size)
            count = stream->m_data.size - stream->m_data.position;
        buf = (char*)memcpy((void*)buf, (void*)(stream->m_memory + stream->m_data.position), count);
        stream->m_data.position += count;
        return count;
    }
    virtual StreamSize write(Stream* stream, const void* buf, StreamSize count)
    {
        if ((count + stream->m_data.position) > stream->m_data.size)
            stream->setSize(stream->m_data.position + count);
        memcpy((void*)(stream->m_memory + stream->m_data.position), buf, count);
        stream->m_data.position += count;
        return count;
    }
    virtual void close(Stream* stream)
    {
        if ((stream->m_memory != nullptr) && (!stream->m_data.isExMem))
        {

            delete (char*)stream->m_memory;
            stream->m_memory = nullptr;
        }
    }
};

static StreamMethods* baseMemoryMethods = new BaseMemoryMethods();

////////////////////////////////////////////////////////////////////////////////
//                                TStream                                     //
////////////////////////////////////////////////////////////////////////////////

Stream::Stream(StreamMethods* methods)
{
    m_methods = methods;
    m_data.isExMem = false;
    m_data.isChange = false;
    m_data.position = 0;
    m_memory = nullptr;
    memset(&m_data, 0, sizeof(StreamData));
}

Stream::Stream(std::string fileName, const uint32_t mode)
    : Stream(baseFileMethods)
{
    if ((mode & FILE_OPEN_READWRITE) == FILE_OPEN_READWRITE)
    {
        //if (ex)
        m_data.isExMem = false;
    }
    m_data.handle = fileCreate(fileName.c_str(), mode);
}

Stream::Stream()
    : Stream(baseMemoryMethods)
{
}

Stream::Stream(StreamSize streamSize)
    : Stream(baseMemoryMethods)
{
    setSize(streamSize);
}

Stream::Stream(void*  data, StreamSize size)
    : Stream(baseMemoryMethods)
{
    m_memory = (uint8_t*)data;
    m_data.size = size;
    m_data.isChange = false;
    m_data.isExMem = true;
}

Stream::~Stream()
{
    seek(0, spBegin);
    m_methods->close(this);
}

fileHandle Stream::getFileStreamHandle()
{
    return m_data.handle;
}

void* Stream::getMemory()
{
    return m_memory;
}

StreamMethods* Stream::getMethods()
{
    return m_methods;
}

StreamSize Stream::read(void* buf, StreamSize count)
{
    return m_methods->read(this, (char*)buf, count);
}

std::string Stream::readStrZ()
{
    char c;
    uint32_t startPos = getPosition();
    do
    {
        if (read(&c, 1) == 0)
            break;
    }
    while (c != '\x00');
    int32_t length = getPosition() - startPos;
    seek(startPos, spBegin);
    return readStrLen(length);
}

std::string Stream::readStrLen(int32_t len)
{
    char res[len + 1];
    read(res, len);
    res[len] = '\x00';
    return res;
}

std::wstring Stream::readWideStrZ()
{
    wchar_t c;
    uint32_t startPos = getPosition();
    do
    {
        if (read(&c, 1) == 0)
            break;
    }
    while (c != L'\x00');
    int32_t length = getPosition() - startPos;
    seek(startPos, spBegin);
    return readWideStrLen(length);
}

std::wstring Stream::readWideStrLen(int32_t len)
{
    wchar_t res[len + 1];
    read(res, len);
    res[len] = L'\x00';
    return res;
}

StreamSize Stream::write(const void* buf, StreamSize count)
{
    return m_methods->write(this, buf, count);
}

StreamSize Stream::writeStr(std::string str)
{
    return write(str.c_str(), str.length() * sizeof(char));
}

StreamSize Stream::writeWideStr(std::wstring str)
{
    return write(str.c_str(), str.length() * sizeof(wchar_t));
}

StreamSize Stream::seek(StreamSize moveTo, ESeekMethod moveMethod)
{
    return m_methods->seek(this, moveTo, moveMethod);
}

StreamSize Stream::getSize()
{
    return m_methods->getSize(this);
}

void Stream::setSize(StreamSize newSize)
{
    m_methods->setSize(this, newSize);
}

StreamSize Stream::getPosition()
{
    return seek(0, spCurrent);
}

void Stream::setPosition(StreamSize newPosition)
{
    m_methods->seek(this, newPosition, spBegin);
}

bool Stream::atEnd()
{
    return (m_methods->seek(this, 0, spCurrent) >= m_methods->getSize(this));
}

#define STRING_SIZE 2048

std::string Stream::readLine()
{
    StreamSize start_pos = this->getPosition();

    char tmp[STRING_SIZE];
    read(tmp, STRING_SIZE);

    std::string str(tmp);
    size_t del_pos = str.find_first_of("\n\r");
    size_t del_max = str.find_first_not_of("\n\r", del_pos);
    str.resize(del_pos);

    setPosition(start_pos + del_max);

    return str;
}

std::wstring Stream::readWideLine()
{
    wchar_t c;
    std::wstring ret = L"";
    do
    {
        if (read(&c, sizeof(wchar_t)) == 0)
            break;
        ret += c;
        if (c == L'\r')
            ret.pop_back();
    }
    while (c != L'\n');
    ret.pop_back();
    return ret;
}

void StreamMem2Stream(Stream *src, Stream *dst)
{
    StreamSize size = src->getSize();
    dst->write(&((uint8_t*)src->getMemory())[src->getPosition()], size);
    //dst->seek(0, spBegin);
}

void Stream2Stream(Stream *src, Stream *dst)
{
    StreamSize size = src->getSize();
    uint8_t *buf = new uint8_t[size];
    src->read(buf, size);
    dst->write(buf, size);
    delete []buf;
    dst->seek(0, spBegin);
}
