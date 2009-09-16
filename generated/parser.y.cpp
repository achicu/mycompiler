
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 1



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 8 "parser.y"

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



/* Line 189 of yacc.c  */
#line 104 "generated\\parser.y.cpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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

/* Line 214 of yacc.c  */
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



/* Line 214 of yacc.c  */
#line 202 "generated\\parser.y.cpp"
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


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 227 "generated\\parser.y.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  74
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   562

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  102
/* YYNRULES -- Number of states.  */
#define YYNSTATES  189

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   302

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     8,    11,    13,    16,    18,
      21,    23,    26,    28,    30,    32,    34,    36,    38,    40,
      42,    44,    46,    48,    50,    52,    54,    56,    58,    60,
      62,    65,    69,    72,    75,    83,    89,    90,    92,    98,
     108,   112,   116,   122,   130,   132,   134,   138,   142,   144,
     148,   150,   155,   159,   161,   164,   166,   169,   172,   175,
     178,   180,   184,   188,   190,   194,   198,   200,   204,   208,
     212,   216,   220,   222,   224,   226,   229,   232,   235,   238,
     240,   244,   246,   248,   252,   257,   259,   262,   270,   279,
     287,   296,   299,   300,   302,   306,   308,   312,   314,   319,
     323,   329,   332
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      49,     0,    -1,    -1,    50,    -1,    54,    -1,    50,    54,
      -1,    56,    -1,    51,    56,    -1,    55,    -1,    52,    55,
      -1,    54,    -1,    53,    54,    -1,    67,    -1,    82,    -1,
      56,    -1,    82,    -1,    87,    -1,    57,    -1,    87,    -1,
      68,    -1,    81,    -1,    88,    -1,    65,    -1,    66,    -1,
      61,    -1,    63,    -1,    64,    -1,    59,    -1,    60,    -1,
      57,    -1,    12,    13,    -1,    12,    53,    13,    -1,    40,
      14,    -1,    41,    14,    -1,    28,    16,    80,    17,    58,
      29,    58,    -1,    28,    16,    80,    17,    58,    -1,    -1,
      80,    -1,    31,    16,    62,    17,    58,    -1,    42,    16,
      62,    14,    62,    14,    62,    17,    58,    -1,    23,    80,
      14,    -1,    46,    71,    14,    -1,    27,    77,    12,    52,
      13,    -1,    27,    77,    26,    86,    12,    52,    13,    -1,
      14,    -1,    76,    -1,    71,     4,    80,    -1,    71,    45,
      80,    -1,    80,    -1,    70,    20,    80,    -1,    77,    -1,
      71,    21,    80,    22,    -1,    71,    43,    77,    -1,    78,
      -1,    30,    72,    -1,    72,    -1,    38,    71,    -1,    71,
      38,    -1,    39,    71,    -1,    71,    39,    -1,    73,    -1,
      74,     5,    73,    -1,    74,     6,    73,    -1,    74,    -1,
      75,     7,    74,    -1,    75,     8,    74,    -1,    75,    -1,
      76,    18,    75,    -1,    76,    19,    75,    -1,    76,    32,
      75,    -1,    76,    33,    75,    -1,    76,    24,    75,    -1,
      11,    -1,     9,    -1,    10,    -1,     8,     9,    -1,     8,
      10,    -1,     7,     9,    -1,     7,    10,    -1,    15,    -1,
      16,    80,    17,    -1,    71,    -1,    79,    -1,    86,    16,
      17,    -1,    86,    16,    70,    17,    -1,    69,    -1,    80,
      14,    -1,    86,    77,    16,    84,    17,    12,    13,    -1,
      86,    77,    16,    84,    17,    12,    51,    13,    -1,     3,
      77,    16,    84,    17,    12,    13,    -1,     3,    77,    16,
      84,    17,    12,    51,    13,    -1,    86,    77,    -1,    -1,
      83,    -1,    84,    20,    83,    -1,    86,    -1,    85,    20,
      86,    -1,    77,    -1,    77,    44,    85,    19,    -1,    86,
      77,    14,    -1,    86,    77,     4,    80,    14,    -1,    25,
      14,    -1,    25,    80,    14,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    78,    78,    79,    83,    84,    88,    89,    93,    94,
      98,    99,   103,   104,   105,   109,   110,   114,   115,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   132,
     133,   134,   138,   142,   146,   147,   151,   152,   156,   160,
     164,   168,   172,   173,   177,   181,   182,   183,   187,   188,
     192,   193,   194,   198,   199,   203,   204,   205,   206,   207,
     211,   212,   213,   217,   218,   219,   223,   224,   225,   226,
     227,   228,   232,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   249,   250,   254,   258,   262,   263,   264,
     265,   269,   273,   274,   275,   280,   281,   285,   286,   290,
     291,   295,   296
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "METHOD", "EQUALS", "MULTIPLY", "DIVIDE",
  "PLUS", "MINUS", "INTEGER_NUMBER", "FLOAT_NUMBER", "IDENTIFIER",
  "BRACKET_START", "BRACKET_END", "SEMICOLON", "STRING_TOKEN",
  "PARAN_START", "PARAN_END", "LESS", "MORE", "COMMA",
  "SQUARE_BRACKET_START", "SQUARE_BRACKET_END", "DEBUG_TOKEN",
  "MORE_EQUALS", "RETURN_TOKEN", "EXTENDS", "STRUCT", "IF_TOKEN",
  "ELSE_TOKEN", "NOT", "WHILE_TOKEN", "D_EQUALS", "LESS_EQUALS",
  "BIT_AND_TOKEN", "AND_TOKEN", "BIT_OR_TOKEN", "OR_TOKEN",
  "PLUSPLUS_TOKEN", "MINUSMINUS_TOKEN", "CONTINUE_TOKEN", "BREAK_TOKEN",
  "FOR_TOKEN", "DOT", "DOT_LESS", "ADD_EQUALS", "READ_TOKEN",
  "IF_WITHOUT_ELSE", "$accept", "Program", "GlobalStatementList",
  "InMethodStatementList", "InStructStatementList", "InBlockStatementList",
  "GlobalStatement", "InStructStatement", "InMethodStatement",
  "InBlockStatement", "BlockOrStatement", "ContinueStatement",
  "BreakStatement", "IfStatement", "ExprOp", "WhileStatement",
  "ForStatement", "DebugStatement", "ReadStatement", "StructNode",
  "EmptyStatement", "AssignmentExpression", "ExpressionList", "LeftSide",
  "NegationExpression", "PlusPlusOrMinusMinusExpression",
  "MultiplyExpression", "PlusExpression", "CompareExpression",
  "Identifier", "Literal", "CallExpression", "Expression",
  "ExpressionStatement", "MethodNode", "ArgumentDeclaration",
  "ArgumentDeclarationList", "TypeDeclarationList", "TypeDeclaration",
  "VariableDeclarationStatement", "ReturnStatement", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    48,    49,    49,    50,    50,    51,    51,    52,    52,
      53,    53,    54,    54,    54,    55,    55,    56,    56,    57,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    58,
      58,    58,    59,    60,    61,    61,    62,    62,    63,    64,
      65,    66,    67,    67,    68,    69,    69,    69,    70,    70,
      71,    71,    71,    72,    72,    73,    73,    73,    73,    73,
      74,    74,    74,    75,    75,    75,    76,    76,    76,    76,
      76,    76,    77,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    79,    79,    80,    81,    82,    82,    82,
      82,    83,    84,    84,    84,    85,    85,    86,    86,    87,
      87,    88,    88
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     1,     2,     1,     2,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     3,     2,     2,     7,     5,     0,     1,     5,     9,
       3,     3,     5,     7,     1,     1,     3,     3,     1,     3,
       1,     4,     3,     1,     2,     1,     2,     2,     2,     2,
       1,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       3,     3,     1,     1,     1,     2,     2,     2,     2,     1,
       3,     1,     1,     3,     4,     1,     2,     7,     8,     7,
       8,     2,     0,     1,     3,     1,     3,     1,     4,     3,
       5,     2,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     0,    73,    74,    72,    44,    79,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     3,     4,    14,    17,    27,    28,    24,
      25,    26,    22,    23,    12,    19,    85,    81,    55,    60,
      63,    66,    45,    50,    53,    82,     0,    20,    13,     0,
      18,    21,     0,    77,    78,    75,    76,     0,     0,     0,
     101,     0,     0,     0,    81,    54,    36,    56,    50,    58,
      32,    33,    36,     0,     1,     5,     0,     0,    57,    59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,    92,    80,    40,   102,     0,
       0,     0,     0,    37,     0,    41,    46,     0,    52,    47,
      81,    61,    62,    64,    65,    67,    68,    71,    69,    70,
      97,     0,    95,    83,     0,    48,     0,    99,    92,    93,
       0,     0,     0,     8,    15,     0,    16,     0,     0,     0,
      36,    51,    98,     0,    84,     0,     0,     0,     0,     0,
      91,    42,     9,     0,     0,    29,    35,    38,     0,    96,
      49,   100,     0,     0,    94,     0,    30,     0,    10,     0,
      36,     0,    89,     0,     6,     0,    43,    31,    11,    34,
       0,    87,     0,    90,     7,     0,     0,    88,    39
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    22,    23,   173,   132,   167,    24,   133,    25,    26,
     156,    27,    28,    29,   102,    30,    31,    32,    33,    34,
      35,    36,   124,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,   129,   130,   121,    58,    50,
      51
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -138
static const yytype_int16 yypact[] =
{
     256,    -1,    16,    87,  -138,  -138,  -138,  -138,  -138,   507,
     507,   109,    -1,    29,   532,    38,    -1,    -1,    44,    46,
      69,    -1,   115,   256,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,  -138,  -138,     8,  -138,  -138,
      74,   106,   126,     6,  -138,  -138,    55,  -138,  -138,    65,
    -138,  -138,    76,  -138,  -138,  -138,  -138,   104,   113,   112,
    -138,   119,     4,   507,    -3,  -138,   507,    -3,  -138,    -3,
    -138,  -138,   507,    14,  -138,  -138,   507,   507,  -138,  -138,
      -1,   507,   507,   507,   507,   507,   507,   507,   507,   507,
     507,    -1,  -138,   496,    17,    -1,  -138,  -138,  -138,    53,
      -1,   134,   140,  -138,   150,  -138,  -138,   147,  -138,  -138,
      27,  -138,  -138,    74,    74,   106,   106,   106,   106,   106,
     132,   141,  -138,  -138,    70,  -138,   507,  -138,    -1,  -138,
     110,    -1,    75,  -138,  -138,    -1,  -138,   165,   296,   296,
     507,  -138,  -138,    -1,  -138,   507,   174,   120,   186,    -1,
    -138,  -138,  -138,    53,   176,  -138,   171,  -138,   188,  -138,
    -138,  -138,   193,   336,  -138,   159,  -138,   216,  -138,   296,
     507,   376,  -138,   416,  -138,    65,  -138,  -138,  -138,  -138,
     191,  -138,   456,  -138,  -138,    23,   296,  -138,  -138
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -138,  -138,  -138,    39,    56,  -138,   -12,  -124,   -89,  -125,
    -137,  -138,  -138,  -138,   -69,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,    22,   197,    92,    96,   107,  -138,     3,
    -138,  -138,    -4,  -138,   -98,    63,    85,  -138,     0,   -90,
    -138
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -98
static const yytype_int16 yytable[] =
{
      49,   134,   157,   104,    52,    57,    59,    61,   152,   136,
       6,    75,    76,   155,   155,    62,    99,   -97,    77,    68,
      68,   126,   -97,    49,    68,    53,    54,   126,   105,    77,
     100,   127,   179,   128,   134,    77,    64,   127,    67,    69,
      80,   152,   136,    73,   155,    63,    78,    79,    77,   188,
      91,    80,    94,    81,    66,   134,     1,    80,    70,   101,
      71,   155,   103,   136,     6,    78,    79,   134,   103,    92,
      80,   158,   106,   107,   174,   136,     6,   109,     1,    82,
      83,    93,   174,   108,   184,    72,     6,   144,   151,   125,
     145,   122,    95,   184,   120,   131,    55,    56,   120,   135,
     137,   180,   120,   120,   110,   110,   110,   110,   110,   110,
     110,   110,   110,    84,    85,    74,     2,     3,     4,     5,
       6,    96,   146,    60,     8,     9,    97,   148,   131,    93,
     149,   120,   135,    98,   150,   120,   103,   162,    94,    14,
     149,   160,   168,   159,    86,    87,   120,    16,    17,   131,
      88,   138,   120,   135,    49,   178,   120,   139,    89,    90,
     142,   143,     1,   175,   140,   135,   103,    49,   120,   141,
       6,   175,   176,   175,   111,   112,    91,   153,   185,     1,
     113,   114,   175,     2,     3,     4,     5,     6,   161,   166,
       7,     8,     9,   115,   116,   117,   118,   119,   163,    10,
     169,    11,   170,    12,    13,   171,    14,    15,   186,   165,
     182,    65,   164,   147,    16,    17,    18,    19,    20,     1,
       0,     0,    21,     2,     3,     4,     5,     6,     0,   177,
       7,     8,     9,     0,     0,     0,     0,     0,     0,    10,
       0,    11,     0,    12,    13,     0,    14,    15,     0,     0,
       0,     0,     0,     0,    16,    17,    18,    19,    20,     1,
       0,     0,    21,     2,     3,     4,     5,     6,     0,     0,
       7,     8,     9,     0,     0,     0,     0,     0,     0,    10,
       0,    11,     0,    12,    13,     0,    14,    15,     0,     0,
       0,     0,     0,     0,    16,    17,    18,    19,    20,     0,
       0,     0,    21,     2,     3,     4,     5,     6,   154,     0,
       7,     8,     9,     0,     0,     0,     0,     0,     0,    10,
       0,    11,     0,     0,    13,     0,    14,    15,     0,     0,
       0,     0,     0,     0,    16,    17,    18,    19,    20,     0,
       0,     0,    21,     2,     3,     4,     5,     6,     0,   172,
       7,     8,     9,     0,     0,     0,     0,     0,     0,    10,
       0,    11,     0,     0,    13,     0,    14,    15,     0,     0,
       0,     0,     0,     0,    16,    17,    18,    19,    20,     0,
       0,     0,    21,     2,     3,     4,     5,     6,     0,   181,
       7,     8,     9,     0,     0,     0,     0,     0,     0,    10,
       0,    11,     0,     0,    13,     0,    14,    15,     0,     0,
       0,     0,     0,     0,    16,    17,    18,    19,    20,     0,
       0,     0,    21,     2,     3,     4,     5,     6,     0,   183,
       7,     8,     9,     0,     0,     0,     0,     0,     0,    10,
       0,    11,     0,     0,    13,     0,    14,    15,     0,     0,
       0,     0,     0,     0,    16,    17,    18,    19,    20,     0,
       0,     0,    21,     2,     3,     4,     5,     6,     0,   187,
       7,     8,     9,     0,     0,     0,     0,     0,     0,    10,
       0,    11,     0,     0,    13,     0,    14,    15,     0,     0,
       0,     0,     0,     0,    16,    17,    18,    19,    20,     0,
       0,     0,    21,     2,     3,     4,     5,     6,     0,     0,
       0,     8,     9,   123,     2,     3,     4,     5,     6,     0,
       0,     0,     8,     9,     0,     0,    14,     0,     0,     0,
       0,     0,     0,     0,    16,    17,     0,    14,     0,     2,
       3,     4,     5,     6,     0,    16,    17,     8,     9,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    14
};

static const yytype_int16 yycheck[] =
{
       0,    99,   139,    72,     1,     9,    10,    11,   132,    99,
      11,    23,     4,   138,   139,    12,    12,    11,    21,    16,
      17,     4,    16,    23,    21,     9,    10,     4,    14,    21,
      26,    14,   169,    16,   132,    21,    14,    14,    16,    17,
      43,   165,   132,    21,   169,    16,    38,    39,    21,   186,
      44,    43,    49,    45,    16,   153,     3,    43,    14,    63,
      14,   186,    66,   153,    11,    38,    39,   165,    72,    14,
      43,   140,    76,    77,   163,   165,    11,    81,     3,     5,
       6,    16,   171,    80,   173,    16,    11,    17,    13,    93,
      20,    91,    16,   182,    91,    95,     9,    10,    95,    99,
     100,   170,    99,   100,    82,    83,    84,    85,    86,    87,
      88,    89,    90,     7,     8,     0,     7,     8,     9,    10,
      11,    17,   126,    14,    15,    16,    14,    17,   128,    16,
      20,   128,   132,    14,   131,   132,   140,    17,   135,    30,
      20,   145,   154,   143,    18,    19,   143,    38,    39,   149,
      24,    17,   149,   153,   154,   167,   153,    17,    32,    33,
      19,    20,     3,   163,    14,   165,   170,   167,   165,    22,
      11,   171,    13,   173,    82,    83,    44,    12,   175,     3,
      84,    85,   182,     7,     8,     9,    10,    11,    14,    13,
      14,    15,    16,    86,    87,    88,    89,    90,    12,    23,
      29,    25,    14,    27,    28,    12,    30,    31,    17,   153,
     171,    14,   149,   128,    38,    39,    40,    41,    42,     3,
      -1,    -1,    46,     7,     8,     9,    10,    11,    -1,    13,
      14,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    25,    -1,    27,    28,    -1,    30,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    40,    41,    42,     3,
      -1,    -1,    46,     7,     8,     9,    10,    11,    -1,    -1,
      14,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    25,    -1,    27,    28,    -1,    30,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    40,    41,    42,    -1,
      -1,    -1,    46,     7,     8,     9,    10,    11,    12,    -1,
      14,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    25,    -1,    -1,    28,    -1,    30,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    40,    41,    42,    -1,
      -1,    -1,    46,     7,     8,     9,    10,    11,    -1,    13,
      14,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    25,    -1,    -1,    28,    -1,    30,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    40,    41,    42,    -1,
      -1,    -1,    46,     7,     8,     9,    10,    11,    -1,    13,
      14,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    25,    -1,    -1,    28,    -1,    30,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    40,    41,    42,    -1,
      -1,    -1,    46,     7,     8,     9,    10,    11,    -1,    13,
      14,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    25,    -1,    -1,    28,    -1,    30,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    40,    41,    42,    -1,
      -1,    -1,    46,     7,     8,     9,    10,    11,    -1,    13,
      14,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    25,    -1,    -1,    28,    -1,    30,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    40,    41,    42,    -1,
      -1,    -1,    46,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    16,    17,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    16,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    -1,    30,    -1,     7,
       8,     9,    10,    11,    -1,    38,    39,    15,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     7,     8,     9,    10,    11,    14,    15,    16,
      23,    25,    27,    28,    30,    31,    38,    39,    40,    41,
      42,    46,    49,    50,    54,    56,    57,    59,    60,    61,
      63,    64,    65,    66,    67,    68,    69,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    86,
      87,    88,    77,     9,    10,     9,    10,    80,    86,    80,
      14,    80,    77,    16,    71,    72,    16,    71,    77,    71,
      14,    14,    16,    71,     0,    54,     4,    21,    38,    39,
      43,    45,     5,     6,     7,     8,    18,    19,    24,    32,
      33,    44,    14,    16,    77,    16,    17,    14,    14,    12,
      26,    80,    62,    80,    62,    14,    80,    80,    77,    80,
      71,    73,    73,    74,    74,    75,    75,    75,    75,    75,
      77,    85,    86,    17,    70,    80,     4,    14,    16,    83,
      84,    86,    52,    55,    82,    86,    87,    86,    17,    17,
      14,    22,    19,    20,    17,    20,    80,    84,    17,    20,
      77,    13,    55,    12,    12,    57,    58,    58,    62,    86,
      80,    14,    17,    12,    83,    52,    13,    53,    54,    29,
      14,    12,    13,    51,    56,    86,    13,    13,    54,    58,
      62,    13,    51,    13,    56,    77,    17,    13,    58
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (&yylloc, arena, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, &yylloc, theScanner)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location, arena); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, Arena* arena)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, arena)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    Arena* arena;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
  YYUSE (arena);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, Arena* arena)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, arena)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    Arena* arena;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, arena);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, Arena* arena)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, arena)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    Arena* arena;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       , arena);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule, arena); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, Arena* arena)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, arena)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    Arena* arena;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (arena);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (Arena* arena);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */





/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (Arena* arena)
#else
int
yyparse (arena)
    Arena* arena;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[2];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 78 "parser.y"
    { arena->SetResult(0); ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 79 "parser.y"
    { arena->SetResult((yyvsp[(1) - (1)].statementList)); ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 83 "parser.y"
    { (yyval.statementList) = A(StatementList()); (yyval.statementList)->push_back((yyvsp[(1) - (1)].statementNode)); DBG((yyval.statementList), (yylsp[(1) - (1)]), (yylsp[(1) - (1)])); ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 84 "parser.y"
    { (yyval.statementList) = (yyvsp[(1) - (2)].statementList); (yyval.statementList)->push_back((yyvsp[(2) - (2)].statementNode)); DBG((yyval.statementList), (yylsp[(1) - (2)]), (yylsp[(2) - (2)])); ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 88 "parser.y"
    { (yyval.statementList) = A(StatementList()); (yyval.statementList)->push_back((yyvsp[(1) - (1)].statementNode)); DBG((yyval.statementList), (yylsp[(1) - (1)]), (yylsp[(1) - (1)])); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 89 "parser.y"
    { (yyval.statementList) = (yyvsp[(1) - (2)].statementList); (yyval.statementList)->push_back((yyvsp[(2) - (2)].statementNode)); DBG((yyval.statementList), (yylsp[(1) - (2)]), (yylsp[(2) - (2)])); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 93 "parser.y"
    { (yyval.statementList) = A(StatementList()); (yyval.statementList)->push_back((yyvsp[(1) - (1)].statementNode)); DBG((yyval.statementList), (yylsp[(1) - (1)]), (yylsp[(1) - (1)])); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 94 "parser.y"
    { (yyval.statementList) = (yyvsp[(1) - (2)].statementList); (yyval.statementList)->push_back((yyvsp[(2) - (2)].statementNode)); DBG((yyval.statementList), (yylsp[(1) - (2)]), (yylsp[(2) - (2)])); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 98 "parser.y"
    { (yyval.statementList) = A(StatementList()); (yyval.statementList)->push_back((yyvsp[(1) - (1)].statementNode)); DBG((yyval.statementList), (yylsp[(1) - (1)]), (yylsp[(1) - (1)])); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 99 "parser.y"
    { (yyval.statementList) = (yyvsp[(1) - (2)].statementList); (yyval.statementList)->push_back((yyvsp[(2) - (2)].statementNode)); DBG((yyval.statementList), (yylsp[(1) - (2)]), (yylsp[(2) - (2)])); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 132 "parser.y"
    { (yyval.statementList) = A(StatementList()); (yyval.statementList)->push_back((yyvsp[(1) - (1)].statementNode)); DBG((yyval.statementList), (yylsp[(1) - (1)]), (yylsp[(1) - (1)])); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 133 "parser.y"
    { (yyval.statementList) = A(StatementList()); DBG((yyval.statementList), (yylsp[(1) - (2)]), (yylsp[(1) - (2)])); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 134 "parser.y"
    { (yyval.statementList) = (yyvsp[(2) - (3)].statementList); DBG((yyval.statementList), (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 138 "parser.y"
    { (yyval.statementNode) = A(ContinueStatement()); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 142 "parser.y"
    { (yyval.statementNode) = A(BreakStatement()); ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 146 "parser.y"
    { (yyval.statementNode) = A(IfStatement((yyvsp[(3) - (7)].arenaNode), (yyvsp[(5) - (7)].statementList), (yyvsp[(7) - (7)].statementList))); DBG((yyval.statementNode), (yylsp[(1) - (7)]), (yylsp[(7) - (7)])); ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 147 "parser.y"
    { (yyval.statementNode) = A(IfStatement((yyvsp[(3) - (5)].arenaNode), (yyvsp[(5) - (5)].statementList), 0)); DBG((yyval.statementNode), (yylsp[(1) - (5)]), (yylsp[(5) - (5)])); ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 151 "parser.y"
    { (yyval.arenaNode) = 0; ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 156 "parser.y"
    { (yyval.statementNode) = A(WhileStatement((yyvsp[(3) - (5)].arenaNode), (yyvsp[(5) - (5)].statementList))); DBG((yyval.statementNode), (yylsp[(1) - (5)]), (yylsp[(5) - (5)])); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 160 "parser.y"
    { (yyval.statementNode) = A(ForStatement((yyvsp[(3) - (9)].arenaNode), (yyvsp[(5) - (9)].arenaNode), (yyvsp[(7) - (9)].arenaNode), (yyvsp[(9) - (9)].statementList))); DBG((yyval.statementNode), (yylsp[(1) - (9)]), (yylsp[(9) - (9)])); ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 164 "parser.y"
    { (yyval.statementNode) = A(DebugStatement((yyvsp[(2) - (3)].arenaNode))); DBG((yyval.statementNode), (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 168 "parser.y"
    { (yyval.statementNode) = A(ReadStatement((yyvsp[(2) - (3)].arenaNode))); DBG((yyval.statementNode), (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 172 "parser.y"
    { (yyval.statementNode) = A(StructNode((yyvsp[(2) - (5)].identifierNode), 0, (yyvsp[(4) - (5)].statementList))); DBG((yyval.statementNode), (yylsp[(1) - (5)]), (yylsp[(5) - (5)])); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 173 "parser.y"
    { (yyval.statementNode) = A(StructNode((yyvsp[(2) - (7)].identifierNode), (yyvsp[(4) - (7)].typeNode), (yyvsp[(6) - (7)].statementList))); DBG((yyval.statementNode), (yylsp[(1) - (7)]), (yylsp[(7) - (7)])); ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 177 "parser.y"
    { (yyval.statementNode) = 0; ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 182 "parser.y"
    { (yyval.arenaNode) = A(AssignNode((yyvsp[(1) - (3)].arenaNode), (yyvsp[(3) - (3)].arenaNode))); DBG((yyval.arenaNode), (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 183 "parser.y"
    { (yyval.arenaNode) = A(AssignOpNode(assign_op_plus_equal, (yyvsp[(1) - (3)].arenaNode), (yyvsp[(3) - (3)].arenaNode))); DBG((yyval.arenaNode), (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 187 "parser.y"
    { (yyval.nodeList) = A(NodeList()); (yyval.nodeList)->push_back((yyvsp[(1) - (1)].arenaNode)); DBG((yyval.nodeList), (yylsp[(1) - (1)]), (yylsp[(1) - (1)])); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 188 "parser.y"
    { (yyval.nodeList) = (yyvsp[(1) - (3)].nodeList); (yyval.nodeList)->push_back((yyvsp[(3) - (3)].arenaNode)); DBG((yyval.nodeList), (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 192 "parser.y"
    { (yyval.arenaNode) = (yyvsp[(1) - (1)].identifierNode) ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 193 "parser.y"
    { (yyval.arenaNode) = A(AccessorNode((yyvsp[(1) - (4)].arenaNode), (yyvsp[(3) - (4)].arenaNode))); DBG((yyval.arenaNode), (yylsp[(1) - (4)]), (yylsp[(4) - (4)])); ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 194 "parser.y"
    { (yyval.arenaNode) = A(DotNode((yyvsp[(1) - (3)].arenaNode), (yyvsp[(3) - (3)].identifierNode))); DBG((yyval.arenaNode), (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 199 "parser.y"
    { (yyval.arenaNode) = A(UnaryOpNode(unary_op_not, (yyvsp[(2) - (2)].arenaNode))); DBG((yyval.arenaNode), (yylsp[(1) - (2)]), (yylsp[(2) - (2)])); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 204 "parser.y"
    { (yyval.arenaNode) = A(AssignOpNode(assign_op_plusplus_prefix, (yyvsp[(2) - (2)].arenaNode))); DBG((yyval.arenaNode), (yylsp[(1) - (2)]), (yylsp[(2) - (2)])); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 205 "parser.y"
    { (yyval.arenaNode) = A(AssignOpNode(assign_op_plusplus_sufix, (yyvsp[(1) - (2)].arenaNode))); DBG((yyval.arenaNode), (yylsp[(1) - (2)]), (yylsp[(2) - (2)])); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 206 "parser.y"
    { (yyval.arenaNode) = A(AssignOpNode(assign_op_minusminus_prefix, (yyvsp[(2) - (2)].arenaNode))); DBG((yyval.arenaNode), (yylsp[(1) - (2)]), (yylsp[(2) - (2)])); ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 207 "parser.y"
    { (yyval.arenaNode) = A(AssignOpNode(assign_op_minusminus_sufix, (yyvsp[(1) - (2)].arenaNode))); DBG((yyval.arenaNode), (yylsp[(1) - (2)]), (yylsp[(2) - (2)])); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 212 "parser.y"
    { (yyval.arenaNode) = A(BinaryOpNode(binary_op_multiply, (yyvsp[(1) - (3)].arenaNode), (yyvsp[(3) - (3)].arenaNode))); DBG((yyval.arenaNode), (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 213 "parser.y"
    { (yyval.arenaNode) = A(BinaryOpNode(binary_op_divide, (yyvsp[(1) - (3)].arenaNode), (yyvsp[(3) - (3)].arenaNode))); DBG((yyval.arenaNode), (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 218 "parser.y"
    { (yyval.arenaNode) = A(BinaryOpNode(binary_op_plus, (yyvsp[(1) - (3)].arenaNode), (yyvsp[(3) - (3)].arenaNode))); DBG((yyval.arenaNode), (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 219 "parser.y"
    { (yyval.arenaNode) = A(BinaryOpNode(binary_op_minus, (yyvsp[(1) - (3)].arenaNode), (yyvsp[(3) - (3)].arenaNode))); DBG((yyval.arenaNode), (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 224 "parser.y"
    { (yyval.arenaNode) = A(BinaryOpNode(binary_op_less, (yyvsp[(1) - (3)].arenaNode), (yyvsp[(3) - (3)].arenaNode))); DBG((yyval.arenaNode), (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 225 "parser.y"
    { (yyval.arenaNode) = A(BinaryOpNode(binary_op_more, (yyvsp[(1) - (3)].arenaNode), (yyvsp[(3) - (3)].arenaNode))); DBG((yyval.arenaNode), (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 226 "parser.y"
    { (yyval.arenaNode) = A(BinaryOpNode(binary_op_equal, (yyvsp[(1) - (3)].arenaNode), (yyvsp[(3) - (3)].arenaNode))); DBG((yyval.arenaNode), (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 227 "parser.y"
    { (yyval.arenaNode) = A(BinaryOpNode(binary_op_less_or_equal, (yyvsp[(1) - (3)].arenaNode), (yyvsp[(3) - (3)].arenaNode))); DBG((yyval.arenaNode), (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 228 "parser.y"
    { (yyval.arenaNode) = A(BinaryOpNode(binary_op_more_or_equal, (yyvsp[(1) - (3)].arenaNode), (yyvsp[(3) - (3)].arenaNode))); DBG((yyval.arenaNode), (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 232 "parser.y"
    { (yyval.identifierNode) = A(IdentifierNode(yytext)); DBG((yyval.identifierNode), (yylsp[(1) - (1)]), (yylsp[(1) - (1)])); ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 236 "parser.y"
    { (yyval.arenaNode) = A(IntegerValueNode(atoi(yytext))); DBG((yyval.arenaNode), (yylsp[(1) - (1)]), (yylsp[(1) - (1)])); ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 237 "parser.y"
    { (yyval.arenaNode) = A(FloatValueNode(atof(yytext))); DBG((yyval.arenaNode), (yylsp[(1) - (1)]), (yylsp[(1) - (1)])); ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 238 "parser.y"
    { (yyval.arenaNode) = A(IntegerValueNode(-atoi(yytext))); DBG((yyval.arenaNode), (yylsp[(1) - (2)]), (yylsp[(1) - (2)])); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 239 "parser.y"
    { (yyval.arenaNode) = A(FloatValueNode(-atof(yytext))); DBG((yyval.arenaNode), (yylsp[(1) - (2)]), (yylsp[(1) - (2)])); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 240 "parser.y"
    { (yyval.arenaNode) = A(IntegerValueNode(atoi(yytext))); DBG((yyval.arenaNode), (yylsp[(1) - (2)]), (yylsp[(1) - (2)])); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 241 "parser.y"
    { (yyval.arenaNode) = A(FloatValueNode(atof(yytext))); DBG((yyval.arenaNode), (yylsp[(1) - (2)]), (yylsp[(1) - (2)])); ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 242 "parser.y"
    { (yyval.arenaNode) = A(StringValueNode(yytext)); DBG((yyval.arenaNode), (yylsp[(1) - (1)]), (yylsp[(1) - (1)])); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 243 "parser.y"
    { (yyval.arenaNode) = (yyvsp[(2) - (3)].arenaNode); DBG((yyval.arenaNode), (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 245 "parser.y"
    { (yyval.arenaNode) = (yyvsp[(1) - (1)].callNode); DBG((yyval.arenaNode), (yylsp[(1) - (1)]), (yylsp[(1) - (1)])); ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 249 "parser.y"
    { (yyval.callNode) = A(CallNode((yyvsp[(1) - (3)].typeNode), 0)); DBG((yyval.callNode), (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 250 "parser.y"
    { (yyval.callNode) = A(CallNode((yyvsp[(1) - (4)].typeNode), (yyvsp[(3) - (4)].nodeList))); DBG((yyval.callNode), (yylsp[(1) - (4)]), (yylsp[(4) - (4)])); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 258 "parser.y"
    { (yyval.statementNode) = A(ExpressionStatement((yyvsp[(1) - (2)].arenaNode))); DBG((yyval.statementNode), (yylsp[(1) - (2)]), (yylsp[(2) - (2)])); ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 262 "parser.y"
    { (yyval.statementNode) = A(MethodNode((yyvsp[(1) - (7)].typeNode), (yyvsp[(2) - (7)].identifierNode), (yyvsp[(4) - (7)].argumentNodeList), 0 )); DBG((yyval.statementNode), (yylsp[(1) - (7)]), (yylsp[(7) - (7)])); ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 263 "parser.y"
    { (yyval.statementNode) = A(MethodNode((yyvsp[(1) - (8)].typeNode), (yyvsp[(2) - (8)].identifierNode), (yyvsp[(4) - (8)].argumentNodeList), (yyvsp[(7) - (8)].statementList) )); DBG((yyval.statementNode), (yylsp[(1) - (8)]), (yylsp[(8) - (8)])); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 264 "parser.y"
    { (yyval.statementNode) = A(MethodNode(0, (yyvsp[(2) - (7)].identifierNode), (yyvsp[(4) - (7)].argumentNodeList), 0 )); DBG((yyval.statementNode), (yylsp[(1) - (7)]), (yylsp[(7) - (7)])); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 265 "parser.y"
    { (yyval.statementNode) = A(MethodNode(0, (yyvsp[(2) - (8)].identifierNode), (yyvsp[(4) - (8)].argumentNodeList), (yyvsp[(7) - (8)].statementList) )); DBG((yyval.statementNode), (yylsp[(1) - (8)]), (yylsp[(8) - (8)])); ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 269 "parser.y"
    { (yyval.argumentNode) = A(ArgumentNode((yyvsp[(1) - (2)].typeNode), (yyvsp[(2) - (2)].identifierNode))); DBG((yyval.argumentNode), (yylsp[(1) - (2)]), (yylsp[(2) - (2)])); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 273 "parser.y"
    { (yyval.argumentNodeList) = 0; ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 274 "parser.y"
    { (yyval.argumentNodeList) = A(ArgumentNodeList()); (yyval.argumentNodeList)->push_back((yyvsp[(1) - (1)].argumentNode)); DBG((yyval.argumentNodeList), (yylsp[(1) - (1)]), (yylsp[(1) - (1)])); ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 275 "parser.y"
    { (yyval.argumentNodeList) = (yyvsp[(1) - (3)].argumentNodeList); (yyval.argumentNodeList)->push_back((yyvsp[(3) - (3)].argumentNode)); DBG((yyval.argumentNodeList), (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 280 "parser.y"
    { (yyval.typeNodeList) = A(TypeNodeList()); (yyval.typeNodeList)->push_back((yyvsp[(1) - (1)].typeNode)); DBG((yyval.typeNodeList), (yylsp[(1) - (1)]), (yylsp[(1) - (1)])); ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 281 "parser.y"
    { (yyval.typeNodeList) = (yyvsp[(1) - (3)].typeNodeList); (yyval.typeNodeList)->push_back((yyvsp[(3) - (3)].typeNode)); DBG((yyval.typeNodeList), (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 285 "parser.y"
    { (yyval.typeNode) = A(TypeNode((yyvsp[(1) - (1)].identifierNode), 0)); DBG((yyval.typeNode), (yylsp[(1) - (1)]), (yylsp[(1) - (1)])); ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 286 "parser.y"
    { (yyval.typeNode) = A(TypeNode((yyvsp[(1) - (4)].identifierNode), (yyvsp[(3) - (4)].typeNodeList))); DBG((yyval.typeNode), (yylsp[(1) - (4)]), (yylsp[(4) - (4)])); ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 290 "parser.y"
    { (yyval.statementNode) = A(VarStatement((yyvsp[(1) - (3)].typeNode), (yyvsp[(2) - (3)].identifierNode), 0)); DBG((yyval.statementNode), (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 291 "parser.y"
    { (yyval.statementNode) = A(VarStatement((yyvsp[(1) - (5)].typeNode), (yyvsp[(2) - (5)].identifierNode), (yyvsp[(4) - (5)].arenaNode))); DBG((yyval.statementNode), (yylsp[(1) - (5)]), (yylsp[(5) - (5)])); ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 295 "parser.y"
    { (yyval.statementNode) = A(ReturnStatement( 0 )); DBG((yyval.statementNode), (yylsp[(1) - (2)]), (yylsp[(2) - (2)])); ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 296 "parser.y"
    { (yyval.statementNode) = A(ReturnStatement( (yyvsp[(2) - (3)].arenaNode) )); DBG((yyval.statementNode), (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;



/* Line 1455 of yacc.c  */
#line 2248 "generated\\parser.y.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, arena, YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (&yylloc, arena, yymsg);
	  }
	else
	  {
	    yyerror (&yylloc, arena, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, &yylloc, arena);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[0] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp, arena);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, arena, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc, arena);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp, arena);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 299 "parser.y"


void yyerror(YYLTYPE* locp, Arena*, const char *str)
{
    fprintf(stderr, "error at line %d: %s\n", locp->first_line, str);
}

