#pragma once

#include "Foundation/Logger.hpp"

namespace Foundation {

#define NEST_STATIC_ASSERT(_condition, ...) static_assert(_condition, "" __VA_ARGS__)

#define NEST_ASSERT(_condition, _message)                                                          \
    if (_condition == false) {                                                                     \
        LOG_CRITICAL("CHECK " _message);                                                           \
        assert(false);                                                                             \
    }

#define NEST_ASSERT_F(_condition, _format, ...)                                                    \
    if (_condition == false) {                                                                     \
        LOG_CRITICAL("CHECK " _format, ##__VA_ARGS__);                                             \
        assert(false);                                                                             \
    }

} // namespace Foundation