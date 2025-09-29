#include "ROM.h"

ROM rom = NULL;

void initROM(){
    rom = (ROM) malloc(sizeof(ROM) * ROM_SIZE);
    for(uint32_t i = 0; i < ROM_SIZE; i++){
        rom[i] = 0x00 ;
    }
}

void printROM(ROM rom){

    for(uint16_t i = 0; i < ROM_SIZE; i++){
        printf("[%04x]:", i);
        for(char j = 0; j < 8; j++){
            printf(" %02x ", rom[i]);
            i++;
        }
        i--;
        printf("\n");
       
    }
}