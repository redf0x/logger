#ifndef SYSLOG_LOGGER_H
#define SYSLOG_LOGGER_H

#include <string>

#include "logger_sink.h"

namespace logger {

class syslog_logger_sink : public logger_sink {
public:
        syslog_logger_sink();
        ~syslog_logger_sink() override { }

        void open (const std::string& name) override;
        void close () override;
        void write (const std::string& message) override;

private:
        char* id;
};

}

#endif // SYSLOG_LOGGER_H
