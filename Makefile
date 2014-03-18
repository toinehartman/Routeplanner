# project name (generate executable with this name)
TARGET		= routeplanner

CC			= gcc -c
# compiling flags here
CFLAGS		= -O3

LINKER		= gcc -o
# linking flags here
LFLAGS		= -lm -O3

# change these to set the proper directories where each files shoould be
SRCDIR		= src
OBJDIR		= obj
BINDIR		= bin

SOURCES		:= $(wildcard $(SRCDIR)/*.c)
INCLUDES	:= $(wildcard $(SRCDIR)/*.h)
OBJECTS 	:= $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEPENDENCIES:= $(wildcard $(OBJDIR)/*.d)

# link
$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(LINKER) $(BINDIR)/$(TARGET) $(LFLAGS) $(OBJECTS)

# compile and generate dependency info
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(SRCDIR)/*.h
	@mkdir -p $(DEPDIR) $(OBJDIR)
	@gcc -MM $(CFLAGS) $(SRCDIR)/$*.c > $(OBJDIR)/$*.d
	$(CC) $(CFLAGS) $(SRCDIR)/$*.c -o $(OBJDIR)/$*.o

# remove compilation products
clean:
	@rm -fv $(BINDIR)/$(TARGET) $(OBJECTS) $(OBJDIR)/*.d

remove:
	@rm -rfv $(BINDIR) $(OBJDIR)

run:
	./$(BINDIR)/$(TARGET) 11 5
