#include <string>
#include "log.h"

int main (int argc, char** argv)
{
        std::string s = "logging facility in C++";
        int x = 42;

        log_warning(s.c_str ());
        log_debug("test debug message: %d @ %p", x, &x);

        return 0;
}
