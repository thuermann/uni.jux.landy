/*
 *  main.c - the main() function of a simple interpreter
 *
 *  94/11/17 by Urs Thuermann for JuX.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "execute.h"

extern FILE *yyin;
extern int yydebug;
extern int yylex();
extern int yyparse();

extern struct stmt *statement;

/*
 *  main() tests if the first argument is "-l" in which case it repeatedly
 *  calls yylex() and prints the token value in decimal.  Token attributes
 *  are not printed.
 *
 *  Otherwise, the function yyparse() is called to parse a statement.  If
 *  yyparse() returns successfully the statement is executed by a call
 *  to exc().
 *
 *  In either case, there must be a file name argument specifying the file
 *  to be opened for reading.
 *  There is little error checking since this is only a small example
 *  for demonstration of lex and yacc and I spent the time I could use
 *  to code error checking in commenting instead.
 */

static void usage(void)
{
	fputs("Usage: sl [-l] file\n", stderr);
	exit(1);
}

int main(int argc, char **argv)
{
	int token, lex_only = 0;

	if (argc > 1 && strcmp(argv[1], "-l") == 0) {
		lex_only = 1;
		argc--, argv++;
	}
	if (argc != 2)
		usage();

	if (!(yyin = fopen(argv[1], "r"))) {
		perror(argv[1]);
		exit(1);
	}

	if (lex_only) {
		while (token = yylex())
			printf("%d\n", token);
	} else {
		if (yyparse() == 1) {
			/* An error has occured in parsing but it
			 * has already been reported by yyerror().
			 */
			exit(1);
		}
		exc(statement);
	}

	return 0;
}
