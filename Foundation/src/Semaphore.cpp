#include "Foundation/PlatformDetection.hpp"
#include "Foundation/Foundation.hpp"

#include "Foundation/Semaphore.hpp"

#if defined(PLATFORM_MACOS) || defined(PLATFORM_IOS)
#    include <dispatch/dispatch.h>
#elif defined(PLATFORM_POSIX)
#    include <semaphore.h>
#    include <fcntl.h>
#elif defined(PLATFORM_WINDOWS)
#    include <windows.h>
#    include <limits.h>
#endif

namespace Foundation {

struct SemaphoreInternal {
#if defined(PLATFORM_MACOS) || defined(PLATFORM_IOS)
    dispatch_semaphore_t m_handle;
#elif defined(PLATFORM_POSIX)
    sem_t *m_sem;
    const char *name;
#elif defined(PLATFORM_WINDOWS)
    HANDLE m_handle;
#endif
};

#if defined(PLATFORM_MACOS) || defined(PLATFORM_IOS)

Semaphore::Semaphore(const char *name) {
    PND_STATIC_ASSERT(sizeof(SemaphoreInternal) <= sizeof(m_internal));

    SemaphoreInternal *si = (SemaphoreInternal *)m_internal;
    si->m_handle = dispatch_semaphore_create(0);
    PND_ASSERT(NULL != si->m_handle, "dispatch_semaphore_create failed.");
}

Semaphore::~Semaphore() {
    SemaphoreInternal *si = (SemaphoreInternal *)m_internal;
    dispatch_release(si->m_handle);
}

void Semaphore::post(uint32_t _count) {
    SemaphoreInternal *si = (SemaphoreInternal *)m_internal;

    for (uint32_t ii = 0; ii < _count; ++ii) {
        dispatch_semaphore_signal(si->m_handle);
    }
}

bool Semaphore::wait(int32_t _msecs) {
    SemaphoreInternal *si = (SemaphoreInternal *)m_internal;

    dispatch_time_t dt = 0 > _msecs ? DISPATCH_TIME_FOREVER
                                    : dispatch_time(DISPATCH_TIME_NOW, int64_t(_msecs) * 1000000);
    return !dispatch_semaphore_wait(si->m_handle, dt);
}

#elif defined(PLATFORM_POSIX)

Semaphore::Semaphore(const char *name) {
    PND_STATIC_ASSERT(sizeof(SemaphoreInternal) <= sizeof(m_internal));

    SemaphoreInternal *si = (SemaphoreInternal *)m_internal;
    si->name = name;

    sem_unlink(name);
    si->m_sem = sem_open(name, O_CREAT, 0644, 0);
    PND_ASSERT(si->m_sem != SEM_FAILED, "sem_open");
}

Semaphore::~Semaphore() {
    SemaphoreInternal *si = (SemaphoreInternal *)m_internal;

    int result = sem_close(si->m_sem);
    PND_ASSERT_F(0 == result, "sem_close %d", result);
    result = sem_unlink(si->name);
    PND_ASSERT_F(0 == result, "sem_unlink %d", result);
}

void Semaphore::post(uint32_t _count) {
    SemaphoreInternal *si = (SemaphoreInternal *)m_internal;

    int result;

    for (uint32_t ii = 0; ii < _count; ++ii) {
        result = sem_post(si->m_sem);
        PND_ASSERT_F(0 == result, "sem_post %d", result);
    }
}

bool Semaphore::wait(int32_t _msecs) {
    SemaphoreInternal *si = (SemaphoreInternal *)m_internal;

    int result = sem_wait(si->m_sem);
    PND_ASSERT_F(0 == result, "sem_wait %d", result);

    return 0 == result;
}

#elif defined(PLATFORM_WINDOWS)

Semaphore::Semaphore(const char *name) {
    SemaphoreInternal *si = (SemaphoreInternal *)m_internal;

    // #if PLATFORM_WINRT \
// ||  PLATFORM_XBOXONE
    si->m_handle = CreateSemaphoreExW(NULL, 0, LONG_MAX, NULL, 0, SEMAPHORE_ALL_ACCESS);
    // #else
    // 		si->m_handle = CreateSemaphoreA(NULL, 0, LONG_MAX, NULL);
    // #endif
    PND_ASSERT(NULL != si->m_handle, "Failed to create Semaphore!");
}

Semaphore::~Semaphore() {
    SemaphoreInternal *si = (SemaphoreInternal *)m_internal;

    CloseHandle(si->m_handle);
}

void Semaphore::post(uint32_t _count) {
    SemaphoreInternal *si = (SemaphoreInternal *)m_internal;

    ReleaseSemaphore(si->m_handle, _count, NULL);
}

bool Semaphore::wait(int32_t _msecs) {
    SemaphoreInternal *si = (SemaphoreInternal *)m_internal;

    DWORD milliseconds = (0 > _msecs) ? INFINITE : _msecs;
    // #if PLATFORM_WINRT \
// ||  PLATFORM_XBOXONE
    return WAIT_OBJECT_0 == WaitForSingleObjectEx(si->m_handle, milliseconds, FALSE);
    // #else
    //		return WAIT_OBJECT_0 == WaitForSingleObject(si->m_handle, milliseconds);
    // #endif
}
#endif // PLATFORM_

} // namespace Foundation
