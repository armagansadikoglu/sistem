CC = gcc
CLASSDIR = $(HOME)
INCLUDE = -I$(CLASSDIR)/include/
LIBS = $(CLASSDIR)/objs/libfdr.a
EX = cheal 

all: $(EX)
clean: 
	rm -f $(EX) *.o

cheal: cheal.o $(LIBS)
	$(CC) $(INCLUDE) -o cheal cheal.o $(LIBS)


.SUFFIXES: .c .o

.c.o: 
	$(CC) $(INCLUDE) -g  -c  $*.c

