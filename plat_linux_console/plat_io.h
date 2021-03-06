/*

plat_io.h - platform dependent printf, input, etc.

*/

#ifndef PLAT_IO_H_INCLUDED
#define PLAT_IO_H_INCLUDED

#include <cstdarg>

// platform specific printf
void plprintf(const char* fmt, ...);

#endif
