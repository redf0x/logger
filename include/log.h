#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus

#include "logger.h"
#include "file_logger.h"
#include "console_logger.h"

namespace logger {

void log_message (log_level level, const char* caller, unsigned line, const char* format, ...);

#define log_debug(fmt, ...) log_message(logger::log_level::DEBUG, __PRETTY_FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)
#define log_info(fmt, ...) log_message(logger::log_level::INFO, __PRETTY_FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)
#define log_warning(fmt, ...) log_message(logger::log_level::WARNING, __PRETTY_FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)
#define log_critical(fmt, ...) log_message(logger::log_level::CRITICAL, __PRETTY_FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)
#define log_fatal(fmt, ...) log_message(logger::log_level::FATAL, __PRETTY_FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)

}

#endif // __cplusplus

#endif // LOG_H
