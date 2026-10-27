CC = gcc
CFLAGS = -Wall -Wextra

all: crea practica4

crea: crea.c
	$(CC) $(CFLAGS) crea.c -o crea

matriz.txt: crea
	./crea

practica4: practica4.c matriz.txt
	$(CC) $(CFLAGS) practica4.c -o practica4

clean:
	rm -f crea practica4 matriz.txt resultados.txt