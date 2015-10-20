%{
	#include <stdio.h>
	#include <math.h>
	int yylex (void);
	void yyerror (char const *);
%}
%define api.value.type {double}

%token INT64
%token UINT64
%token INT32
%token UINT32
%token INT16
%token UINT16
%token INT8
%token UINT8
%token DOUBLE
%token FLOAT
%token STRING
%token UTF8


%token ID
%token GROUP_START GROUP_END // ( )
%token SEQ_START SEQ_END // [ ]
%token NODESET_START NODESET_END // { }
%token END_STMT // ;
%token STMTS_GROUP_START STMTS_GROUP_END // begin end or { }


%left IL1
%right IR2
%left IL3
%right IR4
%left IL5
%right IR6
%left IL7
%right IR8
%left IL9
%right IR10
%left IL11
%right IR12
%left IL13
%right IR14
%left IL15
%right IR16
%left IL17
%right IR18
%left IL19
%right IR20
%left IL21
%right IR22
%left IL23
%right IR24
%left IL25
%right IR26
%left IL27
%right IR28
%left IL29
%right IR30
%left IL31
%right IR32
%precedence PRE1
%precedence POST2
%precedence PRE3
%precedence POST4
%precedence PRE5
%precedence POST6
%precedence PRE7
%precedence POST8
%precedence PRE9
%precedence POST10
%precedence PRE11
%precedence POST12
%precedence PRE13
%precedence POST14
%precedence PRE15
%precedence POST16


%% 
/* Grammar rules and actions follow.
*/

cepsscript:
 stmts
;

stmts:
 %empty
| stmt stmts
| grouping
;

grouping:
 STMTS_GROUP_START stmts STMTS_GROUP_END
;

stmt:
 END_STMT
| juxtaposition END_STMT
;

juxtaposition:
 exp
| exp juxtaposition
;

exp:
INT64
| UINT64
| INT32
| UINT32
| INT16
| UINT16
| INT8
| UINT8
| DOUBLE
| FLOAT
| STRING
| UTF8
| ID
| GROUP_START exp GROUP_END
| NODESET_START stmts NODESET_END
| SEQ_START exp SEQ_END
| exp IL1 exp
| exp IR2 exp
| exp IL3 exp
| exp IR4 exp
| exp IL5 exp
| exp IR6 exp
| exp IL7 exp
| exp IR8 exp
| exp IL9 exp
| exp IR10 exp
| exp IL11 exp
| exp IR12 exp
| exp IL13 exp
| exp IR14 exp
| exp IL15 exp
| exp IR16 exp
| exp IL17 exp
| exp IR18 exp
| exp IL19 exp
| exp IR20 exp
| exp IL21 exp
| exp IR22 exp
| exp IL23 exp
| exp IR24 exp
| exp IL25 exp
| exp IR26 exp
| exp IL27 exp
| exp IR28 exp
| exp IL29 exp
| exp IR30 exp
| exp IL31 exp
| exp IR32 exp
| PRE1 exp
| exp POST2
| PRE3 exp
| exp POST4
| PRE5 exp
| exp POST6
| PRE7 exp
| exp POST8
| PRE9 exp
| exp POST10
| PRE11 exp
| exp POST12
| PRE13 exp
| exp POST14
| PRE15 exp
| exp POST16
;
%%



/* The lexical analyzer returns a double floating point
number on the stack and the token NUM, or the numeric code
of the character read if not a number. It skips all blanks
and tabs, and returns 0 for end-of-input. */
#include <ctype.h>
int yylex (void)
{
 	int c;/* Skip white space. */
 	while ((c = getchar ()) == ' ' || c == '\t' || c == '\n') continue;
	/* Process numbers. */
	if (c == '.' || isdigit (c))
	{
		ungetc (c, stdin);
		scanf ("%lf", &yylval);
		return NUM;
	}
	else if (isalpha(c))
          return ID;
	/* Return end-of-input. */
	if (c == EOF)
		return 0;
	/* Return a single char. */
 	return c;
}

#include <stdio.h>
/* Called by yyparse on error. */
void
yyerror (char const *s)
{
fprintf (stderr, "%s\n", s);
}


int main (void)
{
	return yyparse ();
}










