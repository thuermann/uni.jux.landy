/*
 *  scan.l - lex scanner for a small interpreted language
 *
 *  94/11/17 by Urs Thuermann for JuX.
 */

%{

#include <stdlib.h>

#include "symtab.h"
#include "ops.h"
#include "parse.tab.h"

/* Provide the input line number for yyerror(). */
int lineno = 1;

/* Stop scanning at EOF. */
int yywrap(void)
{
	return 1;
}

%}

%x COMMENT

alpha	[_a-zA-Z]
digit	[0-9]
alnum	({alpha}|{digit})
exp	([eE][+-]?{digit}+)

ws	[ \t\f\r\b]
cbody	\/*([^*/\n]\/*|\*)*

%%

	/* keywords */

if				return IF;
else				return ELSE;
for				return FOR;
print				return PRINT;

	/* identifiers */

{alpha}{alnum}*			{
					yylval.node = lookup(yytext);
					return IDENTIFIER;
				}

	/* numbers.  integers are handled like floats */

{digit}+			|

{digit}*\.{digit}+{exp}?	|
{digit}+\.{exp}?		|
{digit}+{exp}			{
					yylval.value = atof(yytext);
					return NUMBER;
				}

	/* assignment operators */

"="				yylval.op = '=';	 return ASGNOP;
"+="				yylval.op = ASSIGNPLUS;	 return ASGNOP;
"-="				yylval.op = ASSIGNMINUS; return ASGNOP;
"*="				yylval.op = ASSIGNMULT;	 return ASGNOP;
"/="				yylval.op = ASSIGNDIV;	 return ASGNOP;

	/* some other operators */

[-+*/]				return yytext[0];

"=="				yylval.op = EQ;	 return EQCOMPARE;
"!="				yylval.op = NE;	 return EQCOMPARE;
"<"				yylval.op = '<'; return ARITHCOMP;
">"				yylval.op = '>'; return ARITHCOMP;
"<="				yylval.op = LE;	 return ARITHCOMP;
">="				yylval.op = GE;	 return ARITHCOMP;

"&&"				return LOGAND;
"||"				return LOGOR;

"++"				yylval.op = '+'; return INCDEC;
"--"				yylval.op = '-'; return INCDEC;

	/* punctuation, parenthesis, braces, ... */

[(){};]				return yytext[0];

	/* white-space */

{ws}+				;
\n				lineno++;

	/* comments in C and C++ style */

"/*"				BEGIN(COMMENT);
<COMMENT>{cbody}\n		lineno++;
<COMMENT>{cbody}"*/"		BEGIN(0);
"//".*				;
