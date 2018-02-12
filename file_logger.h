#ifndef FILE_LOGGER_H
#define FILE_LOGGER_H

#include <fstream>

#include "logger_sink.h"

namespace logger {

class file_logger_sink : public logger_sink {
        std::ofstream out;

public:
        file_logger_sink() { }
        ~file_logger_sink() { }

        void open (const std::string& name) override;
        void close () override;
        void write (const std::string& message) override;
};

}

#endif // FILE_LOGGER_H
