#ifndef SYMTAB_H
#define SYMTAB_H

struct node {
	struct node *next;
	char        *key;
	double      value;
};

struct node *lookup(const char *key);

#endif
