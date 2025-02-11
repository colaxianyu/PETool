module;

#include "spdlog/spdlog.h"

export module Logger;

import STL;

export class Logger {
private:
	Logger() = default;
	~Logger() = default;

	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;
	Logger(Logger&& other) = delete;
	Logger& operator=(Logger&& other) = delete;

public:
	static Logger& instance();

	void init();
	const std::shared_ptr<spdlog::logger> get_logger() const noexcept { return logger_; }
private:
	std::shared_ptr<spdlog::logger> logger_;
};

export template <typename... Args>
void debug(fmt::format_string<Args...> fmt, Args&&... args) {
	Logger::instance().get_logger()->debug(fmt, std::forward<Args>(args)...);
}

export template <typename... Args>
void info(fmt::format_string<Args...> fmt, Args&&... args) {
	Logger::instance().get_logger()->info(fmt, std::forward<Args>(args)...);
}

export template <typename... Args>
void warn(fmt::format_string<Args...> fmt, Args&&... args) {
	Logger::instance().get_logger()->warn(fmt, std::forward<Args>(args)...);
}

export template <typename... Args>
void error(fmt::format_string<Args...> fmt, Args&&... args) {
	Logger::instance().get_logger()->error(fmt, std::forward<Args>(args)...);
}