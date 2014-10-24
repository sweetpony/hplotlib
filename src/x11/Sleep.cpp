#include "../Sleep.hpp"

#include <unistd.h>

void hpl::sleep(unsigned useconds)
{
	usleep(useconds);
}
