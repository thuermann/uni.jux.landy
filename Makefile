#
# Makefile for a simple interpreter
#
# 94/11/17 by Urs Thuermann for JuX.
#

OBJ = main.o tree.o execute.o symtab.o parse.tab.o scan.o

sl: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

parse.tab.c parse.tab.h: parse.y
	$(YACC) -d -b parse parse.y

main.o:      tree.h execute.h
execute.o:   tree.h execute.h symtab.h ops.h parse.tab.h
tree.o:      tree.h ops.h parse.tab.h
symtab.o:    symtab.h
parse.tab.o: tree.h ops.h
scan.o:      symtab.h ops.h parse.tab.h

clean:
	rm -f *.o parse.tab.c parse.tab.h scan.c
