#include "instructions.h"

#include <stdio.h>
#include <stdint.h>

typedef uint8_t byte;

const char* getOpcodeMnemonic(byte opcode) {
    switch (opcode) {
        case 0x02: return "HTL";
        case 0xEA: return "NOP";

        case 0x00: return "BRK";
        case 0x40: return "RTI";

        case 0xA2: return "LDX_IM";
        case 0xA6: return "LDX_ZP";
        case LDX_ZY: return "LDX_ZY";
        case 0xAE: return "LDX_AB";
        case LDX_AY: return "LDX_AY";

        case 0xA0: return "LDY_IM";
        case 0xA4: return "LDY_ZP";
        case LDY_ZX: return "LDY_ZX";
        case 0xAC: return "LDY_AB";
        case LDY_AX: return "LDY_AX";

        case 0xA9: return "LDA_IM";
        case 0xA5: return "LDA_ZP";
        case LDA_ZX: return "LDA_ZX";
        case 0xAD: return "LDA_AB";
        case 0xB9: return "LDA_AY";
        case 0xBD: return "LDA_AX";
        case LDA_IY: return "LDA_IY";
        case LDA_IX: return "LDA_IX";

        case 0x86: return "STX_ZP";
        case STX_ZY: return "STX_ZY";
        case 0x8E: return "STX_AB";

        case 0x84: return "STY_ZP";
        case STY_ZX: return "STY_ZX";
        case 0x8C: return "STY_AB";

        case STA_ZP: return "STA_ZP";
        case STA_ZX: return "STA_ZX";
        case STA_AB: return "STA_AB";
        case STA_AY: return "STA_AY";
        case STA_AX: return "STA_AX";
        case STA_IY: return "STA_IY";
        case STA_IX: return "STA_IX";

        case 0x10: return "BPL";
        case 0x30: return "BMI";
        case 0x50: return "BVC";
        case 0x70: return "BVS";
        case 0x90: return "BCC";
        case 0xB0: return "BCS";
        case 0xD0: return "BNE";
        case 0xF0: return "BEQ";

        case 0x48: return "PHA";
        case 0x68: return "PLA";
        case 0x08: return "PHP";
        case 0x28: return "PLP";

        case 0x4C: return "JMP";
        case JMP_ID: return "JMP_ID";
        case 0x20: return "JSR";
        case 0x60: return "RTS";

        case 0xEE: return "INC_AB";
        case INC_AX: return "INC_AX";
        case 0xE6: return "INC_ZP";
        case INC_ZX: return "INC_ZX";

        case 0xCE: return "DEC_AB";
        case DEC_AX: return "DEC_AX";
        case 0xC6: return "DEC_ZP";
        case DEC_ZX: return "DEC_ZX";

        case 0xE8: return "INX";
        case 0xCA: return "DEX";
        case 0xC8: return "INY";
        case 0x88: return "DEY";

        case 0xAA: return "TAX";
        case 0x8A: return "TXA";
        case 0xA8: return "TAY";
        case 0x98: return "TYA";
        case 0x9A: return "TXS";
        case 0xBA: return "TSX";

        case 0x38: return "SEC";
        case 0x18: return "CLC";
        case 0xB8: return "CLV";
        case 0x78: return "SEI";
        case 0x58: return "CLI";
        case 0xF8: return "SED";
        case 0xD8: return "CLD";

        case 0x0A: return "ASL_RA";
        case 0x0E: return "ASL_AB";
        case ASL_AX: return "ASL_AX";
        case 0x06: return "ASL_ZP";
        case ASL_ZX: return "ASL_ZX";

        case 0x4A: return "LSR_RA";
        case 0x4E: return "LSR_AB";
        case LSR_AX: return "LSR_AX";
        case 0x46: return "LSR_ZP";
        case LSR_ZX: return "LSR_ZX";

        case 0x2A: return "ROL_RA";
        case 0x2E: return "ROL_AB";
        case ROL_AX: return "ROL_AX";
        case 0x26: return "ROL_ZP";
        case ROL_ZX: return "ROL_ZX";
        
        case 0x6A: return "ROR_RA";
        case 0x6E: return "ROR_AB";
        case ROR_AX: return "ROR_AX";
        case 0x66: return "ROR_ZP";
        case ROR_ZX: return "ROR_ZX";

        case 0x69: return "ADC_IM";
        case 0x6D: return "ADC_AB";
        case ADC_AX: return "ADC_AX";
        case ADC_AY: return "ADC_AY";
        case 0x65: return "ADC_ZP";
        case ADC_ZX: return "ADC_ZX";
        case ADC_IX: return "ADC_IX";
        case ADC_IY: return "ADC_IY";

        case 0xE9: return "SBC_IM";
        case 0xED: return "SBC_AB";
        case SBC_AX: return "SBC_AX";
        case SBC_AY: return "SBC_AY";
        case 0xE5: return "SBC_ZP";
        case SBC_ZX: return "SBC_ZX";
        case SBC_IX: return "SBC_IX";
        case SBC_IY: return "SBC_IY";

        case 0x09: return "ORA_IM";
        case 0x0D: return "ORA_AB";
        case ORA_AX: return "ORA_AX";
        case ORA_AY: return "ORA_AY";
        case 0x05: return "ORA_ZP";
        case ORA_ZX: return "ORA_ZX";
        case ORA_IX: return "ORA_IX";
        case ORA_IY: return "ORA_IY";

        case 0x29: return "AND_IM";
        case 0x2D: return "AND_AB";
        case AND_AY: return "AND_AY";
        case AND_AX: return "AND_AX";
        case 0x25: return "AND_ZP";
        case AND_ZX: return "AND_ZX";
        case AND_IX: return "AND_IX";
        case AND_IY: return "AND_IY";

        case 0x49: return "EOR_IM";
        case 0x4D: return "EOR_AB";
        case EOR_AX: return "EOR_AX";
        case EOR_AY: return "EOR_AY";
        case 0x45: return "EOR_ZP";
        case EOR_ZX: return "EOR_ZX";
        case EOR_IX: return "EOR_IX";
        case EOR_IY: return "EOR_IY";

        case 0xC9: return "CMP_IM";
        case 0xCD: return "CMP_AB";
        case CMP_AX: return "CMP_AX";
        case CMP_AY: return "CMP_AY";
        case 0xC5: return "CMP_ZP";
        case CMP_ZX: return "CMP_ZX";
        case CMP_IX: return "CMP_IX";
        case CMP_IY: return "CMP_IY";

        case 0xE0: return "CPX_IM";
        case 0xEC: return "CPX_AB";
        case 0xE4: return "CPX_ZP";

        case 0xC0: return "CPY_IM";
        case 0xCC: return "CPY_AB";
        case 0xC4: return "CPY_ZP";

        case 0x2C: return "BIT_AB";
        case 0x24: return "BIT_ZP";

        default: return "UNKNOWN";
    }
}
