#ifndef VM__TYPES_H
#define VM__TYPES_H

#include <stdint.h>
#include <stddef.h>

#define BYTES_TO_WORDS(x) (x / sizeof(Word))
#define WORDS_TO_BYTES(x) (x * sizeof(Word))

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

extern State state;

Word* selectRegister(Word);

#endif
