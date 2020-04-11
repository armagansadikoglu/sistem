CC = gcc
CLASSDIR = $(HOME)/sistem
INCLUDE = -I$(CLASSDIR)/include/
LIBS = $(CLASSDIR)/libfdr.a
EX = cheal 

all: $(EX)
clean: 
	rm -f $(EX) *.o

cheal: cheal.o $(LIBS)
	$(CC) $(INCLUDE) -o cheal cheal.o $(LIBS) -lm


.SUFFIXES: .c .o

.c.o: 
	$(CC) $(INCLUDE) -g  -c  $*.c

