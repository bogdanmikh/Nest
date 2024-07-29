#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

class Logger {
public:
    static void init() {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_logger = spdlog::stdout_color_mt("NEST");
        s_logger->set_level(spdlog::level::trace);
    }
    inline static std::shared_ptr<spdlog::logger> &getLogger() {
        return s_logger;
    };

private:
    static std::shared_ptr<spdlog::logger> s_logger;
};

#define LOG_TRACE(...) Logger::getLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...) Logger::getLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) Logger::getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) Logger::getLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)                                                                          \
    Logger::getLogger()->critical(__VA_ARGS__);                                                    \
    assert(false)