#include <sstream>
#include <cstring>
#include <cassert>
#include <syslog.h>
#include <string.h>

#include "syslog_logger.h"

#define SYSLOG_FLAGS    (LOG_PID | LOG_NDELAY)

namespace logger {

syslog_logger_sink::syslog_logger_sink() : id(nullptr)
{
    setlogmask (LOG_UPTO(LOG_DEBUG));
}

void syslog_logger_sink::open (const std::string& name)
{
    if (!name.empty ())
        id = strdup (name.c_str ());

    openlog (id, SYSLOG_FLAGS, LOG_USER);
}

void syslog_logger_sink::close ()
{
    closelog ();
    free (id);
}

void syslog_logger_sink::write (const std::string& message)
{
    syslog (LOG_EMERG, "%s", message.c_str ());
}

}
