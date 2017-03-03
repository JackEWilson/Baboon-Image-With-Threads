all:
	gcc -std=c99 -pthread main.c
clean:
	rm -f *.o *.out *.exe
