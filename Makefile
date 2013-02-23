# Basic Stuff -----------------------------
CC = g++ -g -Wall

# Libraries -------------------------------
CCOPTS = -c -I./Dist/
LDOPTS = -L./Dist/ -lfreeimage

# Final Files and Intermediate .o files ---
SOURCES = main.cpp Film.cpp #add all sources here
OBJECTS = main.o Film.o #add all .o files here
TARGET = as2

# -----------------------------------------
main: $(OBJECTS)
	$(CC) $(LDOPTS) $(OBJECTS) -o $(TARGET)

main.o:
	$(CC) $(CCOPTS) $(SOURCES)

default: $(TARGET)

clean:
	/bin/rm -f *.o $(TARGETS)