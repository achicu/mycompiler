%option yylineno
%option bison-bridge bison-locations
%option reentrant
%option noyywrap
   
%{

#include <iostream>
#include "ParserArena.h"
#include "Nodes.h"
#include "parser.y.hh"

#define YY_EXTRA_TYPE Arena*

#define LOCATION_RESET(Loc)                  \
  (Loc).first_column = (Loc).first_line = 1; \
  (Loc).last_column =  (Loc).last_line = 1;

/* Advance of NUM lines. */
#define LOCATION_LINES(Loc, Num)             \
  (Loc).last_column = 1;                     \
  (Loc).last_line += Num;

/* Restart: move the first cursor to the last position. */
#define LOCATION_STEP(Loc)                   \
  (Loc).first_column = (Loc).last_column;    \
  (Loc).first_line = (Loc).last_line;

/* Each time we match a string, move the end cursor to its end. */
#define YY_USER_ACTION  yylloc->first_line = yylloc->last_line = yylineno; yylloc->last_column += yyleng;

#define YY_INPUT(buf,result,max_size) \
    result = yyextra->Read(buf, max_size);

%}

%%

%{
  /* At each yylex invocation, mark the current position as the
     start of the next token.  */
  LOCATION_STEP (*yylloc_param);
%}


[ \t]+                  LOCATION_STEP (*yylloc);
[\n\r]                  LOCATION_LINES (*yylloc, yyleng); LOCATION_STEP (*yylloc);
\/\/([^\n\r]*)          LOCATION_STEP (*yylloc); // single line comment
\#([^\n\r]*)            LOCATION_STEP (*yylloc); // single line comment
\.\<                    return DOT_LESS;
\.                      return DOT;
struct                  return STRUCT;
extends                 return EXTENDS;
method                  return METHOD;
return                  return RETURN_TOKEN;
debug                   return DEBUG_TOKEN;
if                      return IF_TOKEN;
else                    return ELSE_TOKEN;
while                   return WHILE_TOKEN;
for                     return FOR_TOKEN;
continue                return CONTINUE_TOKEN;
break                   return BREAK_TOKEN;
[0-9]+\.[0-9]+          return FLOAT_NUMBER;
[0-9]+                  return INTEGER_NUMBER;
[a-zA-Z$][a-zA-Z0-9]*    return IDENTIFIER;
\"([^\"]|\\\")*\"       return STRING_TOKEN;
\'([^\']|\\\')*\'       return STRING_TOKEN;
!                       return NOT;
==                      return D_EQUALS;
=\>                     return MORE_EQUALS;
=                       return EQUALS;
\<=                     return LESS_EQUALS;
\>=                     return MORE_EQUALS;
&                       return BIT_AND_TOKEN;
&&                      return AND_TOKEN;
\|                      return BIT_OR_TOKEN;
\|\|                    return OR_TOKEN;
--                      return MINUSMINUS_TOKEN;
\+\+                    return PLUSPLUS_TOKEN;
\+=                     return ADD_EQUALS;
\*                      return MULTIPLY;
\/                      return DIVIDE;
\+                      return PLUS;
-                       return MINUS;
\<                      return LESS;
\>                      return MORE;
,                       return COMMA;
\{                      return BRACKET_START;
\}                      return BRACKET_END;
;                       return SEMICOLON;
\(                      return PARAN_START;
\)                      return PARAN_END;
\[                      return SQUARE_BRACKET_START;
\]                      return SQUARE_BRACKET_END;

%%

void Arena::InitScanner()
{
    yylex_init(&m_scanner);
    yyset_extra(this, m_scanner);
}

void Arena::DestroyScanner()
{
    yylex_destroy(m_scanner);
}

char* Arena::GetLexerText() const
{
    return yyget_text(m_scanner);
}

