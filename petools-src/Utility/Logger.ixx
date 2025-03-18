module;

#include "spdlog/spdlog.h"

export module Logger;

import STL;
import LoggerConfig;

namespace petools {

    export class Logger {
    private:
        Logger() = default;
        ~Logger() = default;

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
        Logger(Logger&& other) = delete;
        Logger& operator=(Logger&& other) = delete;

    public:
        static Logger& instance() {
            static Logger instance;
            return instance;
        }

        void init(const LoggerConfig& config = LoggerConfig{});
        const std::shared_ptr<spdlog::logger> get_logger() const noexcept { return logger_; }
        bool is_initialized() const noexcept { return logger_ != nullptr; }
        void set_level(spdlog::level::level_enum level);

    private:
        std::shared_ptr<spdlog::logger> logger_ = nullptr;
        LoggerConfig config_{};

        inline static int init_count_ = 0;
    };

	export inline auto& logger = Logger::instance().get_logger();

	export template <typename... Args>
        void debug(Args&&... args) {
        logger->debug(std::forward<Args>(args)...);
    }

} //namespace petools