CC = gcc
CLASSDIR = $(HOME)/sistem
INCLUDE = -I$(CLASSDIR)/include/
EX = chain_heal 

all: $(EX)
clean: 
	rm -f $(EX) *.o

chain_heal:
	$(CC) $(INCLUDE) -g chain_heal.c -o chain_heal -lm

