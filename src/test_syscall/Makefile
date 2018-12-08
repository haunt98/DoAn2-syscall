CC = gcc

all:
	@echo "make test_pnametoid for testing pnametoid system call"
	@echo "make test_pidtoname for testing pidtoname system call"

test_pnametoid:
	$(CC) test_pnametoid.c -o test_pnametoid.o
	./test_pnametoid.o

test_pidtoname:
	$(CC) test_pidtoname.c -o test_pidtoname.o
	./test_pidtoname.o

clean:
	rm *.o
