all: test
	@echo "Building all"
	@echo "Built all."

test: liblcd.o main.c
	@echo "Building the test application:"
	gcc main.c liblcd.o -o test
	@echo "Done test application."

liblcd.o: fonts.c macros.h types.h
	@echo "Building liblcd:"
	@gcc fonts.c -c -o liblcd.o
	@echo "Built the liblcd."
