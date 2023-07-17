#include "loader_dumper.h"

void load(char* progpath) {

    int prog_fd = open(progpath, O_RDONLY);
    ssize_t nread;

    nread = read(prog_fd, &state.registers, sizeof(Registers));
    DBG_PRINTF("Register Bank Load: %ld Words\n", BYTES_TO_WORDS(nread));

    nread = read(prog_fd, state.memory, WORDS_TO_BYTES(MEM_SIZE));
    DBG_PRINTF("Program Memory Load: %ld Words\n", BYTES_TO_WORDS(nread));
}

void dump(char* dumppath) {

    int dump_fd = open(dumppath, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    ssize_t nwrite;

    nwrite = write(dump_fd, &state.registers, sizeof(Registers));
    DBG_PRINTF("Register Bank Dump: %ld Words\n", BYTES_TO_WORDS(nwrite));

    nwrite = write(dump_fd, state.memory, WORDS_TO_BYTES(MEM_SIZE));
    DBG_PRINTF("Program Memory Dump: %ld Words\n", BYTES_TO_WORDS(nwrite));
}
