#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H

#include <string>

#include "logger_sink.h"

namespace logger {

class console_logger_sink : public logger_sink {
public:
        console_logger_sink() { }
        ~console_logger_sink() { }

        void open (const std::string& name) override { }
        void close () override { }
        void write (const std::string& message) override;
};

}

#endif // CONSOLE_LOGGER_H
