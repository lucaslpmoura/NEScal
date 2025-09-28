#include "CPU.h"
#include "instructions.h"

#include "RAM.h"
#include "ROM.h"
#include "MemMapping.h"

#include <stdio.h>
#include <stdlib.h>

#define START -1
#define STEPPING_MODE 1
#define PRINT_STACK 0
#define PRINT_RAM 1

char *romFileName = "./tst/1_Example.nes";

FILE *romFile;
long romFileLength;
uint8_t *buffer;

void run();
void reset();

byte *readFileAsBytes();
void copyByteArray(byte *inArray, byte *outArray, size_t size, size_t offset);

int main(int argv, char **argc)
{
    if (argv == 2)
    {
        romFileName = argc[1];
    }

    initRAM();
    initROM();
    initCPU(&ram, &rom);


    printf("Executing: %s\n", romFileName);
    reset();
    run();
    return 0;
}


void run()
{
    if(cpu == NULL){
        perror("CPU not initialized.");
        exit(1);
    }

    if(STEPPING_MODE == 1) printf("Press any key to run.\n");

    printTraceLog(START);
    while (!cpu->HLT)
    {
        if (STEPPING_MODE == 1)
        {
            getchar();
        }

        emulate();
        
        if(PRINT_STACK == 1) {
            printf("\nSTACK: \n");
            printStack();
        }    
    }
    if(PRINT_RAM == 1){
        printf("\n");
        printRAM();
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

    //byte *Header = (byte *)malloc(0x10 * sizeof(byte));
    copyByteArray(HeaderedRom, cpu->rom, HEADERED_ROM_SIZE, 0x10);
    // copyByteArray(HeaderedRom,Header, 0x10, 0);
    
    byte PCL = read(0xFFFC);
    byte PCH = read(0xFFFD);

    cpu->PC = (addr)((PCH * 0x100) + PCL);
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





