#pragma once

#include <sys/types.h>

extern const char * shiftjis[64];

size_t read_shiftjis_string(char ** dest, const char * str, off_t len);
