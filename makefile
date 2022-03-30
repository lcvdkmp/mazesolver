RM=rm -rf
MKDIR=mkdir

# the compiler to use
CC=gcc

# flags to add
CFLAGS=-c -Wall -Wextra -std=c99 -ggdb
LDFLAGS=

# name of the project
PROJECTNAME=maze

# directories where files are stored
SRCDIR=src
BINDIR=bin


SOURCES:=$(wildcard src/*.c)
OBJECTS:=$(SOURCES:src/%.c=src/%.o)
TARGET=$(BINDIR)/$(PROJECTNAME)
LIBS=

# flags used on execution of binary file
EXFLAGS=message\(1\).encrypted2 freq\(1\)

# link
$(TARGET): $(OBJECTS)
	$(MKDIR) -p $(BINDIR)
	@echo "Linking $<..."
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

# get dependency info for existing .o files
-include $(OBJECTS:.o=.d)

# compile and generate dependecies
$(OBJECTS): $(SRCDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@ 
	@echo "Compiling $<..."
	$(CC) -MM $(CFLAGS) $(SOURCES) > $(SRCDIR)/$*.d

.PHONY: clean
clean: 
	$(RM) $(OBJECTS) $(SRCDIR)/*.d
	@echo "Done cleaning!"

.PHONY: remove
remove:
	rm -rf $(TARGET)
	@echo "Target removed!"

.PHONY: run
run: $(TARGET)
	./$(TARGET) $(EXFLAGS)
