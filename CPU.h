#ifndef CPU_H
#define CPU_H


#include "types.h"

#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"

typedef struct CPU{
    addr PC; // Program Counter  (16 Bit)
    byte SP; // Stack Pointer (8 Bit)

    byte A; // A Register
    byte X; // X Register
    byte Y; // Z Register

    bool HLT; // HALT Flag
    bool CRY; // CARRY Flag
    bool ZER; // ZERO Flag
    bool IND; // INTERRUPT DISABLE Flag
    bool DEC; // DECIMAL Flag
    bool OVF; // OVERFLOW Flag
    bool NEG; // Negative Flag
}CPU; 

CPU *InitCPU();
void run();

#endif