#ifndef LOGGER_SINK_H
#define LOGGER_SINK_H

#include <string>

namespace logger {

class logger_sink {
public:
        virtual void open (const std::string& name) = 0;
        virtual void close () = 0;
        virtual void write (const std::string& msg) = 0;

        virtual ~logger_sink() = 0;
};

inline logger_sink::~logger_sink() { }

}

#endif // LOGGER_SINK_H
