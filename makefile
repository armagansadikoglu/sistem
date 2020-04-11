CC = gcc
CLASSDIR = $(HOME)/sistem
INCLUDE = -I$(CLASSDIR)/include/
LIBS = $(CLASSDIR)/libfdr.a
EX = chain_heal 

all: $(EX)
clean: 
	rm -f $(EX) *.o

chain_heal: chain_heal.o $(LIBS)
	$(CC) $(INCLUDE) -o chain_heal chain_heal.o $(LIBS) -lm


.SUFFIXES: .c .o

.c.o: 
	$(CC) $(INCLUDE) -g  -c  $*.c
