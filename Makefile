## Makefile for the routeplanner project

# name of executable output file in $(BINDIR)
TARGET		= routeplanner.out

CC			= gcc -c
# flags for gcc compiler
CFLAGS		= -O3

LINKER		= gcc -o

# flags for gcc linker
LFLAGS		= -lm -O3

# directories for source, object and binary file(s)
SRCDIR		= src
OBJDIR		= obj
BINDIR		= bin

SOURCES		:= $(wildcard $(SRCDIR)/*.c)
INCLUDES	:= $(wildcard $(SRCDIR)/*.h)
OBJECTS 	:= $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEPENDENCIES:= $(wildcard $(OBJDIR)/*.d)

# link all .o files together
$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(LINKER) $(BINDIR)/$(TARGET) $(LFLAGS) $(OBJECTS)

# compile and generate dependency (.d) file for each .c file
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(SRCDIR)/*.h
	@mkdir -p $(DEPDIR) $(OBJDIR)
	gcc -MM $(CFLAGS) $(SRCDIR)/$*.c > $(OBJDIR)/$*.d
	$(CC) $(CFLAGS) $(SRCDIR)/$*.c -o $(OBJDIR)/$*.o

# remove object and dependency files
clean:
	rm -rf $(BINDIR)/$(TARGET) $(OBJECTS) $(DEPENDENCIES)
	rm -rf $(BINDIR) $(OBJDIR)

# run the program with given arguments
run: $(BINDIR)/$(TARGET)
	./$(BINDIR)/$(TARGET) 1 3 11 5

# create archive with source code
release:
	tar -czvf routeplanner.tgz --exclude=*.tgz --exclude=bin --exclude=obj --exclude=.* *

# install binary to /usr/local/bin
install: $(BINDIR)/$(TARGET)
	sudo cp $(BINDIR)/$(TARGET) /usr/local/bin/routeplanner
