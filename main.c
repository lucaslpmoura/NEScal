#include "CPU.h"
#include "instructions.h"

#include "RAM.h"
#include "ROM.h"
#include "MemMapping.h"

#include "stdio.h"
#include "stdlib.h"

char *romFileName = "./tst/1_ReadWrite.nes";

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

void push(byte value);
byte pull();

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
        printf("Press any key to run.");
        getchar();
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
    case LDX_IM:
        cpu->X = read(cpu->PC);

        cpu->ZER = cpu->X == 0;
        cpu->NEG = cpu->X > 127;

        cpu->PC++;
        cycles = 2;
        break;
    case LDY_IM:
        cpu->Y = read(cpu->PC);

        cpu->ZER = cpu->Y == 0;
        cpu->NEG = cpu->Y > 127;

        cpu->PC++;
        cycles = 2;
        break;

    // Load A
    case LDA_IM:
        cpu->A = read(cpu->PC);

        cpu->ZER = cpu->A == 0;
        cpu->NEG = cpu->A > 127;

        cpu->PC++;
        cycles = 2;
        break;
    case LDA_ZP:
    {
        byte temp = read(cpu->PC);
        cpu->PC++;
        cpu->A = read(temp);

        cpu->ZER = cpu->A == 0;
        cpu->NEG = cpu->A > 127;

        cycles = 3;
        break;
    }
    case LDA_AB:
    {
        byte temp_low = read(cpu->PC);
        cpu->PC++;
        byte temp_high = read(cpu->PC);
        cpu->PC++;
        addr addr = (temp_high * 256 + temp_low);
        cpu->A = read(addr);

        cpu->ZER = cpu->A == 0;
        cpu->NEG = cpu->A > 127;

        cycles = 4;
        break;
    }

    // Store Instructions
    case STX_ZP:
    {
        byte temp = read(cpu->PC);
        cpu->PC++;
        write(temp, cpu->X);
        cycles = 3;
        break;
    }
    case STX_AB:
    {
        byte temp_low = read(cpu->PC);
        cpu->PC++;
        byte temp_high = read(cpu->PC);
        cpu->PC++;
        addr addr = (temp_high * 256 + temp_low);
        write(addr, cpu->X);
        cycles = 4;
        break;
    }
    case STY_ZP:
    {
        byte temp = read(cpu->PC);
        cpu->PC++;
        write(temp, cpu->Y);
        cycles = 3;
        break;
    }
    case STY_AB:
    {
        byte temp_low = read(cpu->PC);
        cpu->PC++;
        byte temp_high = read(cpu->PC);
        cpu->PC++;
        addr addr = (temp_high * 256 + temp_low);
        write(addr, cpu->Y);
        cycles = 4;
        break;
    }
    case STA_ZP:
    {
        byte temp = read(cpu->PC);
        cpu->PC++;
        write(temp, cpu->A);
        cycles = 3;
        break;
    }
    case STA_AB:
    {
        byte temp_low = read(cpu->PC);
        cpu->PC++;
        byte temp_high = read(cpu->PC);
        cpu->PC++;
        addr addr = (temp_high * 256 + temp_low);
        write(addr, cpu->A);
        cycles = 4;
        break;
    }

    // Branch Instructions
    case BPL: {
        byte temp = read(cpu->PC);
        cpu->PC++;

        if (!cpu->NEG)
        {
            int signedVal = convertToSignedVal(temp);
            cpu->PC += signedVal;
            cycles = 3;
        }
        else
        {
            cycles = 2;
        }
        break;
    } 
    case BMI: {
        byte temp = read(cpu->PC);
        cpu->PC++;

        if (cpu->NEG){
            int signedVal = convertToSignedVal(temp);
            cpu->PC += signedVal;
            cycles = 3;
        }
        else{
            cycles = 2;
        }
        break;
    }
        

    case BVC: {
        byte temp = read(cpu->PC);
        cpu->PC++;

        if (!cpu->OVF){
            int signedVal = convertToSignedVal(temp);
            cpu->PC += signedVal;
            cycles = 3;
        }
        else{
            cycles = 2;
        }
        break;
    }
    case BVS: {
        byte temp = read(cpu->PC);
        cpu->PC++;

        if (cpu->OVF){
            int signedVal = convertToSignedVal(temp);
            cpu->PC += signedVal;
            cycles = 3;
        }
        else{
            cycles = 2;
        }
        break;
    }
        
    
    case BCC: {
        byte temp = read(cpu->PC);
        cpu->PC++;

        if (!cpu->CRY){
            int signedVal = convertToSignedVal(temp);
            cpu->PC += signedVal;
            cycles = 3;
        }
        else{
            cycles = 2;
        }
        break;
    }
    case BCS: {
        byte temp = read(cpu->PC);
        cpu->PC++;

        if (cpu->CRY){
            int signedVal = convertToSignedVal(temp);
            cpu->PC += signedVal;
            cycles = 3;
        }
        else{
            cycles = 2;
        }
        break;
    }
        


    case BNE: {
        byte temp = read(cpu->PC);
        cpu->PC++;

        if (!cpu->ZER){
            int signedVal = convertToSignedVal(temp);
            cpu->PC += signedVal;
            cycles = 3;
        }
        else{
            cycles = 2;
        }
        break;
    }
        
    case BEQ: {
        byte temp = read(cpu->PC);
        cpu->PC++;

        if (cpu->ZER){
            int signedVal = convertToSignedVal(temp);
            cpu->PC += signedVal;
            cycles = 3;
        }
        else{
            cycles = 2;
        }
        break;
    }
    
    //Stack Instrcutions
    case PHA:
        push(cpu->A);
        cycles = 3;
        break;
    case PLA:
        cpu->A = pull();

        cpu->ZER = cpu->A == 0;
        cpu->NEG = cpu->A > 127;

        cycles = 4;
        break;

    // Subroutine Instructions
    case JSR: {
        byte low = read(cpu->PC);
        cpu->PC++;
        byte high = read(cpu->PC);

        push((byte) cpu->PC/256); // PC High Byte
        push((byte) cpu->PC); // PC Low Byte
        
        cpu->PC = (addr) (high*256 + low);

        cycles = 6;
        break;
    }
    case RTS: {
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
        cpu->X = cpu->A;

        cpu->ZER = cpu->X == 0;
        cpu->NEG = cpu->X > 127;

        cycles = 2;
        break;
    case TXA:
        cpu->A = cpu->X;

        cpu->ZER = cpu->A == 0;
        cpu->NEG = cpu->A > 127;

        cycles = 2;
        break;
    case TAY:
        cpu->Y = cpu->A;

        cpu->ZER = cpu->Y == 0;
        cpu->NEG = cpu->Y > 127;

        cycles = 2;
        break;
    case TYA:
        cpu->A = cpu->Y;

        cpu->ZER = cpu->A == 0;
        cpu->NEG = cpu->A > 127;

        cycles = 2;
        break;
    case TXS:
        cpu->SP = cpu->X;

        cycles = 2;
        break;
    case TSX:
        cpu->X = cpu->SP;


        cpu->ZER = cpu->X == 0;
        cpu->NEG = cpu->X > 127;

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

void push(byte value){
    addr addr = (STACK_START_ADDR + cpu->SP);
    write(addr, value);
    cpu->SP--;
}

byte pull(){
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
    int signedVal = num > 127 ? num - 256 : num;
    return signedVal;
}