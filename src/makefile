

INCLUDE = -I ../library/include/
LIBH = ../library/objs/helpers.a
CC = gcc

EX = tarsau
all: $(EX) 
clean: 
	rm -f $(EX) *.o

tarsau: tarsau.o $(LIBH)
	$(CC) $(INCLUDE) -o ../sauzip tarsau.o $(LIBH)
	 

.SUFFIXES: .c .o

.c.o: 
	$(CC) $(INCLUDE) -g  -c  $*.c


