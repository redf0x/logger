#ifndef CLOG_H
#define CLOG_H

#ifdef __cplusplus
extern "C" {
#endif

void stub_log_debug(const char* caller, unsigned line, const char* format, ...);
void stub_log_info(const char* caller, unsigned line, const char* format, ...);
void stub_log_warning(const char* caller, unsigned line, const char* format, ...);
void stub_log_critical(const char* caller, unsigned line, const char* format, ...);
void stub_log_fatal(const char* caller, unsigned line, const char* format, ...);

#ifdef __cplusplus
}
#endif

#ifndef __cplusplus

#undef log_debug
#undef log_info
#undef log_warning
#undef log_critical
#undef log_fatal

#define log_debug(fmt, ...) stub_log_debug(__PRETTY_FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)
#define log_info(fmt, ...) stub_log_info(__PRETTY_FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)
#define log_warning(fmt, ...) stub_log_warning(__PRETTY_FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)
#define log_critical(fmt, ...) stub_log_critical(__PRETTY_FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)
#define log_fatal(fmt, ...) stub_log_fatal(__PRETTY_FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)

#endif

#endif // CLOG_H
