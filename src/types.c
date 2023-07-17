#include "types.h"

// This is janky -- change later
Word* selectRegister(Word r) {
    switch (r) {
        case 0: return &state.registers.ip;
        case 1: return &state.registers.r1;
        case 2: return &state.registers.r2;
        case 3: return &state.registers.r3;
        case 4: return &state.registers.r4;
        case 5: return &state.registers.r5;
        case 6: return &state.registers.r6;
        case 7: return &state.registers.r7;
        default: return NULL;
    }
}
