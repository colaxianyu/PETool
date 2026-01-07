#pragma once

#include "spdlog/spdlog.h"

extern std::shared_ptr<spdlog::logger> GetLoggerInstance();

#define EXPAND(x) x
#define GET_MACRO_COUNT(_1, _2, _3, COUNT, ...) COUNT

#define SPDLOG_LOGGER_CALL_(level, ...) GetLoggerInstance()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, level, __VA_ARGS__)
#define LOGGER_TRACE(...) SPDLOG_LOGGER_CALL_(spdlog::level::trace, __VA_ARGS__)
#define LOGGER_DEBUG(...) SPDLOG_LOGGER_CALL_(spdlog::level::debug, __VA_ARGS__)
#define LOGGER_INFO(...) SPDLOG_LOGGER_CALL_(spdlog::level::info, __VA_ARGS__)
#define LOGGER_WARN(...) SPDLOG_LOGGER_CALL_(spdlog::level::warn, __VA_ARGS__)
#define LOGGER_ERROR(...) SPDLOG_LOGGER_CALL_(spdlog::level::err, __VA_ARGS__)

//#ifdef _DEBUG
//
//#define LOG_HANDLE_DEBUG_3(msg, handle, error) \
//    LOGGER_DEBUG("WinHandle: {} handle: {:#x} (error: {:#x})", \
//        msg, reinterpret_cast<UINT_PTR>(handle), static_cast<uint32_t>(error))
//
//#define LOG_HANDLE_DEBUG_2(msg, handle) \
//    LOGGER_DEBUG("WinHandle: {} handle: {:#x}", \
//        msg, reinterpret_cast<UINT_PTR>(handle))
//
//#define LOG_HANDLE_DEBUG(...) \
//    EXPAND(GET_MACRO_COUNT(__VA_ARGS__, LOG_HANDLE_DEBUG_3, LOG_HANDLE_DEBUG_2)(__VA_ARGS__))
//#else
//#define LOG_HANDLE_DEBUG_3
//#define LOG_HANDLE_DEBUG_2
//#define LOG_HANDLE_DEBUG(...)
//#endif
