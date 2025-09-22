#ifndef CPU_H
#define CPU_H
#endif

#include "types.h"

#include "stdlib.h"
#include "stdio.h"

typedef  struct CPU {
    addr PC; // Program Counter  (16 Bit)
    byte A; // A Register
    byte X; // X Register
    byte Z; // Z Register

}CPU; 

CPU InitCPU();
