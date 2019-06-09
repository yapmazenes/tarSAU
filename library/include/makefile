
CFLAGS = -O3

all: helpers.a

OBJS = helpers.o

helpers.a: $(OBJS)
	ar ru ../objs/helpers.a $(OBJS)
	ranlib ../objs/helpers.a

clean:
	rm -f core *.o ../objs/helpers.a

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $*.c

