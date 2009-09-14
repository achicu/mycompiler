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
%token STRING_TOKEN PARAN_START PARAN_END LESS MORE COMMA SQUARE_BRACKET_START SQUARE_BRACKET_END DEBUG_TOKEN MORE_EQUALS
%token RETURN_TOKEN EXTENDS STRUCT IF_TOKEN ELSE_TOKEN NOT WHILE_TOKEN D_EQUALS LESS_EQUALS BIT_AND_TOKEN AND_TOKEN BIT_OR_TOKEN OR_TOKEN
%token PLUSPLUS_TOKEN MINUSMINUS_TOKEN CONTINUE_TOKEN BREAK_TOKEN FOR_TOKEN

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

%type <arenaNode> Literal MultiplyExpression Expression PlusExpression LeftSide NegationExpression CompareExpression AssignmentExpression
%type <arenaNode> PlusPlusOrMinusMinusExpression ExprOp
%type <identifierNode> Identifier
%type <callNode> CallExpression
%type <nodeList> ExpressionList
%type <statementNode> EmptyStatement GlobalStatement InMethodStatement InStructStatement InBlockStatement
%type <statementNode> MethodNode ExpressionStatement VariableDeclarationStatement ReturnStatement StructNode DebugStatement IfStatement
%type <statementNode> WhileStatement ForStatement ContinueStatement BreakStatement
%type <statementList> GlobalStatementList InMethodStatementList InStructStatementList InBlockStatementList BlockOrStatement
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

InBlockStatementList:
  GlobalStatement     { $$ = new StatementList(); $$->push_back($1); DBG($$, @1, @1); }
| InBlockStatementList GlobalStatement   { $$ = $1; $$->push_back($2); DBG($$, @1, @2); }
;

GlobalStatement:
  StructNode
| MethodNode
| InMethodStatement
;

InStructStatement:
  MethodNode
| VariableDeclarationStatement
;

InMethodStatement:
  InBlockStatement
| VariableDeclarationStatement
;

InBlockStatement:
  EmptyStatement
| ExpressionStatement
| ReturnStatement
| DebugStatement
| IfStatement
| WhileStatement
| ForStatement
| ContinueStatement
| BreakStatement
;

BlockOrStatement:
  InBlockStatement                                { $$ = new StatementList(); $$->push_back($1); DBG($$, @1, @1); } 
| BRACKET_START BRACKET_END                       { $$ = new StatementList(); DBG($$, @1, @1); }
| BRACKET_START InBlockStatementList BRACKET_END  { $$ = $2; DBG($$, @1, @3); }
;

ContinueStatement:
  CONTINUE_TOKEN SEMICOLON      { $$ = new ContinueStatement(); }
;

BreakStatement:
  BREAK_TOKEN SEMICOLON         { $$ = new BreakStatement(); }
;

IfStatement:
  IF_TOKEN PARAN_START Expression PARAN_END BlockOrStatement ELSE_TOKEN BlockOrStatement { $$ = new IfStatement($3, $5, $7); DBG($$, @1, @7); }
| IF_TOKEN PARAN_START Expression PARAN_END BlockOrStatement { $$ = new IfStatement($3, $5, 0); DBG($$, @1, @5); }
;

ExprOp:
  /* empty statement */ { $$ = 0; }
| Expression
;

WhileStatement:
  WHILE_TOKEN PARAN_START ExprOp PARAN_END BlockOrStatement { $$ = new WhileStatement($3, $5); DBG($$, @1, @5); }
;

ForStatement:
  FOR_TOKEN PARAN_START ExprOp SEMICOLON ExprOp SEMICOLON ExprOp PARAN_END BlockOrStatement { $$ = new ForStatement($3, $5, $7, $9); DBG($$, @1, @9); }
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

AssignmentExpression:
  CompareExpression
| LeftSide EQUALS Expression { $$ = new AssignNode($1, $3); DBG($$, @1, @3); }
;

ExpressionList:
  Expression    { $$ = new NodeList(); $$->push_back($1); DBG($$, @1, @1); }
| ExpressionList COMMA Expression   { $$ = $1; $$->push_back($3); DBG($$, @1, @3); }
;

LeftSide:
  Identifier { $$ = $1 }
| LeftSide SQUARE_BRACKET_START Expression SQUARE_BRACKET_END  { $$ = new AccessorNode($1, $3); DBG($$, @1, @4); }
| LeftSide DOT Identifier { $$ = new DotNode($1, $3); DBG($$, @1, @3); }
;

NegationExpression:
  Literal
| NOT NegationExpression                            { $$ = new UnaryOpNode(unary_op_not, $2); DBG($$, @1, @2); }
;

PlusPlusOrMinusMinusExpression:
  NegationExpression
| PLUSPLUS_TOKEN LeftSide                  { $$ = new AssignOpNode(assign_op_plusplus_prefix, $2); DBG($$, @1, @2); }
| LeftSide PLUSPLUS_TOKEN                  { $$ = new AssignOpNode(assign_op_plusplus_sufix, $1); DBG($$, @1, @2); }
| MINUSMINUS_TOKEN LeftSide                { $$ = new AssignOpNode(assign_op_minusminus_prefix, $2); DBG($$, @1, @2); }
| LeftSide MINUSMINUS_TOKEN                { $$ = new AssignOpNode(assign_op_minusminus_sufix, $1); DBG($$, @1, @2); }
;

MultiplyExpression:
  PlusPlusOrMinusMinusExpression
| MultiplyExpression MULTIPLY PlusPlusOrMinusMinusExpression    { $$ = new BinaryOpNode(binary_op_multiply, $1, $3); DBG($$, @1, @3); }
| MultiplyExpression DIVIDE PlusPlusOrMinusMinusExpression      { $$ = new BinaryOpNode(binary_op_divide, $1, $3); DBG($$, @1, @3); }
;

PlusExpression:
  MultiplyExpression
| PlusExpression PLUS MultiplyExpression    { $$ = new BinaryOpNode(binary_op_plus, $1, $3); DBG($$, @1, @3); }
| PlusExpression MINUS MultiplyExpression   { $$ = new BinaryOpNode(binary_op_minus, $1, $3); DBG($$, @1, @3); }
;

CompareExpression:
  PlusExpression
| CompareExpression LESS PlusExpression         { $$ = new BinaryOpNode(binary_op_less, $1, $3); DBG($$, @1, @3); }
| CompareExpression MORE PlusExpression         { $$ = new BinaryOpNode(binary_op_more, $1, $3); DBG($$, @1, @3); }
| CompareExpression D_EQUALS PlusExpression     { $$ = new BinaryOpNode(binary_op_equal, $1, $3); DBG($$, @1, @3); }
| CompareExpression LESS_EQUALS PlusExpression     { $$ = new BinaryOpNode(binary_op_less_or_equal, $1, $3); DBG($$, @1, @3); }
| CompareExpression MORE_EQUALS PlusExpression     { $$ = new BinaryOpNode(binary_op_more_or_equal, $1, $3); DBG($$, @1, @3); }
;

Identifier:
  IDENTIFIER        { $$ = new IdentifierNode(yytext); DBG($$, @1, @1); }
;

Literal:
  INTEGER_NUMBER    { $$ = new IntegerValueNode(atoi(yytext)); DBG($$, @1, @1); }
| FLOAT_NUMBER      { $$ = new FloatValueNode(atof(yytext)); DBG($$, @1, @1); }
| MINUS INTEGER_NUMBER    { $$ = new IntegerValueNode(-atoi(yytext)); DBG($$, @1, @1); }
| MINUS FLOAT_NUMBER      { $$ = new FloatValueNode(-atof(yytext)); DBG($$, @1, @1); }
| PLUS INTEGER_NUMBER    { $$ = new IntegerValueNode(atoi(yytext)); DBG($$, @1, @1); }
| PLUS FLOAT_NUMBER      { $$ = new FloatValueNode(atof(yytext)); DBG($$, @1, @1); }
| STRING_TOKEN      { $$ = new StringValueNode(yytext); DBG($$, @1, @1); }
| Identifier        { $$ = $1; DBG($$, @1, @1); }
| PARAN_START Expression PARAN_END  { $$ = $2; DBG($$, @1, @3); }
| LeftSide
| CallExpression    { $$ = $1; DBG($$, @1, @1); }
;

CallExpression:
  TypeDeclaration PARAN_START PARAN_END                  { $$ = new CallNode($1, 0); DBG($$, @1, @3); }
| TypeDeclaration PARAN_START ExpressionList PARAN_END   { $$ = new CallNode($1, $3); DBG($$, @1, @4); }
;

Expression:
  AssignmentExpression
;

ExpressionStatement:
  Expression SEMICOLON { $$ = new ExpressionStatement($1); DBG($$, @1, @2); }
;

MethodNode:
  TypeDeclaration Identifier PARAN_START ArgumentDeclarationList PARAN_END BRACKET_START InMethodStatementList BRACKET_END   { $$ = new MethodNode($1, $2, $4, $7 ); DBG($$, @1, @8); }
| METHOD Identifier PARAN_START ArgumentDeclarationList PARAN_END BRACKET_START BRACKET_END   { $$ = new MethodNode(0, $2, $4, 0 ); DBG($$, @1, @7); }
| METHOD Identifier PARAN_START ArgumentDeclarationList PARAN_END BRACKET_START InMethodStatementList BRACKET_END   { $$ = new MethodNode(0, $2, $4, $7 ); DBG($$, @1, @8); }
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
| ArgumentDeclaration                                 { $$ = new ArgumentNodeList(); $$->push_back($1); DBG($$, @1, @1); }
| ArgumentDeclarationList COMMA ArgumentDeclaration   { $$ = $1; $$->push_back($3); DBG($$, @1, @3); }
;


TypeDeclarationList:
  TypeDeclaration    { $$ = new TypeNodeList(); $$->push_back($1); DBG($$, @1, @1); }
| TypeDeclarationList COMMA TypeDeclaration   { $$ = $1; $$->push_back($3); DBG($$, @1, @3); }
;

TypeDeclaration:
  Identifier                            { $$ = new TypeNode($1, 0); DBG($$, @1, @1); }
| Identifier DOT LESS TypeDeclarationList MORE   { $$ = new TypeNode($1, $4); DBG($$, @1, @4); }
;

VariableDeclarationStatement:
  TypeDeclaration Identifier SEMICOLON                   { $$ = new VarStatement($1, $2, 0); DBG($$, @1, @3); }
| TypeDeclaration Identifier EQUALS Expression SEMICOLON { $$ = new VarStatement($1, $2, $4); DBG($$, @1, @5); }
;

ReturnStatement:
  RETURN_TOKEN SEMICOLON            { $$ = new ReturnStatement( 0 ); DBG($$, @1, @2); }
| RETURN_TOKEN Expression SEMICOLON { $$ = new ReturnStatement( $2 ); DBG($$, @1, @3); }
;

%%