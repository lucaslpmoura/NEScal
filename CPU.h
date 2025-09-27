#ifndef CPU_H
#define CPU_H

#include "instructions.h"
#include "types.h"
#include "MemMapping.h"

#include "RAM.h"
#include "ROM.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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

    RAM ram;
    ROM rom;
}CPU; 

extern CPU *cpu;

void initCPU(RAM *ram, ROM *rom);
void emulate();


byte read(addr address);
void write(addr address, byte value);


void load(byte *reg, byte value);

void push(byte value);
void pushProcessorFlags();
byte pull();
void pullProcessorFlags();

int branch(bool flag);
void increment(addr addr, byte *value);
void decrement(addr addr, byte *value);
void transfer(byte *sourceRegister, byte *destinationRegister, bool changeFlags);

void adc(byte value);
void sbc(byte value);
void asl(addr addr, byte *value);
void lsr(addr addr, byte *value);
void rol(addr addr, byte *value);
void ror(addr addr, byte *value);

void ora(byte value);
void and(byte value);
void eor(byte value);

void cmp(byte value, byte *reg);
void bit(byte value);

int convertToSignedVal(byte num);

addr getAbsoluteAddress();
addr getZeroPageAddress();

void printTraceLog(int opcode);


#endif