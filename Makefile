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
	@$(LINKER) $(BINDIR)/$(TARGET) $(LFLAGS) $(OBJECTS)
	@echo "Linking $(OBJECTS), creating $(TARGET)..."
	@echo "Compilation finished!"

# compile and generate dependency (.d) file for each .c file
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(SRCDIR)/*.h
	@mkdir -p $(DEPDIR) $(OBJDIR)
	@gcc -MM $(CFLAGS) $(SRCDIR)/$*.c > $(OBJDIR)/$*.d
	@$(CC) $(CFLAGS) $(SRCDIR)/$*.c -o $(OBJDIR)/$*.o
	@echo "Compiling $(SRCDIR)/$*.c..."

# remove object and dependency files
clean:
	@rm -fv $(BINDIR)/$(TARGET) $(OBJECTS) $(OBJDIR)/*.d
	@echo "All object and dependency files removed!"

# remove all products of make
remove:
	@rm -rf $(BINDIR) $(OBJDIR)
	@echo "All object, dependency and binary files removed!"

# run the program with given arguments
run: $(BINDIR)/$(TARGET)
	./$(BINDIR)/$(TARGET) 11 5
