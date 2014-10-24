#include "../Sleep.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void hpl::sleep(unsigned useconds)
{
	Sleep(useconds);
}
