#include "clog.h"

int main (int argc, char** argv)
{
	int x = 42;

    log_warning("logging facility in C");
	log_debug("test debug message: %d @ %p", x, &x);

	return 0;
}
