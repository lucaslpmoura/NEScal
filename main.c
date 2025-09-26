#include "CPU.h"
#include "instructions.h"

#include "RAM.h"
#include "ROM.h"
#include "MemMapping.h"

#include "stdio.h"
#include "stdlib.h"

#define STEPPING_MODE 1

char *romFileName = "./tst/1_Example.nes";

CPU *cpu;

RAM ram;
ROM rom;

FILE *romFile;
long romFileLength;
uint8_t *buffer;

void run();
void reset();
void emulate();

byte read(addr address);
void write(addr address, byte value);

byte *readFileAsBytes();
void copyByteArray(byte *inArray, byte *outArray, size_t size, size_t offset);
int convertToSignedVal(byte num);

addr getAbsoluteAddress();

void push(byte value);
byte pull();

void load(byte *reg, byte value);
int branch(bool flag);
void transfer(byte *sourceRegister, byte *destinationRegister, bool changeFlags);
void asl(addr addr, byte *value);

int main(int argv, char **argc)
{
    if (argv == 2)
    {
        romFileName = argc[1];
    }
    cpu = InitCPU();
    ram = InitRAM();
    rom = InitROM();

    // createRomFile();

    printf("Executing: %s\n", romFileName);
    reset();
    printf("PC: %x\n", cpu->PC);
    run(cpu);
    return 0;
}

void run(CPU *cpu)
{
    while (!cpu->HLT)
    {
        if (STEPPING_MODE)
        {
            printf("Press any key to run.");
            getchar();
        }

        emulate(cpu);
    }
    printf("A: %x, X: %x, Y: %x\n", cpu->A, cpu->X, cpu->Y);
    printf("RAM: ");
    printMemory(ram, 0, RAM_SIZE);
}

void emulate(CPU *cpu)
{
    byte opcode = read(cpu->PC);
    printf("PC: %x, OP: %x\n", cpu->PC, opcode);
    cpu->PC++;

    int cycles = 0;
    switch (opcode)
    {
    // Halt (Unnoficial)
    case HTL:
        cpu->HLT = 1;
        break;
    // No Operation
    case NOP:
        cycles = 2;
        break;

    // Load Register Instructions
    // Load X
    case LDX_IM:
        load(&cpu->X, read(cpu->PC));

        cycles = 2;
        cpu->PC++;
        break;
    case LDX_ZP:
    {
        load(&cpu->X, read(getZeroPageAddress()));

        cycles = 3;
        break;
    }
    case LDX_AB:
    {
        load(&cpu->X, read(getAbsoluteAddress()));

        cycles = 4;
        break;
    }

    // Load Y
    case LDY_IM:
        load(&cpu->Y, read(cpu->PC));

        cpu->PC++;
        cycles = 2;
        break;
    case LDY_ZP:
    {
        load(&cpu->Y, read(getZeroPageAddress()));

        cycles = 3;
        break;
    }
    case LDY_AB:
    {
        load(&cpu->Y, read(getAbsoluteAddress()));

        cycles = 4;
        break;
    }

    // Load A
    case LDA_IM:
        load(&cpu->A, read(cpu->PC));

        cpu->PC++;
        cycles = 2;
        break;
    case LDA_ZP:
    {
        load(&cpu->A, read(getZeroPageAddress()));

        cycles = 3;
        break;
    }
    case LDA_AB:
    {
        load(&cpu->A, read(getAbsoluteAddress()));

        cycles = 4;
        break;
    }

    // Store Instructions
    // Register X
    case STX_ZP:
    {
        write(getZeroPageAddress(), cpu->X);
        cycles = 3;
        break;
    }
    case STX_AB:
    {
        write(getAbsoluteAddress(), cpu->X);
        cycles = 4;
        break;
    }
    // Register Y
    case STY_ZP:
    {
        write(getZeroPageAddress(), cpu->Y);
        cycles = 3;
        break;
    }
    case STY_AB:
    {
        write(getAbsoluteAddress(), cpu->Y);

        cycles = 4;
        break;
    }
    // Register A
    case STA_ZP:
    {
        write(getZeroPageAddress(), cpu->A);

        cycles = 3;
        break;
    }
    case STA_AB:
    {
        write(getAbsoluteAddress(), cpu->A);

        cycles = 4;
        break;
    }

    // Branch Instructions
    case BPL:
    {
        cycles = branch(!cpu->NEG);
        break;
    }
    case BMI:
    {
        cycles = branch(cpu->NEG);
        break;
    }

    case BVC:
    {
        cycles = branch(!cpu->OVF);
        break;
    }
    case BVS:
    {
        cycles = branch(cpu->OVF);
        break;
    }

    case BCC:
    {
        cycles = branch(!cpu->CRY);
        break;
    }
    case BCS:
    {
        cycles = branch(cpu->CRY);
        break;
    }

    case BNE:
    {
        cycles = branch(!cpu->ZER);
        break;
    }

    case BEQ:
    {
        cycles = branch(cpu->ZER);
        break;
    }

    // Stack Instrcutions
    case PHA:
        push(cpu->A);
        cycles = 3;
        break;
    case PLA:
        cpu->A = pull();

        cpu->ZER = cpu->A == 0;
        cpu->NEG = cpu->A > 0x7F;

        cycles = 4;
        break;

    case PHP:
    {
        byte value = 0;
        value += (byte)(cpu->CRY ? 0x01 : 0);
        value += (byte)(cpu->ZER ? 0x02 : 0);
        value += (byte)(cpu->IND ? 0x04 : 0);
        value += (byte)(cpu->DEC ? 0x08 : 0);
        value += 0x10;
        value += 0x20;
        value += (byte)(cpu->OVF ? 0x40 : 0);
        value += (byte)(cpu->NEG ? 0x80 : 0);
        push(value);

        cycles = 3;
        break;
    }
    case PLP:
    {
        byte value = pull();

        cpu->CRY = (value & 0x01) != 0;
        cpu->ZER = (value & 0x02) != 0;
        cpu->IND = (value & 0x04) != 0;
        cpu->DEC = (value & 0x08) != 0;
        cpu->OVF = (value & 0x40) != 0;
        cpu->NEG = (value & 0x80) != 0;

        cycles = 3;
        break;
    }

    // Jump Instructions
    case JMP:
        byte low = read(cpu->PC);
        cpu->PC++;
        byte high = read(cpu->PC);

        cpu->PC = (addr)(high * 256 + low);
        cycles = 6;
        break;
    case JSR:
    {
        byte low = read(cpu->PC);
        cpu->PC++;
        byte high = read(cpu->PC);

        push((byte)cpu->PC / 256); // PC High Byte
        push((byte)cpu->PC);       // PC Low Byte

        cpu->PC = (addr)(high * 256 + low);

        cycles = 6;
        break;
    }
    case RTS:
    {
        byte low = pull();
        byte high = pull();

        cpu->PC = (addr)(high * 256 + low);
        cpu->PC++;

        cycles = 6;
        break;
    }

        // Increment / Decrement Instructinos

    case INX:
        cpu->X++;
        cycles = 2;
        break;
    case DEX:
        cpu->X--;
        break;

    case INY:
        cpu->Y++;
        cycles = 2;
        break;
    case DEY:
        cpu->Y--;
        cycles = 2;
        break;

    // Transfer Instructions

    case TAX:
        transfer(&cpu->A, &cpu->X, true);

        cycles = 2;
        break;
    case TXA:
        transfer(&cpu->X, &cpu->A, true);

        cycles = 2;
        break;
    case TAY:
        transfer(&cpu->A, &cpu->Y, true);

        cycles = 2;
        break;
    case TYA:
        transfer(&cpu->Y, &cpu->A, true);

        cycles = 2;
        break;
    case TXS:
        /*
        The flags of the 6502 register are not checked on
        SP operations.
        */
        transfer(&cpu->X, &cpu->SP, false);

        cycles = 2;
        break;
    case TSX:
        transfer(&cpu->SP, &cpu->X, true);

        cycles = 2;
        break;

    // Flags Instructions
    case SEC:
        cpu->CRY = 1;
        cycles = 2;
        break;
    case CLC:
        cpu->CRY = 0;
        cycles = 2;
        break;
    case CLV:
        cpu->OVF = 0;
        cycles = 2;
        break;
    case SEI:
        cpu->IND = 1;
        cycles = 2;
        break;
    case CLI:
        cpu->IND = 0;
        cycles = 2;
        break;
    case SED:
        cpu->DEC = 1;
        cycles = 2;
        break;
    case CLD:
        cpu->DEC = 0;
        cycles = 2;
        break;

    // Arithimetic Instructions
    // Arithimetic Shift Left
    case ASL_RA:
        asl(NULL, &cpu->A);
        cycles = 2;
        break;
    case ASL_AB:
    {
        addr addr = getAbsoluteAddress();
        byte value = read(addr);
        asl(addr, &value);

        cycles = 6;
        break;
    }
    case ASL_ZP:
    {
        addr addr = getZeroPageAddress();
        byte value = read(addr);
        asl(addr, &value);

        cycles = 5;
        break;
    }

    //Rotate Left
    case ROL_RA:{
        rol(NULL, &cpu->A);
        cycles = 2;
        break;
    }
    case ROL_AB:{
        addr addr = getAbsoluteAddress();
        byte value = read(addr);
        rol(addr, &value);

        cycles = 6;
        break;
    }
    case ROL_ZP: {
        addr addr = getZeroAddress();
        byte value = read(addr);
        rol(addr, &value);

        cycles = 5;
        break;
    }
    default:
        printf("Unknown instruction: %x.\n", opcode);
        // exit(1);
        break;
    }
}

byte read(addr address)
{
    // RAM
    if ((address >= RAM_INIT_ADDR) && (address < RAM_FINAL_ADDR))
    {
        printf("RAM addr: %x\n", (address % RAM_SIZE));
        return ram[address];
    }

    // RAM Mirrors
    if ((address >= RAM_MIRROR_INIT_ADDR) && (address < RAM_MIRROR_FINAL_ADDR))
    {
        printf("Effective RAM addr: %x\n", (address % RAM_SIZE));
        return ram[address % RAM_SIZE];
    }

    // ROM
    if ((address >= ROM_INIT_ADDR) && (address < ROM_FINAL_ADDR))
    {
        printf("Effective ROM addr: %x\n", (address - ROM_SIZE));
        return rom[address - ROM_SIZE];
    }

    return 0x00;
}

void write(addr address, byte value)
{
    // RAM
    if ((address >= RAM_INIT_ADDR) && (address < RAM_FINAL_ADDR))
    {
        ram[address] = value;
    }

    // RAM Mirrors
    if ((address >= RAM_MIRROR_INIT_ADDR) && (address < RAM_MIRROR_FINAL_ADDR))
    {
        ram[address % RAM_SIZE] = value;
    }
}

void reset()
{
    cpu->IND = 1;
    cpu->SP = 0xFD;

    byte *HeaderedRom = readFileAsBytes();

    if (HeaderedRom == NULL)
    {
        exit(1);
    }
    byte *Header = (byte *)malloc(0x10 * sizeof(byte));

    copyByteArray(HeaderedRom, rom, HEADERED_ROM_SIZE, 0x10);
    // copyByteArray(HeaderedRom,Header, 0x10, 0);

    byte PCL = read(0xFFFC);
    byte PCH = read(0xFFFD);

    cpu->PC = (addr)((PCH * 0x100) + PCL);
}

addr getAbsoluteAddress()
{
    addr absAddr = read(cpu->PC);
    cpu->PC++;
    absAddr = (addr)(read(cpu->PC) << 8 | absAddr);
    return absAddr;
}

addr getZeroPageAddress()
{
    byte temp = read(cpu->PC);
    cpu->PC++;
    return temp;
}

void push(byte value)
{
    addr addr = (STACK_START_ADDR + cpu->SP);
    write(addr, value);
    cpu->SP--;
}

byte pull()
{
    cpu->SP++;
    addr addr = (STACK_START_ADDR + cpu->SP);
    byte value = read(addr);
    return value;
}

byte *readFileAsBytes()
{
    FILE *romFile = fopen(romFileName, "rb");
    if (!romFile)
    {
        perror("Error opening file");
        return NULL;
    }

    byte *buffer = calloc(HEADERED_ROM_SIZE, sizeof(byte));

    if (!buffer)
    {
        perror("Error alocating memory");
        fclose(romFile);
        return NULL;
    }

    romFileLength = fread(buffer, 1, HEADERED_ROM_SIZE, romFile);

    fclose(romFile);
    return buffer;
}

void copyByteArray(byte *inArray, byte *outArray, size_t size, size_t offset)
{
    for (size_t i = offset; i < (size); i++)
    {
        outArray[i - offset] = inArray[i];
    }
}

void printMemory(byte *memory, size_t start, size_t end)
{
    for (size_t i = start; i < end; i++)
    {
        if (i % 8 == 0)
        {
            printf("\n");
            printf("[%04X]:", i);
        }
        printf(" %02X ", memory[i]);
    }
}

int convertToSignedVal(byte num)
{
    int signedVal = num > 0x7F ? num - 256 : num;
    return signedVal;
}

// Instructions functions

void load(byte *reg, byte value)
{
    *reg = value;

    cpu->ZER = *reg == 0;
    cpu->NEG = *reg > 0x7F;
}

int branch(bool flag){
    int cycles = 0;
    byte temp = read(cpu->PC);
    cpu->PC++;

    if (flag){
        int signedVal = convertToSignedVal(temp);
        cpu->PC += signedVal;
        cycles = 3;
    }
    else{
        cycles = 2;
    }
    return cycles;
}

void transfer(byte *sourceRegister, byte *destinationRegister, bool changeFlags){
    *destinationRegister = *sourceRegister;

    if(changeFlags) {
        cpu->ZER = *destinationRegister == 0;
        cpu->NEG = *destinationRegister > 0x7F;
    }
}

void asl(addr addr, byte *value)
{
    printf("Executing: ASL on value: %x\n", *value);
    cpu->CRY = *value > 0x7F;

    *value <<= 1;

    cpu->ZER = *value == 0;
    cpu->NEG = *value > 0x7F;

    if (addr != NULL)
    {
        write(addr, *value);
    }
}

void rol(addr addr, byte *value){
    printf("Executing: ROL on value: %x\n", *value);

    bool newCarry = value > 0x7F;

    *value <<= 1;
    if(cpu->CRY){
        *value |= 0b00000001;
    }

    cpu->ZER = *value == 0;
    cpu->NEG = *value > 0x7F;
    cpu->CRY = newCarry;

    if(addr != NULL){
        write(addr, *value);  
    }
}