%{
#include "hw3_output.hpp"
#include "tokens.h"
#include "parser.tab.hpp"
#include <iostream>
using namespace output;

%}
%option yylineno
%option noyywrap

whitespace [\t\n\r ]
relop ==|!=|<|>|<=|>=
binop \+|\-|\*|\/
comment \/\/[^\r\n]*[\r|\n|\r\n]?
id [a-zA-Z][a-zA-Z0-9]*
num [1-9][0-9]*|0
str \"([^\n\r\"\\]|\\[rnt"\\])+\"
%%
{str}   {yylval=new base_token(yytext); return STRING;}
int		{yylval=new base_token(yytext); return INT;}
byte		{yylval=new base_token(yytext); return BYTE;}
b		{yylval=new base_token(yytext); return B;}
bool		{yylval=new base_token(yytext); return BOOL;}
and		{yylval=new base_token(yytext); return AND;}
or		{yylval=new base_token(yytext); return OR;}
not		{yylval=new base_token(yytext); return NOT;}
true		{yylval=new base_token(yytext); return TRUE;}
false		{yylval=new base_token(yytext); return FALSE;}
return		{yylval=new base_token(yytext); return RETURN;}
if		{yylval=new base_token(yytext); return IF;}
else		{yylval=new base_token(yytext); return ELSE;}
while		{yylval=new base_token(yytext); return WHILE;}
break		{yylval=new base_token(yytext); return BREAK;}
continue	{yylval=new base_token(yytext); return CONTINUE;}
;		{yylval=new base_token(yytext); return SC;}
\(		{yylval=new base_token(yytext); return LPAREN;}
\)		{yylval=new base_token(yytext); return RPAREN;}
\{		{yylval=new base_token(yytext); return LBRACE;}
\}		{yylval=new base_token(yytext); return RBRACE;}
=		{yylval=new base_token(yytext); return ASSIGN;}
{relop}     {yylval=new base_token(yytext); return RELOP;}
\+|\-        {yylval=new base_token(yytext); return ADDOP;}
\/|\*        {yylval=new base_token(yytext); return MULOP;}
{comment}		{}
{id}		{yylval=new base_token(yytext); return ID;}
{num}		{yylval=new base_token(yytext); return NUM;}

{whitespace}	{}
.              {errorLex(yylineno); exit(0);}

%%

