%locations
%pure_parser
%defines
%error-verbose

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
union YYSTYPE;
struct YYLTYPE;
extern int yylex (YYSTYPE* yylval_param, YYLTYPE* yylloc_param);
extern char *yytext;

#define DBG(node, startToken, endToken) node->SetLocation(startToken.first_line, endToken.last_line, startToken.first_column, endToken.last_column)

%}

%token METHOD EQUALS MULTIPLY DIVIDE PLUS MINUS INTEGER_NUMBER FLOAT_NUMBER IDENTIFIER BRACKET_START BRACKET_END SEMICOLON DOT
%token STRING_TOKEN PARAN_START PARAN_END LESS MORE COMMA SQUARE_BRACKET_START SQUARE_BRACKET_END DEBUG_TOKEN
%token RETURN_TOKEN EXTENDS STRUCT

%left MINUS PLUS
%left MULTIPLY DIVIDE

%union {
    ArenaNode* arenaNode;
    CallNode* callNode;
    IdentifierNode* identifierNode;
    NodeList* nodeList;
    StatementList* statementList;
    IdentifierList* identifierList;
    TypeNodeList* typeNodeList;
    StatementNode* statementNode;
    TypeNode* typeNode;
    ArgumentNodeList* argumentNodeList;
    ArgumentNode* argumentNode;
}

%type <arenaNode> Literal MultiplyExpression Expression PlusExpression LeftSide
%type <identifierNode> Identifier
%type <callNode> CallExpression
%type <nodeList> ExpressionList
%type <statementNode> EmptyStatement GlobalStatement InMethodStatement InStructStatement AssignmentStatement 
%type <statementNode> MethodNode ExpressionStatement VariableDeclarationStatement ReturnStatement StructNode DebugStatement
%type <statementList> GlobalStatementList InMethodStatementList InStructStatementList
%type <identifierList> IdentifierList
%type <typeNodeList> TypeDeclarationList
%type <typeNode> TypeDeclaration
%type <argumentNodeList> ArgumentDeclarationList
%type <argumentNode> ArgumentDeclaration

%start Program

%%

Program:
  /* empty */       { Arena::Active()->SetResult(0); }
| GlobalStatementList    { Arena::Active()->SetResult($1); }
;

GlobalStatementList:
  GlobalStatement     { $$ = new StatementList(); $$->push_back($1); DBG($$, @1, @1); }
| GlobalStatementList GlobalStatement   { $$ = $1; $$->push_back($2); DBG($$, @1, @2); }
;

InMethodStatementList:
  InMethodStatement     { $$ = new StatementList(); $$->push_back($1); DBG($$, @1, @1); }
| InMethodStatementList InMethodStatement   { $$ = $1; $$->push_back($2); DBG($$, @1, @2); }
;

InStructStatementList:
  InStructStatement     { $$ = new StatementList(); $$->push_back($1); DBG($$, @1, @1); }
| InStructStatementList InStructStatement   { $$ = $1; $$->push_back($2); DBG($$, @1, @2); }
;

GlobalStatementList:
  GlobalStatement     { $$ = new StatementList(); $$->push_back($1); DBG($$, @1, @1); }
| GlobalStatementList GlobalStatement   { $$ = $1; $$->push_back($2); DBG($$, @1, @2); }
;

GlobalStatement:
  EmptyStatement
| StructNode
| AssignmentStatement
| MethodNode
| ExpressionStatement
| VariableDeclarationStatement
| ReturnStatement
| DebugStatement
;

InStructStatement:
  MethodNode
| VariableDeclarationStatement
;

InMethodStatement:
  EmptyStatement
| AssignmentStatement
| ExpressionStatement
| VariableDeclarationStatement
| ReturnStatement
| DebugStatement
;

DebugStatement:
 DEBUG_TOKEN Expression SEMICOLON { $$ = new DebugStatement($2); DBG($$, @1, @3); }
;

StructNode:
  STRUCT Identifier BRACKET_START InStructStatementList BRACKET_END                     { $$ = new StructNode($2, 0, $4); DBG($$, @1, @5); }
| STRUCT Identifier EXTENDS TypeDeclaration BRACKET_START InStructStatementList BRACKET_END  { $$ = new StructNode($2, $4, $6); DBG($$, @1, @7); }
;

EmptyStatement:
 SEMICOLON  { $$ = 0; }
;

AssignmentStatement:
  LeftSide EQUALS Expression SEMICOLON  { $$ = new AssignNode($1, $3); DBG($$, @1, @4); }
;

ExpressionList:
  Expression    { $$ = new NodeList(); $$->push_back($1); DBG($$, @1, @1); }
| ExpressionList COMMA Expression   { $$ = $1; $$->push_back($3); DBG($$, @1, @3); }
;

LeftSide:
  Identifier { $$ = $1 }
| LeftSide SQUARE_BRACKET_START ExpressionList SQUARE_BRACKET_END  { $$ = new AccessorNode($1, $3); DBG($$, @1, @4); }
| LeftSide DOT Identifier { $$ = new DotNode($1, $3); DBG($$, @1, @3); }
;

MultiplyExpression:
  Literal
| MultiplyExpression MULTIPLY Literal    { $$ = new BinaryOpNode('*', $1, $3); DBG($$, @1, @3); }
| MultiplyExpression DIVIDE Literal      { $$ = new BinaryOpNode('/', $1, $3); DBG($$, @1, @3); }
;

PlusExpression:
  MultiplyExpression
| PlusExpression PLUS MultiplyExpression    { $$ = new BinaryOpNode('+', $1, $3); DBG($$, @1, @3); }
| PlusExpression MINUS MultiplyExpression   { $$ = new BinaryOpNode('-', $1, $3); DBG($$, @1, @3); }
;

Identifier:
  IDENTIFIER        { $$ = new IdentifierNode(yytext); DBG($$, @1, @1); }
;

Literal:
  INTEGER_NUMBER    { $$ = new IntegerValueNode(atoi(yytext)); DBG($$, @1, @1); }
| FLOAT_NUMBER      { $$ = new FloatValueNode(atof(yytext)); DBG($$, @1, @1); }
| STRING_TOKEN      { $$ = new StringValueNode(yytext); DBG($$, @1, @1); }
| CallExpression    { $$ = $1; DBG($$, @1, @1); }
| Identifier        { $$ = $1; DBG($$, @1, @1); }
| PARAN_START Expression PARAN_END  { $$ = $2; DBG($$, @1, @3); }
;

CallExpression:
  LeftSide PARAN_START PARAN_END                  { $$ = new CallNode($1, 0); DBG($$, @1, @3); }
| LeftSide PARAN_START ExpressionList PARAN_END   { $$ = new CallNode($1, $3); DBG($$, @1, @4); }
;

Expression:
  CallExpression    { $$ = $1; DBG($$, @1, @1); }
| PlusExpression
;

ExpressionStatement:
  Expression SEMICOLON { $$ = new ExpressionStatement($1); DBG($$, @1, @2); }
;

MethodNode:
  TypeDeclaration Identifier PARAN_START ArgumentDeclarationList PARAN_END BRACKET_START InMethodStatementList BRACKET_END   { $$ = new MethodNode($1, $2, $4, $7 ); DBG($$, @1, @8); }
;

IdentifierList:
  Identifier    { $$ = new IdentifierList(); $$->push_back($1); DBG($$, @1, @1); }
| IdentifierList COMMA Identifier   { $$ = $1; $$->push_back($3); DBG($$, @1, @3); }
;

ArgumentDeclaration:
  TypeDeclaration Identifier    { $$ = new ArgumentNode($1, $2); DBG($$, @1, @2); }
;

ArgumentDeclarationList:
 /* emtpy */  { $$ = 0; }
|
| ArgumentDeclaration                                 { $$ = new ArgumentNodeList(); $$->push_back($1); DBG($$, @1, @1); }
| ArgumentDeclarationList COMMA ArgumentDeclaration   { $$ = $1; $$->push_back($3); DBG($$, @1, @3); }
;


TypeDeclarationList:
  TypeDeclaration    { $$ = new TypeNodeList(); $$->push_back($1); DBG($$, @1, @1); }
| TypeDeclarationList COMMA TypeDeclaration   { $$ = $1; $$->push_back($3); DBG($$, @1, @3); }
;

TypeDeclaration:
  Identifier                            { $$ = new TypeNode($1, 0); DBG($$, @1, @1); }
| Identifier LESS TypeDeclarationList MORE   { $$ = new TypeNode($1, $3); DBG($$, @1, @3); }
;

VariableDeclarationStatement:
  TypeDeclaration Identifier SEMICOLON                   { $$ = new VarStatement($1, $2, 0); DBG($$, @1, @3); }
| TypeDeclaration Identifier EQUALS Expression SEMICOLON { $$ = new VarStatement($1, $2, $4); DBG($$, @1, @5); }
;

ReturnStatement:
  RETURN_TOKEN SEMICOLON    { $$ = 0; /*fixme*/ }
| RETURN_TOKEN Expression SEMICOLON { $$ = 0; }
;

%%