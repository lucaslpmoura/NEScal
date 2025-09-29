#ifndef UTIL_H
#define UTIL_H

#include "types.h"

#include <stdbool.h>
#include <stdio.h>

bool isLineNotZero(byte *mem, uint16_t line);
void printMemory(byte *mem, size_t size);


#endif