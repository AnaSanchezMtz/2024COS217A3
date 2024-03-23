all: testsymtablelist testsymtablehash
testsymtablehash: testsymtablehash.o symtablehash.o
	gcc217 testsymtablehash.o symtablehash.o -o testsymtablehash
testsymtablelist: testsymtablelist.o symtablelist.o
	gcc217 testsymtablelist.o symtablelist.o -o testsymtablelist
testsymtablehash.o: testsymtablehash.c symtable.h
	gcc217 -c testsymtablehash.c
testsymtablelist.o: testsymtablelist.c symtable.h
	gcc217 -c testsymtablelist.c
symtablehash.o: symtablehash.o symtable.h
	gcc217 -c symtablehash.c
symtablelist.o: symtablelist.o symtable.h
	gcc217 -c symtablelist.c