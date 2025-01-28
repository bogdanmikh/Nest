//
// Created by Admin on 03.02.2022.
//

#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Foundation {

class Logger {
public:
    static void init();
    inline static std::shared_ptr<spdlog::logger> &getLogger() {
        return s_logger;
    };

private:
    static std::shared_ptr<spdlog::logger> s_logger;
};

class EditorLogger {
public:
    enum MessageType { INFO, WARNING, ERROR };
    using EditorLoggerCallback = void (*)(std::string_view message, MessageType type);

    static void init(EditorLoggerCallback callback);
    static void log(std::string_view message, MessageType type);

    template<typename... Args>
    using format_string_t = fmt::format_string<Args...>;

    template<typename... Args>
    static void log(MessageType type, format_string_t<Args...> fmt, Args &&...args) {
        fmt::basic_memory_buffer<char, 250> buf;
        fmt::vformat_to(fmt::appender(buf), fmt, fmt::make_format_args(args...));
        log(std::string_view(buf.data(), buf.size()), type);
    }

private:
    static EditorLoggerCallback s_callback;
};

} // namespace Foundation

#define LOG_TRACE(...) ::Foundation::Logger::getLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...) ::Foundation::Logger::getLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) ::Foundation::Logger::getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::Foundation::Logger::getLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::Foundation::Logger::getLogger()->critical(__VA_ARGS__)

#define LOG_INFO_EDITOR(...)                                                                       \
    ::Foundation::EditorLogger::log(Foundation::EditorLogger::MessageType::INFO, __VA_ARGS__)
#define LOG_WARN_EDITOR(...)                                                                       \
    ::Foundation::EditorLogger::log(Foundation::EditorLogger::MessageType::WARNING, __VA_ARGS__)
#define LOG_ERROR_EDITOR(...)                                                                      \
    ::Foundation::EditorLogger::log(Foundation::EditorLogger::MessageType::ERROR, __VA_ARGS__)