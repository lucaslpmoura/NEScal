#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#endif


#define HTL 0x02 // Halt
#define NOP 0xEA // No Operation

#define LDX_IM 0xA2 // Load X, Immediate
#define LDY_IM 0xA0 // Load Y, Immediate

#define LDA_IM 0xA9 // Load A, Immediate
#define LDA_ZP 0xA5 // Load A, Zero Page
#define LDA_AB 0xAD // Load A, Absolute

#define STX_ZP 0x86 // Store X, Zero Page
#define STX_AB 0x8E // Store X, Absolute

#define STY_ZP 0x84 // Store Y, Zero Page
#define STY_AB 0x8C // Store Y, Absolute

#define STA_ZP 0x85 // Store A, Zero Page
#define STA_AB 0x8D // Store A, Absolute


#define BPL 0x10 // Branch on Plus  (!NEG / >= 0)
#define BMI 0x30 // Branch on Minues (NEG/ < 0)
#define BVC 0x50 // Branch on Overflow Clear (!OVF)
#define BVS 0x70 // Branch on Overflow Set (OVF)
#define BCC 0x90 // Branch on Carry Clear (!CRY)
#define BCS 0xB0 // Branch on Carry Set (CRY)
#define BNE 0xD0 // Branch Not Equal (!ZER / != 0)
#define BEQ 0xF0 // Branch on Equal (ZER / == 0)


#define PHA 0x48 // Push A
#define PLA 0x68 // Pull A

#define PHP 0x08 // Push Processor Flags
#define PLP 0x28 // Pull Processor Flags


#define JSR 0x20 // Jump to Subroutine
#define RTS 0x60 // Return From Subroutine


#define INX 0xE8 // Increment X
#define DEX 0xCA // Decrement X
#define INY 0xC8 // Increment Y
#define DEY 0x88 // Decrement Y


#define TAX 0xAA // Transfer A to X
#define TXA 0x8A // Transfer X to A
#define TAY 0xA8 // Transfer A to Y
#define TYA 0x98 // Trasnfer Y to A
#define TXS 0x9A // Transfer X to SP
#define TSX 0xBA // Transfer SP To X


#define SEC 0x38 // Set Carry Flag
#define CLC 0x18 // Clear Carry Flag
#define CLV 0xB8 // Clear Overflow Flag
#define SEI 0x78 // Set Interrupt Disable Flag
#define CLI 0x58 // Clear Interrupt Disable Flag
#define SED 0xF8 // Set Decimal Flag
#define CLD 0xD8 // Clear Decimal Flag

#define ASL_RA 0x0A // Arithimetic Shift Left, A Register
#define ASL_AB 0x0E // Arithimetic Shift Left, Absolute
