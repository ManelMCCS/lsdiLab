all: main.c lab45.o digital.o
	clang main.c lab45.o digital.o -o exe

lab45.o: lab45.c lab45.h
	clang -c lab45.c

digital.o: digital.c digital.h
	clang -c digital.c



