
#include "LinuxHelper.hpp"

#ifdef LINUX

#include <X11/Xlib.h>

void init()
{
    XInitThreads();
}

#endif
