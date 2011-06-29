/*
 *  symtab.c - the symbol table for a simple interpreter
 *
 *  This symbol table is implemented using a singly linked list.
 *
 *  94/11/17 by Urs Thuermann for JuX.
 */

#include <stdlib.h>
#include <string.h>

#include "symtab.h"

static struct node *root = NULL;

/*
 * Lookup a name in the symbol table and return the associated node.
 * If no entry with that name could be found, insert a new node and
 * return a pointer to it.
 */

struct node *lookup(const char *key)
{
	struct node *n;

	for (n = root; n; n = n->next)
		if (strcmp(key, n->key) == 0)
			return n;

	n = malloc(sizeof(struct node));
	n->next  = root;
	n->key   = strdup(key);
	n->value = 0.0;

	root = n;

	return n;
}
