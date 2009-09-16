%locations
%pure_parser
%defines
%error-verbose
%parse-param { Arena* arena }
%lex-param { void* theScanner }

%{
#include "Platform.h"

#include <stdio.h>
#include <string.h>
#include "ParserArena.h"
#include "Nodes.h"
 
#define YYERROR_VERBOSE
#define YYDEBUG 1

union YYSTYPE;
struct YYLTYPE;
extern char *yytext;
extern int yylex (YYSTYPE* yylval_param, YYLTYPE* yylloc_param, void* theScanner);
void yyerror(YYLTYPE* locp, Arena*, const char *str);

#define DBG(node, startToken, endToken) node->SetLocation(startToken.first_line, endToken.last_line, startToken.first_column, endToken.last_column)

template <typename NodeType>
NodeType* AdoptIntoArena(Arena* arena, NodeType* node)
{
    return static_cast<NodeType*>(arena->RegisterNode(node));
}
#define A(...) (AdoptIntoArena(arena, new __VA_ARGS__))
#define theScanner arena->GetScanner()
#define yytext (arena->GetLexerText())

%}

%token METHOD EQUALS MULTIPLY DIVIDE PLUS MINUS INTEGER_NUMBER FLOAT_NUMBER IDENTIFIER BRACKET_START BRACKET_END SEMICOLON
%token STRING_TOKEN PARAN_START PARAN_END LESS MORE COMMA SQUARE_BRACKET_START SQUARE_BRACKET_END DEBUG_TOKEN MORE_EQUALS
%token RETURN_TOKEN EXTENDS STRUCT IF_TOKEN ELSE_TOKEN NOT WHILE_TOKEN D_EQUALS LESS_EQUALS BIT_AND_TOKEN AND_TOKEN BIT_OR_TOKEN OR_TOKEN
%token PLUSPLUS_TOKEN MINUSMINUS_TOKEN CONTINUE_TOKEN BREAK_TOKEN FOR_TOKEN DOT DOT_LESS ADD_EQUALS READ_TOKEN

%nonassoc IF_WITHOUT_ELSE
%nonassoc ELSE_TOKEN

%union {
    ArenaNode* arenaNode;
    CallNode* callNode;
    IdentifierNode* identifierNode;
    NodeList* nodeList;
    StatementList* statementList;
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
%type <statementNode> MethodNode ExpressionStatement VariableDeclarationStatement ReturnStatement StructNode DebugStatement ReadStatement IfStatement
%type <statementNode> WhileStatement ForStatement ContinueStatement BreakStatement
%type <statementList> GlobalStatementList InMethodStatementList InStructStatementList InBlockStatementList BlockOrStatement
%type <typeNodeList> TypeDeclarationList
%type <typeNode> TypeDeclaration
%type <argumentNodeList> ArgumentDeclarationList
%type <argumentNode> ArgumentDeclaration

%start Program

%%

Program:
  /* empty */       { arena->SetResult(0); }
| GlobalStatementList    { arena->SetResult($1); }
;

GlobalStatementList:
  GlobalStatement     { $$ = A(StatementList()); $$->push_back($1); DBG($$, @1, @1); }
| GlobalStatementList GlobalStatement   { $$ = $1; $$->push_back($2); DBG($$, @1, @2); }
;

InMethodStatementList:
  InMethodStatement     { $$ = A(StatementList()); $$->push_back($1); DBG($$, @1, @1); }
| InMethodStatementList InMethodStatement   { $$ = $1; $$->push_back($2); DBG($$, @1, @2); }
;

InStructStatementList:
  InStructStatement     { $$ = A(StatementList()); $$->push_back($1); DBG($$, @1, @1); }
| InStructStatementList InStructStatement   { $$ = $1; $$->push_back($2); DBG($$, @1, @2); }
;

InBlockStatementList:
  GlobalStatement     { $$ = A(StatementList()); $$->push_back($1); DBG($$, @1, @1); }
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
| ReadStatement
| IfStatement
| WhileStatement
| ForStatement
| ContinueStatement
| BreakStatement
;

BlockOrStatement:
  InBlockStatement                                { $$ = A(StatementList()); $$->push_back($1); DBG($$, @1, @1); } 
| BRACKET_START BRACKET_END                       { $$ = A(StatementList()); DBG($$, @1, @1); }
| BRACKET_START InBlockStatementList BRACKET_END  { $$ = $2; DBG($$, @1, @3); }
;

ContinueStatement:
  CONTINUE_TOKEN SEMICOLON      { $$ = A(ContinueStatement()); }
;

BreakStatement:
  BREAK_TOKEN SEMICOLON         { $$ = A(BreakStatement()); }
;

IfStatement:
  IF_TOKEN PARAN_START Expression PARAN_END BlockOrStatement ELSE_TOKEN BlockOrStatement { $$ = A(IfStatement($3, $5, $7)); DBG($$, @1, @7); }
| IF_TOKEN PARAN_START Expression PARAN_END BlockOrStatement %prec IF_WITHOUT_ELSE { $$ = A(IfStatement($3, $5, 0)); DBG($$, @1, @5); }
;

ExprOp:
  /* empty statement */ { $$ = 0; }
| Expression
;

WhileStatement:
  WHILE_TOKEN PARAN_START ExprOp PARAN_END BlockOrStatement { $$ = A(WhileStatement($3, $5)); DBG($$, @1, @5); }
;

ForStatement:
  FOR_TOKEN PARAN_START ExprOp SEMICOLON ExprOp SEMICOLON ExprOp PARAN_END BlockOrStatement { $$ = A(ForStatement($3, $5, $7, $9)); DBG($$, @1, @9); }
;

DebugStatement:
 DEBUG_TOKEN Expression SEMICOLON { $$ = A(DebugStatement($2)); DBG($$, @1, @3); }
;

ReadStatement:
 READ_TOKEN LeftSide SEMICOLON { $$ = A(ReadStatement($2)); DBG($$, @1, @3); }
;

StructNode:
  STRUCT Identifier BRACKET_START InStructStatementList BRACKET_END                     { $$ = A(StructNode($2, 0, $4)); DBG($$, @1, @5); }
| STRUCT Identifier EXTENDS TypeDeclaration BRACKET_START InStructStatementList BRACKET_END  { $$ = A(StructNode($2, $4, $6)); DBG($$, @1, @7); }
;

EmptyStatement:
 SEMICOLON  { $$ = 0; }
;

AssignmentExpression:
  CompareExpression
| LeftSide EQUALS Expression { $$ = A(AssignNode($1, $3)); DBG($$, @1, @3); }
| LeftSide ADD_EQUALS Expression { $$ = A(AssignOpNode(assign_op_plus_equal, $1, $3)); DBG($$, @1, @3); }
;

ExpressionList:
  Expression    { $$ = A(NodeList()); $$->push_back($1); DBG($$, @1, @1); }
| ExpressionList COMMA Expression   { $$ = $1; $$->push_back($3); DBG($$, @1, @3); }
;

LeftSide:
  Identifier { $$ = $1 }
| LeftSide SQUARE_BRACKET_START Expression SQUARE_BRACKET_END  { $$ = A(AccessorNode($1, $3)); DBG($$, @1, @4); }
| LeftSide DOT Identifier { $$ = A(DotNode($1, $3)); DBG($$, @1, @3); }
;

NegationExpression:
  Literal
| NOT NegationExpression                            { $$ = A(UnaryOpNode(unary_op_not, $2)); DBG($$, @1, @2); }
;

PlusPlusOrMinusMinusExpression:
  NegationExpression
| PLUSPLUS_TOKEN LeftSide                  { $$ = A(AssignOpNode(assign_op_plusplus_prefix, $2)); DBG($$, @1, @2); }
| LeftSide PLUSPLUS_TOKEN                  { $$ = A(AssignOpNode(assign_op_plusplus_sufix, $1)); DBG($$, @1, @2); }
| MINUSMINUS_TOKEN LeftSide                { $$ = A(AssignOpNode(assign_op_minusminus_prefix, $2)); DBG($$, @1, @2); }
| LeftSide MINUSMINUS_TOKEN                { $$ = A(AssignOpNode(assign_op_minusminus_sufix, $1)); DBG($$, @1, @2); }
;

MultiplyExpression:
  PlusPlusOrMinusMinusExpression
| MultiplyExpression MULTIPLY PlusPlusOrMinusMinusExpression    { $$ = A(BinaryOpNode(binary_op_multiply, $1, $3)); DBG($$, @1, @3); }
| MultiplyExpression DIVIDE PlusPlusOrMinusMinusExpression      { $$ = A(BinaryOpNode(binary_op_divide, $1, $3)); DBG($$, @1, @3); }
;

PlusExpression:
  MultiplyExpression
| PlusExpression PLUS MultiplyExpression    { $$ = A(BinaryOpNode(binary_op_plus, $1, $3)); DBG($$, @1, @3); }
| PlusExpression MINUS MultiplyExpression   { $$ = A(BinaryOpNode(binary_op_minus, $1, $3)); DBG($$, @1, @3); }
;

CompareExpression:
  PlusExpression
| CompareExpression LESS PlusExpression         { $$ = A(BinaryOpNode(binary_op_less, $1, $3)); DBG($$, @1, @3); }
| CompareExpression MORE PlusExpression         { $$ = A(BinaryOpNode(binary_op_more, $1, $3)); DBG($$, @1, @3); }
| CompareExpression D_EQUALS PlusExpression     { $$ = A(BinaryOpNode(binary_op_equal, $1, $3)); DBG($$, @1, @3); }
| CompareExpression LESS_EQUALS PlusExpression     { $$ = A(BinaryOpNode(binary_op_less_or_equal, $1, $3)); DBG($$, @1, @3); }
| CompareExpression MORE_EQUALS PlusExpression     { $$ = A(BinaryOpNode(binary_op_more_or_equal, $1, $3)); DBG($$, @1, @3); }
;

Identifier:
  IDENTIFIER        { $$ = A(IdentifierNode(yytext)); DBG($$, @1, @1); }
;

Literal:
  INTEGER_NUMBER    { $$ = A(IntegerValueNode(atoi(yytext))); DBG($$, @1, @1); }
| FLOAT_NUMBER      { $$ = A(FloatValueNode(atof(yytext))); DBG($$, @1, @1); }
| MINUS INTEGER_NUMBER    { $$ = A(IntegerValueNode(-atoi(yytext))); DBG($$, @1, @1); }
| MINUS FLOAT_NUMBER      { $$ = A(FloatValueNode(-atof(yytext))); DBG($$, @1, @1); }
| PLUS INTEGER_NUMBER    { $$ = A(IntegerValueNode(atoi(yytext))); DBG($$, @1, @1); }
| PLUS FLOAT_NUMBER      { $$ = A(FloatValueNode(atof(yytext))); DBG($$, @1, @1); }
| STRING_TOKEN      { $$ = A(StringValueNode(yytext)); DBG($$, @1, @1); }
| PARAN_START Expression PARAN_END  { $$ = $2; DBG($$, @1, @3); }
| LeftSide
| CallExpression    { $$ = $1; DBG($$, @1, @1); }
;

CallExpression:
  TypeDeclaration PARAN_START PARAN_END                  { $$ = A(CallNode($1, 0)); DBG($$, @1, @3); }
| TypeDeclaration PARAN_START ExpressionList PARAN_END   { $$ = A(CallNode($1, $3)); DBG($$, @1, @4); }
;

Expression:
  AssignmentExpression
;

ExpressionStatement:
  Expression SEMICOLON { $$ = A(ExpressionStatement($1)); DBG($$, @1, @2); }
;

MethodNode:
  TypeDeclaration Identifier PARAN_START ArgumentDeclarationList PARAN_END BRACKET_START BRACKET_END   { $$ = A(MethodNode($1, $2, $4, 0 )); DBG($$, @1, @7); }
| TypeDeclaration Identifier PARAN_START ArgumentDeclarationList PARAN_END BRACKET_START InMethodStatementList BRACKET_END   { $$ = A(MethodNode($1, $2, $4, $7 )); DBG($$, @1, @8); }
| METHOD Identifier PARAN_START ArgumentDeclarationList PARAN_END BRACKET_START BRACKET_END   { $$ = A(MethodNode(0, $2, $4, 0 )); DBG($$, @1, @7); }
| METHOD Identifier PARAN_START ArgumentDeclarationList PARAN_END BRACKET_START InMethodStatementList BRACKET_END   { $$ = A(MethodNode(0, $2, $4, $7 )); DBG($$, @1, @8); }
;

ArgumentDeclaration:
  TypeDeclaration Identifier    { $$ = A(ArgumentNode($1, $2)); DBG($$, @1, @2); }
;

ArgumentDeclarationList:
 /* emtpy */  { $$ = 0; }
| ArgumentDeclaration                                 { $$ = A(ArgumentNodeList()); $$->push_back($1); DBG($$, @1, @1); }
| ArgumentDeclarationList COMMA ArgumentDeclaration   { $$ = $1; $$->push_back($3); DBG($$, @1, @3); }
;


TypeDeclarationList:
  TypeDeclaration    { $$ = A(TypeNodeList()); $$->push_back($1); DBG($$, @1, @1); }
| TypeDeclarationList COMMA TypeDeclaration   { $$ = $1; $$->push_back($3); DBG($$, @1, @3); }
;

TypeDeclaration:
  Identifier                            { $$ = A(TypeNode($1, 0)); DBG($$, @1, @1); }
| Identifier DOT_LESS TypeDeclarationList MORE   { $$ = A(TypeNode($1, $3)); DBG($$, @1, @4); }
;

VariableDeclarationStatement:
  TypeDeclaration Identifier SEMICOLON                   { $$ = A(VarStatement($1, $2, 0)); DBG($$, @1, @3); }
| TypeDeclaration Identifier EQUALS Expression SEMICOLON { $$ = A(VarStatement($1, $2, $4)); DBG($$, @1, @5); }
;

ReturnStatement:
  RETURN_TOKEN SEMICOLON            { $$ = A(ReturnStatement( 0 )); DBG($$, @1, @2); }
| RETURN_TOKEN Expression SEMICOLON { $$ = A(ReturnStatement( $2 )); DBG($$, @1, @3); }
;

%%

void yyerror(YYLTYPE* locp, Arena*, const char *str)
{
    fprintf(stderr, "error at line %d: %s\n", locp->first_line, str);
}
