%{
#include <stdio.h>
#include <string.h>
#include "ParserArena.h"
#include "Nodes.h"
 
#define YYERROR_VERBOSE
#define YYDEBUG 1

void yyerror(const char *str)
{
    fprintf(stderr,"error: %s\n", str);
}
 
int yywrap() { return 0; }
int yylex(void);
extern char *yytext;

%}

%token METHOD EQUALS MULTIPLY DIVIDE PLUS MINUS INTEGER_NUMBER FLOAT_NUMBER IDENTIFIER BRACKET_START BRACKET_END SEMICOLON DOT
%token STRING_TOKEN PARAN_START PARAN_END LESS MORE COMMA SQUARE_BRACKET_START SQUARE_BRACKET_END
%token RETURN_TOKEN

%left MINUS PLUS
%left MULTIPLY DIVIDE

%union {
    ArenaNode* arenaNode;
    CallNode* callNode;
    IdentifierNode* identifierNode;
    NodeList* nodeList;
    StatementList* statementList;
    IdentifierList* identifierList;
    StatementNode* statementNode;
}

%type <arenaNode> Literal MultiplyExpression Expression PlusExpression LeftSide
%type <identifierNode> Identifier
%type <callNode> CallExpression
%type <nodeList> ExpressionList
%type <statementNode> EmptyStatement Statement AssignmentStatement MethodStatement ExpressionStatement VariableDeclarationStatement ReturnStatement
%type <statementList> StatementList Program
%type <identifierList> IdentifierList

%start Program

%%

Program:
  /* empty */       { Arena::Active()->SetResult(0); }
| StatementList     { Arena::Active()->SetResult($1); }
;

StatementList:
  Statement     { $$ = new StatementList(); $$->push_back($1); }
| StatementList Statement   { $$ = $1; $$->push_back($2); }
;

Statement:
  EmptyStatement
| AssignmentStatement
| MethodStatement
| ExpressionStatement
| VariableDeclarationStatement
| ReturnStatement
;

EmptyStatement:
 SEMICOLON  { $$ = 0; }
;

AssignmentStatement:
  LeftSide EQUALS Expression SEMICOLON  { $$ = new AssignNode($1, $3); }
;

ExpressionList:
  Expression    { $$ = new NodeList(); $$->push_back($1); }
| ExpressionList COMMA Expression   { $$ = $1; $$->push_back($3); }
;

LeftSide:
  Identifier
| LeftSide SQUARE_BRACKET_START ExpressionList SQUARE_BRACKET_END  { $$ = new AccessorNode($1, $3); }
| LeftSide DOT Identifier { $$ = new DotNode($1, $3); }
;

MultiplyExpression:
  PlusExpression
| PlusExpression MULTIPLY MultiplyExpression    { $$ = new BinaryOpNode('*', $1, $3); }
| PlusExpression DIVIDE MultiplyExpression      { $$ = new BinaryOpNode('/', $1, $3); }
;

PlusExpression:
  Literal
| Literal PLUS PlusExpression   { $$ = new BinaryOpNode('+', $1, $3); }
| Literal MINUS PlusExpression  { $$ = new BinaryOpNode('-', $1, $3); }
;

Identifier:
  IDENTIFIER        { $$ = new IdentifierNode(yytext); }
;

Literal:
  INTEGER_NUMBER    { $$ = new IntegerValueNode(atoi(yytext)); }
| FLOAT_NUMBER      { $$ = new FloatValueNode(atof(yytext)); }
| STRING_TOKEN      { $$ = new StringValueNode(yytext); }
| CallExpression    { $$ = $1; }
| Identifier        { $$ = $1; }
| PARAN_START Expression PARAN_END  { $$ = $2; }
;

CallExpression:
  LeftSide PARAN_START PARAN_END                  { $$ = new CallNode($1, 0); }
| LeftSide PARAN_START ExpressionList PARAN_END   { $$ = new CallNode($1, $3); }
;

Expression:
  CallExpression    { $$ = $1; }
| MultiplyExpression
;

ExpressionStatement:
  Expression SEMICOLON
;

MethodStatement:
  METHOD Identifier PARAN_START PARAN_END BRACKET_START StatementList BRACKET_END   { $$ = new MethodStatement( $2, $6 ); }
;

IdentifierList:
  Identifier    { $$ = new IdentifierList(); $$->push_back($1); }
| IdentifierList COMMA Identifier   { $$ = $1; $$->push_back($3); }
;

VariableDeclarationStatement:
  Identifier Identifier SEMICOLON   { $$ = new VarDeclarationStatement($1, $2, 0); }
| Identifier Identifier EQUALS Expression SEMICOLON { $$ = new VarDeclarationStatement($1, $2, $4); }
| Identifier LESS IdentifierList MORE Identifier SEMICOLON { $$ = new TemplateVarDeclarationStatement($1, $3, $5); }
;

ReturnStatement:
  RETURN_TOKEN SEMICOLON
| RETURN_TOKEN Expression SEMICOLON
;

%%