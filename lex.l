%{

#include <stdio.h>
#include "ParserArena.h"
#include "Nodes.h"

#include "parser.y.hh"

extern void readNext(char* buf, int& result, int maxSize);
#define YY_INPUT(buf,result,max_size) readNext(buf, result, max_size);

int line = 0;

%}

%%

[ \t]+                  /* ignore whitespace */
[\n\r]                  { printf("parsing line %d\n", ++line); }
\.                      return DOT;
method                  return METHOD;
return                  return RETURN_TOKEN;
[0-9]+\.[0-9]+          return FLOAT_NUMBER;
[0-9]+                  return INTEGER_NUMBER;
[a-zA-Z][a-zA-Z0-9]*    return IDENTIFIER;
\"([^\"]|\\\")*\"       return STRING_TOKEN;
\'([^\']|\\\')*\'       return STRING_TOKEN;
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