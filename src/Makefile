all: test

test: liblcd.o main.c
	gcc main.c liblcd.o -o test

liblcd.o: fonts.c macros.h types.h
	@gcc fonts.c -c -o liblcd.o
