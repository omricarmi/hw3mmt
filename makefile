# HW3 MakeFile
CC = gcc
CFLAGS = -g -Wall
CCLINK = $(CC)
LIBS =
OBJS = list.o graph.o set.o main.o
RM = rm -f

# Creating the executable
robommt: $(OBJS)
	$(CCLINK) -o robommt $(OBJS) $(LIBS)

# Creating object files using default rules

graph.o: graph.c set.h list.h graph.h
list.o: list.c list.h
main.o: main.c list.h set.h graph.h
set.o: set.c list.h set.h

# DEBUG
#test.o: test.c graph.h set.h list.h

# Cleaning old files before new make
clean:
	$(RM) robommt list.o graph.o set.o main.o
