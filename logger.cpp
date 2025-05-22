#include "logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

static std::shared_ptr<spdlog::logger> logger;


void setup_logger() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    console_sink->set_pattern("[%T] [%^%l%$] %v");

    logger = std::make_shared<spdlog::logger>("main_logger", console_sink);
    spdlog::register_logger(logger);

    logger->set_level(spdlog::level::debug);
    logger->flush_on(spdlog::level::warn);
}

std::shared_ptr<spdlog::logger> get_logger() {
    return logger;
}

