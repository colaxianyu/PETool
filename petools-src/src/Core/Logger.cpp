module;

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/async.h"

module Logger;

namespace petools {

    void Logger::init(const LoggerConfig& config) {
        config_ = config;

        try {
            if (config_.async_mode) {
                spdlog::init_thread_pool(config_.queue_size, config_.async_threads);
            }

            std::vector<spdlog::sink_ptr> sinks;

            if (config_.console_output) {
                auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
                console_sink->set_pattern(config_.log_pattern);
                sinks.push_back(console_sink);
            }

            auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                config_.log_file_path,
                config_.max_file_size,
                config_.max_files
            );
            file_sink->set_pattern(config_.log_pattern);
            sinks.push_back(file_sink);

            if (config_.async_mode) {
                logger_ = std::make_shared<spdlog::async_logger>(
                    "logger",
                    sinks.begin(),
                    sinks.end(),
                    spdlog::thread_pool(),
                    spdlog::async_overflow_policy::block
                );
            }
            else {
                logger_ = std::make_shared<spdlog::logger>("logger", sinks.begin(), sinks.end());
            }

            logger_->set_level(config_.log_level);
            logger_->flush_on(spdlog::level::warn);
            spdlog::register_logger(logger_);

        }
        catch (const spdlog::spdlog_ex& ex) {
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            logger_ = std::make_shared<spdlog::logger>("fallback_logger", console_sink);
            logger_->set_level(spdlog::level::warn);
            logger_->warn("Logger initialization failed: {}. Falling back to console logging.", ex.what());
        }
    }

    void Logger::set_level(spdlog::level::level_enum level) {
        if (logger_) {
            logger_->set_level(level);
        }
    }

} // namespace petools