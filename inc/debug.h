#ifndef VM__DEBUG_H
#define VM__DEBUG_H

#ifdef DEBUG

#include <stdio.h>
#define DBG_PRINTF(fmt_string, ...) fprintf(stderr, fmt_string, __VA_ARGS__);

#else

#define DBG_PRINTF(...) ;

#endif

#endif
