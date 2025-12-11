module;

#include "spdlog/spdlog.h"

export module LoggerConfig;

import STL;

namespace petools {

    export struct LoggerConfig {
        std::string log_file_path = "logs/log.txt";
        std::string log_pattern = "[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%#] %v";

        spdlog::level::level_enum log_level =
#ifdef _DEBUG
            spdlog::level::debug;
#else
            spdlog::level::warn;
#endif

        bool async_mode = false;
        size_t queue_size = 8192;
        size_t async_threads = 1;

        size_t max_file_size = 1048576 * 5;
        size_t max_files = 3;

        bool console_output =
#ifdef _DEBUG
            true;
#else
            false;
#endif

        bool validate() const noexcept {
            if (max_files == 0) {
                return false;
            }
            if (max_file_size < 1024) {
                return false;
            }
            if (async_mode && queue_size == 0) {
                return false;
            }
            return true;
        }
    };

} //namespace petools
