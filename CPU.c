#include "CPU.h"

CPU *InitCPU()
{
    CPU *cpu = (CPU *)malloc(sizeof(CPU));

    // Pointes
    cpu->PC = 0x0000; // Program Count
    cpu->SP = 0x00; // Stack Pointer

    // Registers
    cpu->A = 0x00;
    cpu->X = 0x00;
    cpu->Y = 0x00;

    // Flags
    cpu->HLT = 0;
    cpu->CRY = 0;
    cpu->ZER = 0;
    cpu->IND = 0;
    cpu->DEC = 0;
    cpu->OVF = 0;
    cpu->NEG = 0;

    return cpu;
}

