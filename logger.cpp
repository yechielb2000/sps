#include "logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

#include "spdlog/sinks/base_sink.h"

static std::shared_ptr<spdlog::logger> logger;

template<typename Mutex>
class exit_on_err_sink final : public spdlog::sinks::base_sink<Mutex> {
public:
    explicit exit_on_err_sink(std::shared_ptr<spdlog::sinks::sink> wrapped_sink)
        : wrapped_sink_(std::move(wrapped_sink)) {
    }

protected:
    void sink_it_(const spdlog::details::log_msg &msg) override {
        wrapped_sink_->log(msg);

        if (msg.level >= spdlog::level::err) {
            wrapped_sink_->flush();
            std::exit(EXIT_FAILURE);
        }
    }

    void flush_() override {
        wrapped_sink_->flush();
    }

private:
    std::shared_ptr<spdlog::sinks::sink> wrapped_sink_;
};

std::shared_ptr<spdlog::logger> get_logger() {
    return spdlog::get("main_logger");
}


void setup_logger() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    console_sink->set_pattern("[%T] [%^%l%$] %v");

    auto wrapped_sink = std::make_shared<exit_on_err_sink<std::mutex>>(console_sink);


    logger = std::make_shared<spdlog::logger>("main_logger", wrapped_sink);
    spdlog::register_logger(logger);

    logger->set_level(spdlog::level::info);
    logger->flush_on(spdlog::level::warn);
}
