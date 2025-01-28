#pragma once

#include "Foundation/Allocator.hpp"
#include "Foundation/Assert.hpp"

#include <string>

#define COMMAND_BUFFER_ALIGNMENT 64
// Do you remember what game this cheat is from?
#define COMMAND_BUFFER_FINISH_KEY 787898

namespace Foundation {

class CommandBuffer {
    struct Header {
        uint32_t size;
        uint16_t align;

        Header(uint32_t size, uint16_t align)
            : size(size)
            , align(align) {}
    };

public:
    struct Command {
        uint16_t type;

        Command(uint16_t type)
            : type(type) {}

        Command()
            : type(0) {}
    };

    CommandBuffer(uint32_t size)
        : m_size(size)
        , m_pos(0) {
        m_data = (uint8_t *)F_ALLOC(getAllocator(), size);
    }

    ~CommandBuffer() {
        F_FREE(getAllocator(), m_data);
    }

    template<typename CMD>
    void write(CMD &cmd) {
        static_assert(std::is_base_of_v<Command, CMD>, "Not inherited from Command");
        Header header(sizeof(CMD), __alignof(CMD));
        writeHeader(header);
        align(__alignof(CMD));
        write(&cmd, sizeof(CMD));
    }

    Command *read() {
        Header &header = readHeader();
        if (m_pos == 0 || header.size == COMMAND_BUFFER_FINISH_KEY) {
            return nullptr;
        }
        align(header.align);
        Command *cmd = (Command *)read(header.size);
        return cmd;
    }

    // Finish writing commands. Start reading
    void finishWriting() {
        Header header(COMMAND_BUFFER_FINISH_KEY, 0);
        writeHeader(header);
        m_pos = 0;
    }

    void reset() {
        m_pos = 0;
    }

private:
    void writeHeader(Header &header) {
        align(__alignof(Header));
        write(&header, sizeof(Header));
    }

    Header &readHeader() {
        align(__alignof(Header));
        return *(Header *)read(sizeof(Header));
    }

    void write(const void *_data, uint32_t _size) {
        memcpy(&m_data[m_pos], _data, _size);
        m_pos += _size;
        NEST_ASSERT(m_pos < m_size, "NEED MORE SPACE FOR BUFFER");
    }

    void *read(uint32_t _size) {
        uint32_t pos = m_pos;
        m_pos += _size;
        return &m_data[pos];
    }

    void align(uint32_t _alignment) {
        const uint32_t mask = _alignment - 1;
        const uint32_t pos = (m_pos + mask) & (~mask);
        m_pos = pos;
    }

    uint32_t m_size;
    uint32_t m_pos;
    uint8_t *m_data;
};

} // namespace Foundation