//
// Created by Admin on 03.02.2022.
//

#include "Foundation/Logger.hpp"

namespace Foundation {

std::shared_ptr<spdlog::logger> Logger::s_logger;
EditorLogger::EditorLoggerCallback EditorLogger::s_callback;

void Logger::init() {
    spdlog::set_pattern("%^[%T] %n: %v%$");

    s_logger = spdlog::stdout_color_mt("NEST");
    s_logger->set_level(spdlog::level::trace);
}

void EditorLogger::init(EditorLoggerCallback callback) {
    s_callback = callback;
}

void EditorLogger::log(std::string_view message, MessageType type) {
    s_callback(message, type);
}

} // namespace Foundation