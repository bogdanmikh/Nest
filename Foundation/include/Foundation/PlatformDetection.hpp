#pragma once

// Platform detection using predefined macros
#ifdef _WIN32
#    ifdef _WIN64
#        define PLATFORM_WINDOWS
#    else
#        error "x86 Builds are not supported!"
#    endif
#elif defined(__APPLE__) || defined(__MACH__)
#    include <TargetConditionals.h>
/* TARGET_OS_MAC exists on all the platforms
 * so we must check all of them (in this order)
 * to ensure that we're running on MAC
 * and not some other Apple platform */
#    if TARGET_IPHONE_SIMULATOR == 1
#        define PLATFORM_IOS
#    elif TARGET_OS_IPHONE == 1
#        define PLATFORM_IOS
#    elif TARGET_OS_MAC == 1
#        define PLATFORM_MACOS
#    else
#        error "Unknown Apple platform!"
#    endif
/* We also have to check __ANDROID__ before __linux__
 * since android is based on the linux kernel
 * it has __linux__ defined */
#elif defined(__ANDROID__)
#    define PLATFORM_ANDROID
#    error "Android is not supported!"
#elif defined(__linux__)
#    define PLATFORM_LINUX
#else
/* Unknown compiler/platform */
#    error "Unknown platform!"
#endif // End of platform detection

#if defined(PLATFORM_WINDOWS) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS)
#    define PLATFORM_DESKTOP
#endif

#if defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS) || defined(PLATFORM_LINUX) ||               \
    defined(PLATFORM_MACOS)
#    define PLATFORM_POSIX
#endif
