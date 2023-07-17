#ifndef VM__LOADER_DUMPER_H
#define VM__LOADER_DUMPER_H

#ifdef DEBUG
#include <stdio.h>
#endif

#include "types.h"

#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

void load(char* progpath);
void dump(char* dumppath);

#endif
