//
// Created by Admin on 03.02.2022.
//

#include "Foundation/Logger.hpp"

namespace Foundation {

std::shared_ptr<spdlog::logger> Logger::s_logger;

void Logger::init() {
    spdlog::set_pattern("%^[%T] %n: %v%$");

    s_logger = spdlog::stdout_color_mt("PANDA");
    s_logger->set_level(spdlog::level::trace);
}

} // namespace Foundation