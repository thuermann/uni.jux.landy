/*
 *  tree.c - construction of nodes representing statements and expressions
 *
 *  94/11/17 by Urs Thuermann for JuX.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>

#include "parse.tab.h"

#include "ops.h"
#include "tree.h"

/*
 *  Make a statement node for a statement of a given type
 *  with parameters appropriate for that type.
 *
 *  The argument list is variable and depends on the type as
 *  given by the first argument to mkstmt().
 */

struct stmt *mkstmt(int type, ...)
{
	va_list ap;
	struct stmt *s;

	va_start(ap, type);

	s = malloc(sizeof(struct stmt));
	s->type = type;
	s->next = NULL;

	switch (type) {
		/* For an IF statement read an expression
		 * and two statements from the argument list.
		 */
	case IF:
		s->e1 = va_arg(ap, struct expr *);
		s->s1 = va_arg(ap, struct stmt *);
		s->s2 = va_arg(ap, struct stmt *);
		break;

		/* For a FOR statement read three expressions
		 * and one statement from the argument list.
		 */
	case FOR:
		s->e1 = va_arg(ap, struct expr *);
		s->e2 = va_arg(ap, struct expr *);
		s->e3 = va_arg(ap, struct expr *);
		s->s1 = va_arg(ap, struct stmt *);
		break;

		/* For PRINT and EXPR statements read one
		 * expression from the argument list.
		 */
	case PRINT:
	case EXPR:
		s->e1 = va_arg(ap, struct expr *);
		break;
	}

	va_end(ap);

	return s;
}


/*
 *  Make an expression node for an expression of a given type
 *  with parameters appropriate for that type.
 *
 *  The argument list is variable and depends on the type as
 *  given by the first argument to mkexpr().
 */

struct expr *mkexpr(int type, ...)
{
	va_list ap;
	struct expr *e;

	va_start(ap, type);

	e = malloc(sizeof(struct expr));
	e->type = type;

	switch (type) {
		/* The following binary operators need
		 * two expressions from the argument list.
		 */
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	case EQ:
	case NE:
	case '<':
	case '>':
	case LE:
	case GE:
	case LOGAND:
	case LOGOR:
		e->arg1 = va_arg(ap, struct expr *);
		e->arg2 = va_arg(ap, struct expr *);
		break;

		/* For unary operators one expression is
		 * read from the argument list.
		 */
	case '!':
	case NEG:
		e->arg1 = va_arg(ap, struct expr *);
		break;

		/* Unary operators with an Lvalue argument. */
	case POSTINC:
	case POSTDEC:
	case PREINC:
	case PREDEC:
		e->id = va_arg(ap, struct node *);
		break;

		/* Binary operators with an Lvalue at the left side. */
	case '=':
	case ASSIGNPLUS:
	case ASSIGNMINUS:
	case ASSIGNMULT:
	case ASSIGNDIV:
	case ASSIGNMOD:
		e->id   = va_arg(ap, struct node *);
		e->arg1 = va_arg(ap, struct expr *);
		break;

		/* Expressions consisting of an identifier have a
		 * pointer to the identifier's node associated with
		 * themselves.
		 */
	case IDENTIFIER:
		e->id = va_arg(ap, struct node *);
		break;

		/* For number expressions the double value is passed to
		 * mkexpr() and stored in the expression node created.
		 */
	case NUMBER:
		e->value = va_arg(ap, double);
		break;
	}

	va_end(ap);

	return e;
}
