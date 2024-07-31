#pragma once

#define INVALID_HANDLE_VALUE -1

namespace Foundation {

typedef int32_t (*ThreadFn)(class Thread *_self, void *_userData);

class Thread {
public:
    Thread();
    virtual ~Thread();
    bool
    init(ThreadFn _fn, void *_userData = NULL, uint32_t _stackSize = 0, const char *_name = NULL);
    void shutdown();
    bool isRunning() const;
    int32_t getExitCode() const;
    void setThreadName(const char *_name);

private:
    friend struct ThreadInternal;
    int32_t entry();

    uint8_t m_internal[64];
    ThreadFn m_fn;
    void *m_userData;
    uint32_t m_stackSize;
    int32_t m_exitCode;
    bool m_running;
};

} // namespace Foundation