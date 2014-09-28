## Makefile for the routeplanner project

# name of executable output file in $(BINDIR)
TARGET		= routeplanner.out

CC			= gcc
# flags for gcc compiler
CFLAGS		+= -std=c99 -S -O3

# flags for gcc linker
LDFLAGS		= 

# directories for source, object and binary file(s)
SRCDIR		= src
ASMDIR		= asm
BINDIR		= bin

SOURCES		:= $(wildcard $(SRCDIR)/*.c)
INCLUDES	:= $(wildcard $(SRCDIR)/*.h)
ASM 		:= $(SOURCES:$(SRCDIR)/%.c=$(ASMDIR)/%.s)

default: $(BINDIR)/$(TARGET)

# link all .o files together
$(BINDIR)/$(TARGET): $(ASM)
	@mkdir -p $(BINDIR)
	$(CC) -o $(BINDIR)/$(TARGET) $(LDFLAGS) $(ASM)

# compile and generate dependency (.d) file for each .c file
$(ASM): $(ASMDIR)/%.s : $(SRCDIR)/%.c $(SRCDIR)/*.h
	@mkdir -p $(ASMDIR)
	$(CC) -c $(CFLAGS) $(SRCDIR)/$*.c -o $(ASMDIR)/$*.s

debug:
	@make clean && CFLAGS="-DDEBUG" make

# remove object and dependency files
clean:
	rm -rf $(BINDIR) $(ASMDIR)

# run the program with given arguments
run: $(BINDIR)/$(TARGET)
	./$(BINDIR)/$(TARGET) 1 3 11 5

# create archive with source code
release:
	tar -czvf routeplanner.tgz --exclude=*.tgz --exclude=*.sh --exclude=bin --exclude=asm --exclude=.* *

# install binary to /usr/local/bin
install: $(BINDIR)/$(TARGET)
	sudo cp $(BINDIR)/$(TARGET) /usr/local/bin/routeplanner
