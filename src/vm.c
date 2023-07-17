#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "vm.h"
int main(int argc, char* argv[]) {

    state.memory = malloc(MEM_SIZE * sizeof(Word));

    load(argv[1]);

    interpret();

    dump("mem.dump");

    free(state.memory);

    return 0;
}
