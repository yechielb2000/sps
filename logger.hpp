#pragma once
#include <memory>
#include <spdlog/spdlog.h>

void setup_logger();

std::shared_ptr<spdlog::logger> get_logger();
