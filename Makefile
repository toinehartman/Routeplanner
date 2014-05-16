## Makefile for the routeplanner project

# name of executable output file in $(BINDIR)
TARGET		= routeplanner.out

CC			= gcc
# flags for gcc compiler
CFLAGS		+=

# flags for gcc linker
LDFLAGS		= -lm

# directories for source, object and binary file(s)
SRCDIR		= src
OBJDIR		= obj
BINDIR		= bin

SOURCES		:= $(wildcard $(SRCDIR)/*.c)
INCLUDES	:= $(wildcard $(SRCDIR)/*.h)
OBJECTS 	:= $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEPENDENCIES:= $(wildcard $(OBJDIR)/*.d)

default: $(BINDIR)/$(TARGET)

# link all .o files together
$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) -o $(BINDIR)/$(TARGET) $(LDFLAGS) $(OBJECTS)

# compile and generate dependency (.d) file for each .c file
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(SRCDIR)/*.h
	@mkdir -p $(DEPDIR) $(OBJDIR)
	@gcc -MM $(CFLAGS) $(SRCDIR)/$*.c > $(OBJDIR)/$*.d
	$(CC) -c $(CFLAGS) $(SRCDIR)/$*.c -o $(OBJDIR)/$*.o

debug:
	@make clean && CFLAGS="-DDEBUG" make

# remove object and dependency files
clean:
	rm -rf $(BINDIR) $(OBJDIR)

# run the program with given arguments
run: $(BINDIR)/$(TARGET)
	./$(BINDIR)/$(TARGET) 1 3 11 5

# create archive with source code
release:
	tar -czvf routeplanner.tgz --exclude=*.tgz --exclude=*.sh --exclude=bin --exclude=obj --exclude=.* *

# install binary to /usr/local/bin
install: $(BINDIR)/$(TARGET)
	sudo cp $(BINDIR)/$(TARGET) /usr/local/bin/routeplanner
