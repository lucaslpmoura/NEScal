#include "ROM.h"

ROM InitROM(){
    ROM rom = (byte*) malloc(sizeof(byte) * ROM_SIZE);
    for(uint32_t i = 0; i < ROM_SIZE; i++){
        rom[i] = 0x00 ;
    }

    return rom;
}

void PrintROM(ROM rom){

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