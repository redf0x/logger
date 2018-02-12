#include <iostream>

#include "console_logger.h"

namespace logger {

void console_logger_sink::write (const std::string& message)
{
    std::cerr << message << std::endl << std::flush;
}

}
