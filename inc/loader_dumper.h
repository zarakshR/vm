#ifndef VM__LOADER_DUMPER_H
#define VM__LOADER_DUMPER_H

#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#include "types.h"
#include "debug.h"

void load(char* progpath);
void dump(char* dumppath);

#endif
