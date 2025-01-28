#pragma once

#include <cstdlib>

namespace Foundation {

class Semaphore {
public:
    Semaphore(const char *name);
    ~Semaphore();
    void post(uint32_t _count = 1);
    bool wait(int32_t _msecs = -1);

private:
    uint8_t m_internal[128];
};

} // namespace Foundation