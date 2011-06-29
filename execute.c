/*
 *  execute.c - execution of statements and evaluation of expressions
 *
 *  94/11/17 by Urs Thuermann for JuX.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "parse.tab.h"
#include "ops.h"

#include "symtab.h"
#include "tree.h"
#include "execute.h"

static double eval(const struct expr *e);

/* Execute the statement list pointed to by the argument s */
void exc(const struct stmt *s)
{
	for (; s; s = s->next) {
		switch (s->type) {
		case IF:
			if (eval(s->e1))
				exc(s->s1);
			else
				exc(s->s2);
			break;

		case FOR:
			for (eval(s->e1); eval(s->e2); eval(s->e3))
				exc(s->s1);
			break;

		case PRINT:
			printf("%f\n", eval(s->e1));
			break;

		case EXPR:
			eval(s->e1);
			break;

		default:
			fprintf(stderr, "Internal interpreter bug at %s:%d\n",
				__FILE__, __LINE__);
			exit(1);
		}
	}
}

/* Evaluate the expression pointed to by the argument e */
static double eval(const struct expr *e)
{
	if (!e)
		return 1;

	switch (e->type) {
	case NEG:
		return -eval(e->arg1);
	case '+':
		return eval(e->arg1) + eval(e->arg2);
	case '-':
		return eval(e->arg1) - eval(e->arg2);
	case '*':
		return eval(e->arg1) * eval(e->arg2);
	case '/':
		return eval(e->arg1) / eval(e->arg2);
	case '%':
		return fmod(eval(e->arg1), eval(e->arg2));
	case EQ:
		return eval(e->arg1) == eval(e->arg2);
	case NE:
		return eval(e->arg1) != eval(e->arg2);
	case '<':
		return eval(e->arg1) < eval(e->arg2);
	case '>':
		return eval(e->arg1) > eval(e->arg2);
	case LE:
		return eval(e->arg1) <= eval(e->arg2);
	case GE:
		return eval(e->arg1) >= eval(e->arg2);
	case LOGAND:
		return eval(e->arg1) && eval(e->arg2);
	case LOGOR:
		return eval(e->arg1) || eval(e->arg2);
	case '!':
		return !eval(e->arg1);
	case POSTINC:
		return e->id->value++;
	case POSTDEC:
		return e->id->value--;
	case PREINC:
		return ++e->id->value;
	case PREDEC:
		return --e->id->value;
	case '=':
		return e->id->value = eval(e->arg1);
	case ASSIGNPLUS:
		return e->id->value += eval(e->arg1);
	case ASSIGNMINUS:
		return e->id->value -= eval(e->arg1);
	case ASSIGNMULT:
		return e->id->value *= eval(e->arg1);
	case ASSIGNDIV:
		return e->id->value /= eval(e->arg1);
	case ASSIGNMOD:
		return e->id->value = fmod(e->id->value, eval(e->arg1));
	case IDENTIFIER:
		return e->id->value;
	case NUMBER:
		return e->value;
	default:
		fprintf(stderr, "Internal interpreter bug at %s:%d\n",
			__FILE__, __LINE__);
		exit(1);
	}
}
