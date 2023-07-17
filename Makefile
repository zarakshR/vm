# r - diables built-in rules
# R - diables built-in variables
MAKEFLAGS += -rR

# Make all targets depend on this Makefile
.EXTRA_PREREQS:= $(abspath $(lastword $(MAKEFILE_LIST)))

PROGRAM_NAME=vm
P=$(PROGRAM_NAME)

INCLUDE_DIR=inc
OBJECT_DIR=obj
BIN_DIR=bin
SOURCE_DIR=src

CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -O3 -D_FORTIFY_SOURCE=2 -I$(INCLUDE_DIR) -I$(OBJECT_DIR) # OBJECT_DIR contains the precompiled headers
LDFLAGS=
DBGFLAGS=-g3 -D DEBUG
SANFLAGS=-fsanitize=address,undefined,null,bounds,float-divide-by-zero,integer-divide-by-zero -fno-omit-frame-pointer # Flags for ASan builds

LDLIBS=

# Header files
HEADERS=vm types interpreter loader_dumper
# Compilation units
UNITS=vm types interpreter loader_dumper

# Generate lists of headers, sources, objects, and object-with-debugging-symbols
HDRS=$(patsubst %, $(OBJECT_DIR)/%.h.gch, $(HEADERS))
SRCS=$(patsubst %,$(SOURCE_DIR)/%.c, $(UNITS))
OBJS=$(patsubst %, $(OBJECT_DIR)/%.o, $(UNITS))
OBJS_DEBUG=$(patsubst %,$(OBJECT_DIR)/%.o.debug, $(UNITS))

# build main binary by default
.PHONY: default
default: $(BIN_DIR)/$(P)

# build main binary, main binary with debug symbols, and main binary built for valgrind
.PHONY: all
all: $(BIN_DIR)/$(P) $(BIN_DIR)/$(P)_debug $(BIN_DIR)/$(P)_sanitize

# Build pre-compiled header file
.SECONDARY: $(OBJECT_DIR)/%.gch
$(OBJECT_DIR)/%.gch: $(INCLUDE_DIR)/%
	@echo Building pre-compiled header: $@
	@mkdir -p $(OBJECT_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

# Build object file
.SECONDARY: $(OBJECT_DIR)/%.o
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c $(HDRS)
	@echo Building object: $@
	@mkdir -p $(OBJECT_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $< -c -o $@ $(LDLIBS)

# Build object file with debugging symbols
.SECONDARY: $(OBJECT_DIR)/%.o.debug
$(OBJECT_DIR)/%.o.debug: $(SOURCE_DIR)/%.c $(HDRS)
	@echo Building object with debug symbols: $@
	@mkdir -p $(OBJECT_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(DBGFLAGS) $< -c -o $@ $(LDLIBS)

# Build main binary
$(BIN_DIR)/$(P): $(OBJS)
	@echo Building $@
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(BIN_DIR)/$(P)_sanitize: $(OBJS_DEBUG)
	@echo Building $@
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(DBGFLAGS) $(SANFLAGS) $^ -o $@ $(LDLIBS)

# Build binary with debugging symbols.
$(BIN_DIR)/$(P)_debug: $(OBJS_DEBUG)
	@echo Building $@
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(DBGFLAGS) $^ -o $@ $(LDLIBS)

# Run program
.PHONY: run
run: $(BIN_DIR)/$(P) program.bin
	$^

# Start gdb on debug build
.PHONY: debug
debug: $(BIN_DIR)/$(P)_debug
	gdb $^

# Run valgrind on debug build
.PHONY: grind
grind: $(BIN_DIR)/$(P)_debug
	valgrind --track-origins=yes --leak-check=full --leak-resolution=high $^

# Run linter
.PHONY: lint
lint: $(SRCS)
	clang-tidy\
	 --quiet\
	 --checks=*,\
	-llvmlibc-restrict-system-libc-headers,\
	-altera-id-dependent-backward-branch,\
	-modernize-macro-to-enum,\
	-altera-unroll-loops,\
	-llvm-include-order,\
	-bugprone-reserved-identifier,\
	-cert-dcl37-c,\
	-cert-dcl51-cpp,\
	-misc-no-recursion,\
	-google-readability-todo\
	 $^\
	 -- -I$(INCLUDE_DIR) # Have to include this or clang will complain about not being able to find header files. See https://stackoverflow.com/a/56457021/15446749

# Format all sources
.PHONY: format
format: $(SRCS)
	clang-format --fallback-style=LLVM -i $(SRCS)

.PHONY: clean
clean:
	rm -rf $(OBJECT_DIR)
	rm -rf $(BIN_DIR)
