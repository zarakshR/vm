#include "loader_dumper.h"

void load(char* progpath) {

    int prog_fd = open(progpath, O_RDONLY);
    ssize_t nread;

    nread = read(prog_fd, &state.registers, sizeof(Registers));

#ifdef DEBUG
    printf("Register Bank Load: %ld Words\n", BYTES_TO_WORDS(nread));
#endif

    nread = read(prog_fd, state.memory, WORDS_TO_BYTES(MEM_SIZE));
#ifdef DEBUG
    printf("Program Memory Load: %ld Words\n", BYTES_TO_WORDS(nread));
#endif
}

void dump(char* dumppath) {

    int dump_fd = open(dumppath, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    ssize_t nwrite;

    nwrite = write(dump_fd, &state.registers, sizeof(Registers));
#ifdef DEBUG
    printf("Register Bank Dump: %ld Words\n", BYTES_TO_WORDS(nwrite));
#endif

    nwrite = write(dump_fd, state.memory, WORDS_TO_BYTES(MEM_SIZE));
#ifdef DEBUG
    printf("Program Memory Dump: %ld Words\n", BYTES_TO_WORDS(nwrite));
#endif
}
