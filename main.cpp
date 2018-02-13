#include <cstdarg>
#include <limits>
#include <cstddef>
#include <string>

#include "log.h"
#include "clog.h"

#include "syslog_logger.h"

namespace logger {

logger<syslog_logger_sink> _log("test-id", log_level::DEBUG, logger_flags::flag_none);

#define __log_debug _log.log<log_level::DEBUG>
#define __log_info  _log.log<log_level::INFO>
#define __log_warning   _log.log<log_level::WARNING>
#define __log_critical  _log.log<log_level::CRITICAL>
#define __log_fatal _log.log<log_level::FATAL>

static char* make_message (const char* source, unsigned line, const char* format, va_list ap)
{
    char* message = NULL;
    std::string fmt;
    int ret;

    fmt = std::string(source) + std::string(":") + std::to_string (line) +
            std::string(": ") + std::string(format);
    ret = vasprintf (&message, fmt.c_str (), ap);

    return (ret != -1) ? message : nullptr;
}

static void log_message_internal (log_level level, const char* caller, unsigned line, const char* format, va_list ap)
{
    char* message;

    message = make_message (caller, line, format, ap);

    if (message == nullptr)
        return;

    switch (level) {
        case log_level::DEBUG:
            __log_debug(message);
            break;

        case log_level::INFO:
            __log_info(message);
            break;

        case log_level::WARNING:
            __log_warning(message);
            break;

        case log_level::CRITICAL:
            __log_critical(message);
            break;

        case log_level::FATAL:
            __log_fatal(message);
            break;
    }

    free (message);

    return;
}

void log_message (log_level level, const char* caller, unsigned line, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    log_message_internal (level, caller, line, format, ap);
}

}

void stub_log_debug (const char* caller, unsigned line, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    log_message_internal (logger::log_level::DEBUG, caller, line, format, ap);
}

void stub_log_info (const char* caller, unsigned line, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    log_message_internal (logger::log_level::INFO, caller, line, format, ap);
}

void stub_log_warning (const char* caller, unsigned line, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    log_message_internal (logger::log_level::WARNING, caller, line, format, ap);
}

void stub_log_critical (const char* caller, unsigned line, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    log_message_internal (logger::log_level::CRITICAL, caller, line, format, ap);
}

void stub_log_fatal (const char* caller, unsigned line, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    log_message_internal (logger::log_level::FATAL, caller, line, format, ap);
}

#ifdef LOGGER_TEST

int main ()
{
    unsigned x = 42;

    log_debug("test message: %d", 42);
    log_warning("test message: %d", x);
    log_warning("test message: %p", &x);
    log_critical("test message");

    return 0;
}

#endif // LOGGER_TEST
