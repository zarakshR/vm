#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "vm.h"
int main(int argc, char* argv[]) {

    state.memory = malloc(MEM_SIZE * sizeof(Word));

    load(argv[1]);

    Word opcode, operand1, operand2;

    for (; state.registers.ip != MEM_SIZE - 1; state.registers.ip += 3) {

        opcode   = state.memory[state.registers.ip];
        operand1 = state.memory[state.registers.ip + 1];
        operand2 = state.memory[state.registers.ip + 2];

#ifdef DEBUG
        printf("IP:%04x OP:%04x O1:%04x O2:%04x\n", state.registers.ip, opcode,
               operand1, operand2);
#endif

        switch (opcode) {
            case 0: // NOOP
                break;
            case 1: // LOAD IMMEDIATE
                *selectRegister(operand1) = operand2;
                break;
            case 2: // LOAD ADDRESS
                *selectRegister(operand1) =
                    state.memory[*selectRegister(operand2)];
                break;
            case 3: // STORE
                state.memory[*selectRegister(operand1)] =
                    *selectRegister(operand2);
                break;
            case 4: // ADD
                *selectRegister(operand1) =
                    *selectRegister(operand1) + *selectRegister(operand2);
                break;
            case 5: // SUB
                *selectRegister(operand1) =
                    *selectRegister(operand1) - *selectRegister(operand2);
                break;
            case 6: // JMP
                state.registers.ip = *selectRegister(operand1) - 3;
                continue;
            case 7: // JZ
                if (*selectRegister(operand1) == 0) {
                    state.registers.ip = *selectRegister(operand2) - 3;
                    continue;
                }
                break;
            case 8: // JNZ
                if (*selectRegister(operand1) != 0) {
                    state.registers.ip = *selectRegister(operand2) - 3;
                    continue;
                }
                break;
        }
    }

    dump("mem.dump");

    free(state.memory);

    return 0;
}
