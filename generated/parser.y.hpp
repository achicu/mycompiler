
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     METHOD = 258,
     EQUALS = 259,
     MULTIPLY = 260,
     DIVIDE = 261,
     PLUS = 262,
     MINUS = 263,
     INTEGER_NUMBER = 264,
     FLOAT_NUMBER = 265,
     IDENTIFIER = 266,
     BRACKET_START = 267,
     BRACKET_END = 268,
     SEMICOLON = 269,
     STRING_TOKEN = 270,
     PARAN_START = 271,
     PARAN_END = 272,
     LESS = 273,
     MORE = 274,
     COMMA = 275,
     SQUARE_BRACKET_START = 276,
     SQUARE_BRACKET_END = 277,
     DEBUG_TOKEN = 278,
     MORE_EQUALS = 279,
     RETURN_TOKEN = 280,
     EXTENDS = 281,
     STRUCT = 282,
     IF_TOKEN = 283,
     ELSE_TOKEN = 284,
     NOT = 285,
     WHILE_TOKEN = 286,
     D_EQUALS = 287,
     LESS_EQUALS = 288,
     BIT_AND_TOKEN = 289,
     AND_TOKEN = 290,
     BIT_OR_TOKEN = 291,
     OR_TOKEN = 292,
     PLUSPLUS_TOKEN = 293,
     MINUSMINUS_TOKEN = 294,
     CONTINUE_TOKEN = 295,
     BREAK_TOKEN = 296,
     FOR_TOKEN = 297,
     DOT = 298,
     DOT_LESS = 299,
     ADD_EQUALS = 300,
     READ_TOKEN = 301,
     IF_WITHOUT_ELSE = 302
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 46 "parser.y"

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



/* Line 1676 of yacc.c  */
#line 114 "generated\\parser.y.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif



#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



