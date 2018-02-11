#include "logger.h"

logger::logger<logger::file_logger_sink> _log("test.log", logger::log_level::DEBUG);

int main ()
{
        unsigned x = 42;

        _log.log<logger::log_level::DEBUG>("test message");
        _log.log<logger::log_level::WARNING>("test message x = ", x);
        _log.log<logger::log_level::WARNING>("test message @x = ", &x);
        _log.log<logger::log_level::CRITICAL>("test message");

        return 0;
}
