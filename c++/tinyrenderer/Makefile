SYSCONF_LINK = g++
CPPFLAGS     =
LDFLAGS      = -static-libgcc -static-libstdc++
LIBS         = -lm

SRCDIR   = src
OBJDIR   = obj
DESTDIR  = dist
TARGET   = main

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# target1:
# 	@echo $(SOURCES)
# 	@echo $(OBJECTS)

all: directories $(DESTDIR)/$(TARGET)

directories:
#Window
	@if not exist $(OBJDIR) mkdir $(OBJDIR)
	@if not exist $(DESTDIR) mkdir $(DESTDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(SYSCONF_LINK) -Wall $(CPPFLAGS) -c $(CFLAGS) $< -o $@

$(DESTDIR)/$(TARGET): $(OBJECTS)
	$(SYSCONF_LINK) -Wall $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)

clean:
	-rm -rf $(OBJDIR)
	-rm -rf $(DESTDIR)
	-rm -f *.tga



