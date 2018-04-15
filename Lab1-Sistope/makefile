CC=gcc
route=

all: main.o ProcesamientoImg.o Archivo.o
	$(CC) main.o ProcesamientoImg.o Archivo.o -o laboratorio1 -Wall -I.
	rm main.o ProcesamientoImg.o Archivo.o

main.o: $(route)main.c
	$(CC) -c $(route)main.c

ProcesamientoImg.o: $(route)ProcesamientoImg.c
	$(CC) -c $(route)ProcesamientoImg.c

Archivo.o: $(route)Archivo.c
	$(CC) -c $(route)Archivo.c
