#include <sstream>
#include <cstring>
#include <cassert>

#include "file_logger.h"

namespace logger {

void file_logger_sink::open (const std::string& name)
{
    out.open (name.c_str (), std::ios_base::binary | std::ios_base::out);
    assert(out.is_open () == true);
    out.precision (20);
}

void file_logger_sink::close ()
{
    if (out)
        out.close ();
}

void file_logger_sink::write (const std::string& message)
{
    out << message << std::endl << std::flush;
}

}
