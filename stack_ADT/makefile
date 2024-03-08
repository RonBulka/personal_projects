CC= gcc
CCFLAGS=-g -Wall -std=c99
CCLINK=$(CC)
OBJS= main.o student.o stack.o
EXEC=prog.exe
RM= rm -rf *.o *.exe

$(EXEC): $(OBJS)
	$(CCLINK) $(CCFLAGS) -o $(EXEC) $(OBJS)

main.o: main.c student.h stack.h
	$(CC) $(CCFLAGS) -c main.c
student.o: student.c student.h stack.h
	$(CC) $(CCFLAGS) -c student.c
stack.o: stack.c stack.h
	$(CC) $(CCFLAGS) -c stack.c
clean:
	$(RM)
