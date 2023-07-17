#include "vm.h"

State state;

int main(int argc, char* argv[]) {

    state.memory = malloc(MEM_SIZE * sizeof(Word));

    load(argv[1]);

    interpret();

    dump("mem.dump");

    free(state.memory);

    return 0;
}
