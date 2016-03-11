CFLAGS = -g -Wall -Werror -pedantic -O3
LDADD  = -ldynamic

mp4dump: segment.o atom.o atoms.o main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDADD)

clean:
	rm -f mp4dump *.o
