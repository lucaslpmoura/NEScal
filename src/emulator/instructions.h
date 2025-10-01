#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "types.h"


#define HTL 0x02 // Halt
#define NOP 0xEA // No Operation



#define BRK_NMI 0x00 // Break and Non-Maskable Interrupt
#define RTI 0x40 // Return from Interrupt


#define LDX_IM 0xA2 // Load X, Immediate
#define LDX_ZP 0xA6 // Load X, Zero Page
#define LDX_ZY 0xB6 // Load X, Zero Page with Y offset
#define LDX_AB 0xAE // Load X, Absolute
#define LDX_AY 0xBE // Load X, Absolute with Y offset

#define LDY_IM 0xA0 // Load Y, Immediate
#define LDY_ZP 0xA4 // Load Y, Zero Page
#define LDY_ZX 0xB4 // Load Y, Zero Page with X offset
#define LDY_AB 0xAC // Load Y, Absolute
#define LDY_AX 0xBC // Load Y, Absolute with X offset

#define LDA_IM 0xA9 // Load A, Immediate
#define LDA_ZP 0xA5 // Load A, Zero Page
#define LDA_ZX 0xB5 // Loda A, Zero Page with X offset
#define LDA_AB 0xAD // Load A, Absolute
#define LDA_AY 0xB9 // Load A, Absolute with Y offset
#define LDA_AX 0xBD // Load A, Absolute with X offset
#define LDA_IY 0xA1 // Load A, Indirect with Y offset
#define LDA_IX 0xB1 // Load A, Indirect with X offset

#define STX_ZP 0x86 // Store X, Zero Page
#define STX_ZY 0x96 // Store X, Zero Page with Y offset
#define STX_AB 0x8E // Store X, Absolute

#define STY_ZP 0x84 // Store Y, Zero Page
#define STY_ZX 0x94 // Store Y, Zero Page with X offset
#define STY_AB 0x8C // Store Y, Absolute

#define STA_ZP 0x85 // Store A, Zero Page
#define STA_ZX 0x95 // Store A, Zero Page with X Offset
#define STA_AB 0x8D // Store A, Absolute
#define STA_AY 0x99 // Store A, Absolute with Y offset
#define STA_AX 0x9D // Store A, Absolute with X Offset
#define STA_IY 0x91 // Store A, Indirect with Y Offset
#define STA_IX 0x81 // Store A, Indirect with X Offset



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



#define JMP_AB 0x4C // Jump, Absolute
#define JMP_ID 0x6C // Jump, Indirect
#define JSR 0x20 // Jump to Subroutine
#define RTS 0x60 // Return From Subroutine



#define INC_AB 0xEE // Increment, Absolute
#define INC_AX 0xFE // Increment, Absolute with X offset
#define INC_ZP 0xE6 // Increment, Zero Page
#define INC_ZX 0xF6 // Increment, Zero Page with X offset

#define DEC_AB 0xCE // Decrement, Absolute
#define DEC_AX 0xDE // Decrement, Absolute with X offset
#define DEC_ZP 0xC6 // Decrement, Zero Page
#define DEC_ZX 0xD6 // Decrement, Zero Page with X offset

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
#define ASL_AX 0x1E // Arithimetic Shift Left, Absolute with X offset
#define ASL_ZP 0x06 // Arithimetic Shift Left, Zero Page
#define ASL_ZX 0x16 // Arithimetic Shift Left, Zero Page with X offset

#define LSR_RA 0x4A // Logical Shift Right, A Register
#define LSR_AB 0x4E // Logical Shift Right, Absolute
#define LSR_AX 0x5E // Logical Shift Right, Absolute with X offset
#define LSR_ZP 0x46 // Logical Shift Right, Zero Page
#define LSR_ZX 0x56 // Logical Shift Right, Zero Page with X offset

#define ROL_RA 0x2A // Rotate Left, A Register
#define ROL_AB 0x2E // Rotate Left, Absolute
#define ROL_AX 0x3E // Rotate Left, Absolute with X offset
#define ROL_ZP 0x26 // Rotate Left, Zero Page
#define ROL_ZX 0x36 // Rotate Left, Zero Page with X offset


#define ROR_RA 0x6A // Rotate Right, A Register
#define ROR_AB 0x6E // Rotate Right, Absolute
#define ROR_AX 0x7E // Rotate Right, Absolute with X offset
#define ROR_ZP 0x66 // Rotate Right, Zero Page
#define ROR_ZX 0x76 // Rotate Right, Zero Page with X offset


#define ADC_IM 0x69 // Add with carry with A Register, Immediate
#define ADC_AB 0x6D // Add with carry with A Register, Absolute
#define ADC_AY 0x79 // Add with carry with A register, Absolute with Y offset
#define ADC_AX 0x7D // Add with carry with A register, Absolute with X offset
#define ADC_ZP 0x65 // Add with carry with A Register, Zero Page
#define ADC_ZX 0x75 // Add with carry with A Register, Zero Page with X offset
#define ADC_IY 0x71 // Add with carry with A register, Indirect with Y offset
#define ADC_IX 0x61 // Add with carry with A register, Indirect with X offset

#define SBC_IM 0xE9 // Subtract with carry with A Register, Immediate
#define SBC_AB 0xED // Subtract with carry with A Register, Absolute
#define SBC_AX 0xFD // Subtract with carry with A Register, Absolute with X offset
#define SBC_AY 0xF9 // Subtract with carry with A Register, Absolute with Y offset
#define SBC_ZP 0xE5 // Subtract with carry with A Register, Zero Page
#define SBC_ZX 0xF5 // Subtract with carry with A Register, Zero Page with X offset
#define SBC_IX 0xE1 // Subtract with carry with A Register, Indirect with X offset
#define SBC_IY 0xF1 // Subtract with carry with A Register, Indirect with Y offset


#define ORA_IM 0x09 // Bitwise OR with A Register, Immediate
#define ORA_AB 0x0D // Bitwise OR with A Register, Absolute
#define ORA_AX 0x1D // Bitwise OR with A Register, Absolute with X offset
#define ORA_AY 0x19 // Bitwise OR with A Register, Absolute with Y offset
#define ORA_ZP 0x05 // Bitwise OR with A Register, Zero Page
#define ORA_ZX 0x15 // Bitwise OR with A Register, Zero Page wit X offset
#define ORA_IX 0x01 // Bitwise OR with A Register, Indirect wit X offset
#define ORA_IY 0x11 // Bitwise OR with A Register, Indirect wit Y offset

#define AND_IM 0x29 // Bitwise AND with A Register, Immediate
#define AND_AB 0x2D // Bitwise AND with A Register, Absolute
#define AND_AX 0x3D // Bitwise AND with A Register, Absolute with X offset
#define AND_AY 0x39 // Bitwise AND with A Register, Absolute with Y offset
#define AND_ZP 0x25 // Bitwise AND with A Register, Zero Page
#define AND_ZX 0x35 // Bitwise AND with A Register, Zero Page with X offset
#define AND_IX 0x21 // Bitwise AND with A Register, Indirect with X offset
#define AND_IY 0x31 // Bitwise AND with A Register, Indirect with Y offset

#define EOR_IM 0x49 // Bitwise XOR with A Register, Immediate
#define EOR_AB 0x4D // Bitwise XOR with A Register, Absolute
#define EOR_AX 0x5D // Bitwise XOR with A Register, Absolute with X offset
#define EOR_AY 0x59 // Bitwise XOR with A Register, Absolute with Y offset
#define EOR_ZP 0x45 // Bitwise XOR with A Register, Zero Page
#define EOR_ZX 0x55 // Bitwise XOR with A Register, Zero Page with X offset
#define EOR_IX 0x41 // Bitwise XOR with A Register, Indirect with X offset
#define EOR_IY 0x51 // Bitwise XOR with A Register, Indirect with Y offset


#define CMP_IM 0xC9 // Compare with A Register, Immediate
#define CMP_AB 0xCD // Compare with A Register, Absolute
#define CMP_AX 0xDD // Compare with A Register, Absolute with X offset
#define CMP_AY 0xD9 // Compare with A Register, Absolute with Y offset
#define CMP_ZP 0xC5 // Compare with A Register, Zero Page
#define CMP_ZX 0xD5 // Compare with A Register, Zero Page with X offset
#define CMP_IX 0xC1 // Compare with A register, Indirect with X offset
#define CMP_IY 0xD1 // Compare with A register, Indirect with Y offset

#define CPX_IM 0xE0 // Compare with X Register, Immediate
#define CPX_AB 0xEC // Compare with X Register, Absolute
#define CPX_ZP 0xE4 // Compare with X Register, Zero Page

#define CPY_IM 0xC0 // Compare with Y Register, Immediate
#define CPY_AB 0xCC // Compare with Y Register, Absolute
#define CPY_ZP 0xC4 // Compare with Y Register, Zero Page

#define BIT_AB 0x2c // Bit Compare with A Register, Absolute
#define BIT_ZP 0x24 // Bit Compare with A Register, Zero Page

const char *getOpcodeMnemonic(byte opcode);

#endif