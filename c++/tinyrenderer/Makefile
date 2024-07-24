SYSCONF_LINK = g++
CPPFLAGS     =
LDFLAGS      = -static-libgcc -static-libstdc++
LIBS         = -lm

DESTDIR = ./dist/
TARGET  = main

OBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

#This sets the default target to build the final executable.
all: $(DESTDIR)$(TARGET)

#Compilation Rule:
# $< represents the input .cpp file.
# $@ represents the output .o file.
$(OBJECTS): %.o: %.cpp
	$(SYSCONF_LINK) -Wall $(CPPFLAGS) -c $(CFLAGS) $< -o $@

#Linking Rule:
$(DESTDIR)$(TARGET): $(OBJECTS)
	$(SYSCONF_LINK) -Wall $(LDFLAGS) -o $(DESTDIR)$(TARGET) $(OBJECTS) $(LIBS)

clean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET)
	-rm -f *.tga

