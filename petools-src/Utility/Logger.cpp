module;

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

module Logger;
Logger& Logger::instance() {
	static Logger instance;
	return instance;
}

void Logger::init() {
#ifdef _DEBUG
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

	logger_ = std::make_shared<spdlog::logger>("debug_logger", spdlog::sinks_init_list{ console_sink });
	logger_->set_level(spdlog::level::debug);
#else
	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/log.txt", true);
	file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

	logger_ = std::make_shared<spdlog::logger>("release_logger", spdlog::sinks_init_list{ file_sink });
	logger_->set_level(spdlog::level::warn);
#endif 
	logger_->flush_on(spdlog::level::warn);
	spdlog::register_logger(logger_);
}
