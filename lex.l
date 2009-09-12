%option yylineno
%option bison-bridge bison-locations

%{

#include <stdio.h>
#include "ParserArena.h"
#include "Nodes.h"

#include "parser.y.hh"

extern void readNext(char* buf, int& result, int maxSize);
#define YY_INPUT(buf,result,max_size) readNext(buf, result, max_size);

#define LOCATION_RESET(Loc)                  \
  (Loc).first_column = (Loc).first_line = 1;  \
  (Loc).last_column =  (Loc).last_line = 1;

/* Advance of NUM lines. */
#define LOCATION_LINES(Loc, Num)             \
  (Loc).last_column = 1;                      \
  (Loc).last_line += Num;

/* Restart: move the first cursor to the last position. */
#define LOCATION_STEP(Loc)                   \
  (Loc).first_column = (Loc).last_column;     \
  (Loc).first_line = (Loc).last_line;

/* Each time we match a string, move the end cursor to its end. */
#define YY_USER_ACTION  yylloc->first_line = yylloc->last_line = yylineno; yylloc->last_column += yyleng;


%}

%%

%{
  /* At each yylex invocation, mark the current position as the
     start of the next token.  */
  LOCATION_STEP (*yylloc_param);
%}


[ \t]+                  LOCATION_STEP (*yylloc);
[\n\r]                  LOCATION_LINES (*yylloc, yyleng); LOCATION_STEP (*yylloc);
\.                      return DOT;
struct                  return STRUCT;
extends                 return EXTENDS;
method                  return METHOD;
return                  return RETURN_TOKEN;
debug                   return DEBUG_TOKEN;
if                      return IF_TOKEN;
else                    return ELSE_TOKEN;
[0-9]+\.[0-9]+          return FLOAT_NUMBER;
[0-9]+                  return INTEGER_NUMBER;
[a-zA-Z][a-zA-Z0-9]*    return IDENTIFIER;
\"([^\"]|\\\")*\"       return STRING_TOKEN;
\'([^\']|\\\')*\'       return STRING_TOKEN;
!                       return NOT;
=                       return EQUALS;
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