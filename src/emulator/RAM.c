#include "RAM.h"

RAM ram = NULL;

void initRAM()
{
    ram = (RAM)malloc(sizeof(RAM) * RAM_SIZE);
    for (uint16_t i = 0; i < RAM_SIZE; i++)
    {
        ram[i] = 0x00;
    }
}

void printRAM()
{
    printMemory(ram, RAM_SIZE);
}


void printStack()
{
    for (uint16_t i = STACK_START_ADDR; i < STACK_START_ADDR + STACK_SIZE; i++)
    {
        printf("[%04x]:", i);
        for (char j = 0; j < 8; j++)
        {
            printf(" %02x ", ram[i]);
            i++;
        }
        i--;
        printf("\n");
    }
}