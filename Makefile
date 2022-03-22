all: testsymtablelist testsymtablehash

testsymtablelist: testsymtable.o symtablelist.o
	gcc testsymtable.o symtablelist.o -o testsymtablelist
testsymtable.o: testsymtable.c symtable.h
	gcc -c testsymtable.c
symtablelist.o: symtablelist.c symtable.h
	gcc -c symtablelist.c
testsymtablehash: testsymtable.o symtablehash.o
	gcc testsymtable.o symtablehash.o -o testsymtablehash
symtablehash.o: symtablehash.c symtable.h
	gcc symtablehash.c
