/*
 *  parse.y - yacc parser for a small interpreted language
 *
 *  94/11/17 by Urs Thuermann for JuX.
 */

%{

#include <stdio.h>

#include "tree.h"
#include "ops.h"

extern int yylex(void);

static void yyerror(char *s);

/* The top level statement list is stored in the following variable. */
struct stmt *statement;

%}

/*
 *  The value stack contains a value of the following
 *  union type for each symbol in the symbol stack.
 */

%union {
	struct node *node;
	struct expr *expr;
	struct stmt *stmt;
	double      value;
	int         op;
}

/*
 *  Declaration of the tokens and types of nonterminals of the grammar.
 *
 *  The following is somewhat tricky and dirty:
 *  EXPR is not really a token of the grammar, i.e. a symbol which can
 *  be returned by the lexical analyzer.
 *  Its declaration as token is useful though, because it makes yacc
 *  generate a
 *      #define EXPR ...
 *  in the y.tab.h file (parse.tab.h for bison) with a value that is
 *  different from the values defined for IF, FOR, and PRINT.
 *  Thus, we can use it like
 *      mkstmt(EXPR, ...);
 *  in the rules' actions (see below).
 */

%token	IF ELSE DO WHILE FOR PRINT EXPR

%token	<node>	IDENTIFIER
%token	<value>	NUMBER

%type	<expr>	expr opt_expr
%type	<stmt>	stmt stmt_list

/*
 * Precedence of operators from lowest to highest.
 *
 * NEG is not a token in the langauge but this specification produces
 * a token name that can be use in a %prec specification below. We cannot
 * use '-' in two specifications, one for the binary operator and one for
 * the unary operator.
 */

%right	<op>	ASGNOP
%left		LOGOR
%left		LOGAND
%left	<op>	EQCOMPARE
%left	<op>	ARITHCOMP
%left		'+' '-'
%left		'*' '/' '%'
%right	<op>	'!' NEG INCDEC

%%

input	: stmt_list
	{
		statement = $1;
	}
	;

stmt	: '{' stmt_list '}'
	{
		$$ = $2;
	}
	/* The if-statement makes the grammer ambiguous and produces a shift/reduce
	 * conflict.  This is because the statement
	 *       if (e1) if (e2) s1 else s2
	 * can parsed in two ways:
	 *       if (e1) { if (e2) s1 else s2 }
	 * or    if (e1) { if (e2) s1 } else s2
	 *
	 * Like in C, in case of ambiguity we want the 'else' to be part of the
	 * rightmost if-statement possible.  This is achieved by executing shift
	 * instead of reduce which is also the default action in yacc and bison.
	 */
	| IF '(' expr ')' stmt
	{
		$$ = mkstmt(IF, $3, $5, NULL);
	}
	| IF '(' expr ')' stmt ELSE stmt
	{
		$$ = mkstmt(IF, $3, $5, $7);
	}
	| DO stmt WHILE '(' expr ')' ';'
	{
		$$ = mkstmt(DO, $5, $2);
	}
	| WHILE '(' expr ')' stmt
	{
		$$ = mkstmt(WHILE, $3, $5);
	}
	| FOR '(' opt_expr ';' opt_expr ';' opt_expr ')' stmt
	{
		$$ = mkstmt(FOR, $3, $5, $7, $9);
	}
	| PRINT '(' expr ')' ';'
	{
		$$ = mkstmt(PRINT, $3);
	}
	| expr ';'
	{
		$$ = mkstmt(EXPR, $1);
	}
	;

stmt_list
	: stmt_list stmt
	{
		if (!$1)
			$$ = $2;
		else {
			struct stmt *p;

			for (p = $1; p->next; p = p->next)
				;
			p->next = $2;
		}
	}
	| /* EMPTY */
	{
		$$ = NULL;
	}
	;

expr	: '(' expr ')'
	{
		$$ = $2;
	}
	| expr '+' expr
	{
		$$ = mkexpr('+', $1, $3);
	}
	| expr '-' expr
	{
		$$ = mkexpr('-', $1, $3);
	}
	| expr '*' expr
	{
		$$ = mkexpr('*', $1, $3);
	}
	| expr '/' expr
	{
		$$ = mkexpr('/', $1, $3);
	}
	| expr '%' expr
	{
		$$ = mkexpr('%', $1, $3);
	}
	| '-' expr %prec NEG
	{
		$$ = mkexpr(NEG, $2);
	}
	| expr EQCOMPARE expr
	{
		$$ = mkexpr($2, $1, $3);
	}
	| expr ARITHCOMP expr
	{
		$$ = mkexpr($2, $1, $3);
	}
	| expr LOGAND expr
	{
		$$ = mkexpr(LOGAND, $1, $3);
	}
	| expr LOGOR expr
	{
		$$ = mkexpr(LOGOR, $1, $3);
	}
	| '!' expr
	{
		$$ = mkexpr('!', $2);
	}
	| IDENTIFIER INCDEC
	{
		if ($2 == '+')
			$$ = mkexpr(POSTINC, $1);
		else
			$$ = mkexpr(POSTDEC, $1);
	}
	| INCDEC IDENTIFIER
	{
		if ($1 == '+')
			$$ = mkexpr(PREINC, $2);
		else
			$$ = mkexpr(PREDEC, $2);
	}
	| IDENTIFIER ASGNOP expr
	{
		$$ = mkexpr($2, $1, $3);
	}
	| IDENTIFIER
	{
		$$ = mkexpr(IDENTIFIER, $1);
	}
	| NUMBER
	{
		$$ = mkexpr(NUMBER, $1);
	}
	;

opt_expr
	: expr
	| /* EMPTY */
	{
		$$ = NULL;
	}
	;

%%

static void yyerror(char *s)
{
	extern int lineno;

	fprintf(stderr, "%s in line %d\n", s, lineno);
}
