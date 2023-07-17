#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#define MEM_SIZE 65536

typedef uint16_t Word;

typedef struct {
    Word ip;
    Word r1;
    Word r2;
    Word r3;
    Word r4;
    Word r5;
    Word r6;
    Word r7;
} Registers;

typedef struct {
    Word* memory;
    Registers registers;
} State;

static State state;

// This is janky -- change later
Word* selectRegister(Word x) {
    switch (x) {
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

int main() {

    state.memory = malloc(MEM_SIZE * sizeof(Word));

    int prog_fd = open("program.bin", O_RDONLY);
    read(prog_fd, &state.registers, sizeof(Registers));
    read(prog_fd, state.memory, MEM_SIZE);

    Word opcode, operand1, operand2;

    for (; state.registers.ip != MEM_SIZE - 1; state.registers.ip += 3) {

        opcode   = state.memory[state.registers.ip];
        operand1 = state.memory[state.registers.ip + 1];
        operand2 = state.memory[state.registers.ip + 2];

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
            case 8: // JNZ
        }
    }

    int dump_fd = open("mem.dump", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    write(dump_fd, &state.registers, sizeof(Registers));
    write(dump_fd, state.memory, MEM_SIZE * sizeof(Word));

    free(state.memory);

    return 0;
}
