/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "pascal.y"

// group: Travis Seville, Georgii Tkachuk
/*
 * grammar.y
 *
 * Pascal grammar in Yacc format, based originally on BNF given
 * in "Standard Pascal -- User Reference Manual", by Doug Cooper.
 * This in turn is the BNF given by the ANSI and ISO Pascal standards,
 * and so, is PUBLIC DOMAIN. The grammar is for ISO Level 0 Pascal.
 * The grammar has been massaged somewhat to make it LALR.
 */

#include "shared.h"
#include "rulefuncs.h"
#include "symtab.h"

int yylex(void);
void yyerror(const char *error);

extern char *yytext;		  /* yacc text variable */
extern int line_number;	   /* Holds the current line number; specified
				   in the lexer */

struct function_declaration_t *cur_function; 	/* points to the current function. Not sure if need this*/
extern struct identifier_list_t *global_scope;
extern struct identifier_list_t *cur_class_scopes;
/* points to the current class scope, starting with the current, each next is higher, up to global, this is held in symtab */
struct program_t *program;			/* points to our program */
extern int lookupTypeIndex(char * _name, struct identifier_list_t * _scope);
void dprint(char * str)
{
	if (DEBUG)
	{
		printf("\n\n-----------------%s---------------\n\n", str);
	}
}


/* Line 268 of yacc.c  */
#line 110 "y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
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
     AND = 258,
     ARRAY = 259,
     ASSIGNMENT = 260,
     CLASS = 261,
     COLON = 262,
     COMMA = 263,
     DIGSEQ = 264,
     DO = 265,
     DOT = 266,
     DOTDOT = 267,
     ELSE = 268,
     END = 269,
     EQUAL = 270,
     EXTENDS = 271,
     FUNCTION = 272,
     GE = 273,
     GT = 274,
     IDENTIFIER = 275,
     IF = 276,
     LBRAC = 277,
     LE = 278,
     LPAREN = 279,
     LT = 280,
     MINUS = 281,
     MOD = 282,
     NEW = 283,
     NOT = 284,
     NOTEQUAL = 285,
     OF = 286,
     OR = 287,
     PBEGIN = 288,
     PLUS = 289,
     PRINT = 290,
     PROGRAM = 291,
     RBRAC = 292,
     RPAREN = 293,
     SEMICOLON = 294,
     SLASH = 295,
     STAR = 296,
     THEN = 297,
     VAR = 298,
     WHILE = 299
   };
#endif
/* Tokens.  */
#define AND 258
#define ARRAY 259
#define ASSIGNMENT 260
#define CLASS 261
#define COLON 262
#define COMMA 263
#define DIGSEQ 264
#define DO 265
#define DOT 266
#define DOTDOT 267
#define ELSE 268
#define END 269
#define EQUAL 270
#define EXTENDS 271
#define FUNCTION 272
#define GE 273
#define GT 274
#define IDENTIFIER 275
#define IF 276
#define LBRAC 277
#define LE 278
#define LPAREN 279
#define LT 280
#define MINUS 281
#define MOD 282
#define NEW 283
#define NOT 284
#define NOTEQUAL 285
#define OF 286
#define OR 287
#define PBEGIN 288
#define PLUS 289
#define PRINT 290
#define PROGRAM 291
#define RBRAC 292
#define RPAREN 293
#define SEMICOLON 294
#define SLASH 295
#define STAR 296
#define THEN 297
#define VAR 298
#define WHILE 299




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 106 "pascal.y"

	char *id;	
	int	*i;	
	int	op;	
	struct	actual_parameter_list_t	*apl;
	struct	actual_parameter_t	*ap;
	struct	array_type_t	*at;
	struct	assignment_statement_t	*as;
	struct	attribute_designator_t	*ad;
	struct	class_block_t	*cb;
	struct	class_identification_t	*ci;
	struct	class_list_t	*cl;
	struct	expression_t	*e;
	struct	factor_t	*f;
	struct	formal_parameter_section_list_t	*fpsl;
	struct	formal_parameter_section_t	*fps;
	struct	func_declaration_list_t	*fdl;
	struct	function_block_t	*fb;
	struct	function_declaration_t	*funcd;
	struct	function_declaration_t	*cur_function;
	struct	function_declaration_t	*cur_class;
	struct	function_designator_t	*fdes;
	struct	function_heading_t	*fh;
	struct	identifier_list_t	*idl;
	struct	if_statement_t	*is;
	struct	index_expression_list_t	*iel;
	struct	indexed_variable_t	*iv;
	struct	method_designator_t	*md;
	struct	object_instantiation_t	*os;
	struct	primary_t	*p;
	struct	print_statement_t	*ps;
	struct	program_heading_t	*ph;
	struct	program_t	*program;
	struct	range_t	*r;
	struct	simple_expression_t	*se;
	struct	statement_sequence_t	*ss;
	struct	statement_t	*s;
	struct	term_t	*t;
	struct	type_denoter_t	*tden;
	struct	unsigned_number_t	*un;
	struct	variable_access_t	*va;
	struct	variable_declaration_list_t	*vdl;
	struct	variable_declaration_t	*vd;
	struct	while_statement_t	*ws;



/* Line 293 of yacc.c  */
#line 282 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 294 "y.tab.c"

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
# if defined YYENABLE_NLS && YYENABLE_NLS
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

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

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
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
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   172

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  59
/* YYNRULES -- Number of rules.  */
#define YYNRULES  105
/* YYNRULES -- Number of states.  */
#define YYNSTATES  182

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   299

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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     8,    11,    17,    21,    23,    29,    34,
      37,    42,    45,    47,    49,    56,    60,    64,    65,    69,
      71,    75,    79,    81,    82,    86,    90,    92,    94,    96,
     100,   105,   109,   113,   118,   124,   126,   129,   132,   134,
     138,   140,   144,   146,   148,   150,   152,   154,   159,   166,
     170,   174,   177,   181,   184,   186,   188,   190,   192,   197,
     201,   203,   205,   209,   213,   217,   221,   223,   225,   229,
     235,   237,   239,   243,   245,   249,   251,   255,   257,   259,
     262,   264,   266,   268,   270,   274,   277,   279,   281,   283,
     286,   288,   290,   292,   294,   296,   298,   300,   302,   304,
     306,   308,   310,   312,   314,   316
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      46,     0,    -1,    47,   102,    49,    11,    -1,    36,   101,
      -1,    36,   101,    24,    48,    38,    -1,    48,   103,   101,
      -1,   101,    -1,    49,    50,    33,    51,    14,    -1,    50,
      33,    51,    14,    -1,     6,   101,    -1,     6,   101,    16,
     101,    -1,    55,    58,    -1,    53,    -1,   101,    -1,     4,
      22,    54,    37,    31,    52,    -1,    96,    12,    96,    -1,
      43,    56,   102,    -1,    -1,    56,   102,    57,    -1,    57,
      -1,    48,     7,    52,    -1,    58,   102,    64,    -1,    64,
      -1,    -1,    24,    60,    38,    -1,    60,   102,    61,    -1,
      61,    -1,    62,    -1,    63,    -1,    48,     7,   101,    -1,
      43,    48,     7,   101,    -1,    67,   102,    68,    -1,    65,
     102,    68,    -1,    17,   101,     7,    66,    -1,    17,   101,
      59,     7,    66,    -1,   101,    -1,    17,   101,    -1,    55,
      69,    -1,    70,    -1,    33,    71,    14,    -1,    72,    -1,
      71,   102,    72,    -1,    75,    -1,    70,    -1,    74,    -1,
      73,    -1,    77,    -1,    44,    87,    10,    72,    -1,    21,
      87,    42,    72,    13,    72,    -1,    78,     5,    88,    -1,
      78,     5,    76,    -1,    28,   101,    -1,    28,   101,    84,
      -1,    35,    78,    -1,   101,    -1,    79,    -1,    82,    -1,
      83,    -1,    78,    22,    80,    37,    -1,    80,   103,    81,
      -1,    81,    -1,    88,    -1,    78,    11,   101,    -1,    78,
      11,    97,    -1,    24,    85,    38,    -1,    85,   103,    86,
      -1,    86,    -1,    88,    -1,    88,     7,    88,    -1,    88,
       7,    88,     7,    88,    -1,    88,    -1,    89,    -1,    89,
     100,    89,    -1,    90,    -1,    89,    98,    90,    -1,    92,
      -1,    90,    99,    92,    -1,    34,    -1,    26,    -1,    91,
      92,    -1,    93,    -1,    78,    -1,    94,    -1,    97,    -1,
      24,    88,    38,    -1,    29,    93,    -1,    95,    -1,    96,
      -1,     9,    -1,   101,    84,    -1,    34,    -1,    26,    -1,
      32,    -1,    41,    -1,    40,    -1,    27,    -1,     3,    -1,
      15,    -1,    30,    -1,    25,    -1,    19,    -1,    23,    -1,
      18,    -1,    20,    -1,    39,    -1,     8,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   154,   154,   165,   171,   180,   187,   195,   205,   216,
     235,   264,   274,   282,   292,   301,   310,   316,   322,   329,
     338,   359,   366,   373,   379,   385,   392,   400,   405,   411,
     422,   433,   445,   455,   462,   472,   475,   483,   493,   496,
     502,   508,   517,   526,   534,   542,   550,   560,   569,   579,
     606,   615,   621,   630,   638,   657,   668,   678,   690,   703,
     712,   722,   724,   733,   742,   750,   757,   765,   771,   778,
     788,   790,   798,   810,   818,   830,   838,   850,   855,   861,
     871,   882,   895,   904,   914,   923,   935,   938,   940,   953,
     963,   967,   971,   977,   981,   985,   989,   995,   999,  1003,
    1007,  1011,  1015,  1021,  1027,  1030
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "AND", "ARRAY", "ASSIGNMENT", "CLASS",
  "COLON", "COMMA", "DIGSEQ", "DO", "DOT", "DOTDOT", "ELSE", "END",
  "EQUAL", "EXTENDS", "FUNCTION", "GE", "GT", "IDENTIFIER", "IF", "LBRAC",
  "LE", "LPAREN", "LT", "MINUS", "MOD", "NEW", "NOT", "NOTEQUAL", "OF",
  "OR", "PBEGIN", "PLUS", "PRINT", "PROGRAM", "RBRAC", "RPAREN",
  "SEMICOLON", "SLASH", "STAR", "THEN", "VAR", "WHILE", "$accept",
  "program", "program_heading", "identifier_list", "class_list",
  "class_identification", "class_block", "type_denoter", "array_type",
  "range", "variable_declaration_part", "variable_declaration_list",
  "variable_declaration", "func_declaration_list", "formal_parameter_list",
  "formal_parameter_section_list", "formal_parameter_section",
  "value_parameter_specification", "variable_parameter_specification",
  "function_declaration", "function_heading", "result_type",
  "function_identification", "function_block", "statement_part",
  "compound_statement", "statement_sequence", "statement",
  "while_statement", "if_statement", "assignment_statement",
  "object_instantiation", "print_statement", "variable_access",
  "indexed_variable", "index_expression_list", "index_expression",
  "attribute_designator", "method_designator", "params",
  "actual_parameter_list", "actual_parameter", "boolean_expression",
  "expression", "simple_expression", "term", "sign", "factor", "primary",
  "unsigned_constant", "unsigned_number", "unsigned_integer",
  "function_designator", "addop", "mulop", "relop", "identifier",
  "semicolon", "comma", 0
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
     295,   296,   297,   298,   299
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    45,    46,    47,    47,    48,    48,    49,    49,    50,
      50,    51,    52,    52,    53,    54,    55,    55,    56,    56,
      57,    58,    58,    58,    59,    60,    60,    61,    61,    62,
      63,    64,    64,    65,    65,    66,    67,    68,    69,    70,
      71,    71,    72,    72,    72,    72,    72,    73,    74,    75,
      75,    76,    76,    77,    78,    78,    78,    78,    79,    80,
      80,    81,    82,    83,    84,    85,    85,    86,    86,    86,
      87,    88,    88,    89,    89,    90,    90,    91,    91,    92,
      92,    93,    93,    93,    93,    93,    94,    95,    96,    97,
      98,    98,    98,    99,    99,    99,    99,   100,   100,   100,
     100,   100,   100,   101,   102,   103
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     2,     5,     3,     1,     5,     4,     2,
       4,     2,     1,     1,     6,     3,     3,     0,     3,     1,
       3,     3,     1,     0,     3,     3,     1,     1,     1,     3,
       4,     3,     3,     4,     5,     1,     2,     2,     1,     3,
       1,     3,     1,     1,     1,     1,     1,     4,     6,     3,
       3,     2,     3,     2,     1,     1,     1,     1,     4,     3,
       1,     1,     3,     3,     3,     3,     1,     1,     3,     5,
       1,     1,     3,     1,     3,     1,     3,     1,     1,     2,
       1,     1,     1,     1,     3,     2,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,   103,     3,     1,   104,     0,     0,
       0,     0,     0,     0,     6,     9,     2,     0,    17,   105,
       4,     0,     0,    17,     0,     0,    23,     5,    10,     0,
       0,     0,    19,     8,     0,    11,    22,     0,     0,     7,
       0,    16,    36,     0,    17,    17,     0,    20,    12,    13,
      18,     0,     0,     0,    21,     0,    32,    31,     0,    33,
      35,     0,     0,     0,    26,    27,    28,     0,     0,    37,
      38,    88,     0,     0,     0,     0,    24,     0,    34,     0,
       0,     0,    43,     0,    40,    45,    44,    42,    46,     0,
      55,    56,    57,    54,     0,     0,     0,    29,    25,     0,
      78,     0,    77,    81,     0,    70,    71,    73,     0,    75,
      80,    82,    86,    87,    83,    54,    53,     0,    39,     0,
       0,     0,     0,     0,    15,    30,     0,    85,     0,    97,
     102,   100,   101,    99,    91,    98,    92,    90,     0,     0,
      96,    95,    94,    93,     0,    79,     0,    89,     0,    41,
       0,    50,    49,    63,    62,     0,    60,    61,    14,    84,
       0,    74,    72,    76,     0,    66,    67,    47,    51,    58,
       0,     0,    64,     0,     0,    52,    59,    48,    65,    68,
       0,    69
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,    30,    11,    12,    25,    47,    48,    72,
      26,    31,    32,    35,    53,    63,    64,    65,    66,    36,
      37,    59,    38,    56,    69,    82,    83,    84,    85,    86,
      87,   151,    88,   103,    90,   155,   156,    91,    92,   147,
     164,   165,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   138,   144,   139,   115,     8,    21
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -101
static const yytype_int16 yypact[] =
{
     -14,     9,    36,     7,  -101,    33,  -101,  -101,    53,     9,
       9,    82,    47,    -3,  -101,    68,  -101,    54,    48,  -101,
    -101,     9,     9,    48,     9,    85,    92,  -101,  -101,   105,
      89,     7,  -101,  -101,     9,     7,  -101,     7,     7,  -101,
      11,     9,     0,    92,    48,    48,    84,  -101,  -101,  -101,
    -101,     9,   -17,   116,  -101,    88,  -101,  -101,   119,  -101,
    -101,     9,   106,    78,  -101,  -101,  -101,     9,    91,  -101,
    -101,  -101,    99,   113,   123,     9,  -101,   -17,  -101,    81,
       9,    81,  -101,    -2,  -101,  -101,  -101,  -101,  -101,    67,
    -101,  -101,  -101,  -101,   103,   119,     9,  -101,  -101,    81,
    -101,    44,  -101,    27,    96,  -101,   114,     3,    81,  -101,
    -101,  -101,  -101,  -101,  -101,   121,    27,   133,  -101,    91,
      74,     9,    81,    11,  -101,  -101,   112,  -101,    91,  -101,
    -101,  -101,  -101,  -101,  -101,  -101,  -101,  -101,    81,    81,
    -101,  -101,  -101,  -101,    81,  -101,    81,  -101,    91,  -101,
       9,  -101,  -101,  -101,   121,    10,  -101,  -101,  -101,  -101,
     138,     3,    43,  -101,    17,  -101,   145,  -101,   121,  -101,
      81,    91,  -101,    81,    81,  -101,  -101,  -101,  -101,   146,
      81,  -101
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -101,  -101,  -101,    -7,  -101,   143,   132,    34,  -101,  -101,
      97,  -101,   115,  -101,  -101,  -101,    83,  -101,  -101,   118,
    -101,    95,  -101,   120,  -101,   104,  -101,  -100,  -101,  -101,
    -101,  -101,  -101,   -67,  -101,  -101,   -12,  -101,  -101,    -5,
    -101,    -9,    86,   -88,    29,    28,  -101,   -81,    70,  -101,
    -101,   -54,    51,  -101,  -101,  -101,    -1,   -21,   -99
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
       5,    89,    13,     4,    73,    19,   140,    51,    14,    15,
      41,   126,   118,   116,    43,    46,    44,    45,    19,   149,
      27,    28,     1,    14,    52,    19,    61,   145,   160,     4,
     141,     4,   152,    42,   157,    20,     6,     7,   121,    49,
      14,   124,    77,   142,   143,    62,     7,   169,   167,   122,
      60,    14,    89,    71,    74,   172,   170,     9,   166,    10,
      14,    89,   119,   163,     4,   173,    60,    93,    99,   134,
      62,   177,   120,   101,    97,   136,    14,   137,   121,    93,
      18,    89,   157,    71,    22,   166,   179,    23,    10,   122,
      71,    24,   181,    16,     4,   125,    40,    19,    99,    33,
     100,     4,   150,   101,    89,    99,    58,   100,   102,    34,
     101,     4,    79,    75,    19,   102,    76,     7,    93,    39,
     154,    68,    49,    67,    68,    95,    80,    93,    71,   129,
      96,    19,   130,   131,   123,    81,    94,   132,   128,   133,
     134,    55,    55,   148,   135,   146,   136,    93,   137,   168,
     159,   171,   174,   180,    17,    29,    50,   158,   176,    70,
      98,    54,    78,   175,   178,    57,   161,   117,   162,     0,
      93,   127,   153
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-101))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       1,    68,     9,    20,    58,     8,     3,     7,     9,    10,
      31,    99,    14,    80,    35,     4,    37,    38,     8,   119,
      21,    22,    36,    24,    24,     8,    43,   108,   128,    20,
      27,    20,   120,    34,   122,    38,     0,    39,    11,    40,
      41,    95,    63,    40,    41,    52,    39,    37,   148,    22,
      51,    52,   119,     9,    61,    38,   155,    24,   146,     6,
      61,   128,    83,   144,    20,   164,    67,    68,    24,    26,
      77,   171,     5,    29,    75,    32,    77,    34,    11,    80,
      33,   148,   170,     9,    16,   173,   174,    33,     6,    22,
       9,    43,   180,    11,    20,    96,     7,     8,    24,    14,
      26,    20,    28,    29,   171,    24,    22,    26,    34,    17,
      29,    20,    21,     7,     8,    34,    38,    39,   119,    14,
     121,    33,   123,     7,    33,    12,    35,   128,     9,    15,
       7,     8,    18,    19,    31,    44,    37,    23,    42,    25,
      26,    44,    45,    10,    30,    24,    32,   148,    34,   150,
      38,    13,     7,     7,    11,    23,    41,   123,   170,    55,
      77,    43,    67,   168,   173,    45,   138,    81,   139,    -1,
     171,   101,   121
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    36,    46,    47,    20,   101,     0,    39,   102,    24,
       6,    49,    50,    48,   101,   101,    11,    50,    33,     8,
      38,   103,    16,    33,    43,    51,    55,   101,   101,    51,
      48,    56,    57,    14,    17,    58,    64,    65,    67,    14,
       7,   102,   101,   102,   102,   102,     4,    52,    53,   101,
      57,     7,    24,    59,    64,    55,    68,    68,    22,    66,
     101,    43,    48,    60,    61,    62,    63,     7,    33,    69,
      70,     9,    54,    96,    48,     7,    38,   102,    66,    21,
      35,    44,    70,    71,    72,    73,    74,    75,    77,    78,
      79,    82,    83,   101,    37,    12,     7,   101,    61,    24,
      26,    29,    34,    78,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,   101,    78,    87,    14,   102,
       5,    11,    22,    31,    96,   101,    88,    93,    42,    15,
      18,    19,    23,    25,    26,    30,    32,    34,    98,   100,
       3,    27,    40,    41,    99,    92,    24,    84,    10,    72,
      28,    76,    88,    97,   101,    80,    81,    88,    52,    38,
      72,    90,    89,    92,    85,    86,    88,    72,   101,    37,
     103,    13,    38,   103,     7,    84,    81,    72,    86,    88,
       7,    88
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
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
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


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
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
		  Type, Value); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
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
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
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
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

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
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

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
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

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

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
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

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

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
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

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
  if (yypact_value_is_default (yyn))
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
      if (yytable_value_is_error (yyn))
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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1806 of yacc.c  */
#line 155 "pascal.y"
    {
		//This was already created as global in the first section. We just need to allocate.
		//struct program_t * program = new_program();
		program = new_program();
		(yyval.program) = program;
		(yyval.program)->ph = (yyvsp[(1) - (4)].ph);
		(yyval.program)->cl = (yyvsp[(3) - (4)].cl);
	}
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 166 "pascal.y"
    {
		struct program_heading_t * program_heading = new_program_heading();
		(yyval.ph) = program_heading;
		(yyval.ph)->id = (yyvsp[(2) - (2)].id);
	}
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 172 "pascal.y"
    {
		struct program_heading_t * program_heading = new_program_heading();
		(yyval.ph) = program_heading;
		(yyval.ph)->id = (yyvsp[(2) - (5)].id);
		(yyval.ph)->il = (yyvsp[(4) - (5)].idl);
	}
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 181 "pascal.y"
    {
		struct identifier_list_t * identifier_list = new_identifier_list();
		(yyval.idl) = identifier_list;
		(yyval.idl)->id = (yyvsp[(3) - (3)].id);
		(yyval.idl)->next = (yyvsp[(1) - (3)].idl);
	}
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 188 "pascal.y"
    {
		struct identifier_list_t * identifier_list = new_identifier_list();
		(yyval.idl) = identifier_list;
		(yyval.idl)->id = (yyvsp[(1) - (1)].id);
	}
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 196 "pascal.y"
    {
		struct class_list_t * class_list = new_class_list();
		(yyval.cl) = class_list;
		(yyval.cl)->ci = (yyvsp[(2) - (5)].ci);
		(yyval.cl)->cb = (yyvsp[(4) - (5)].cb);
		(yyval.cl)->next = (yyvsp[(1) - (5)].cl);
		// get rid of this class from current scopes
		cur_class_scopes = cur_class_scopes->next;
	}
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 206 "pascal.y"
    {
		struct class_list_t * class_list = new_class_list();
		(yyval.cl) = class_list;
		(yyval.cl)->ci = (yyvsp[(1) - (4)].ci);
		(yyval.cl)->cb = (yyvsp[(3) - (4)].cb);
		// get rid of this class from current scopes
		cur_class_scopes = cur_class_scopes->next;
	}
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 217 "pascal.y"
    {
		struct class_identification_t * class_identification = new_class_identification();
		(yyval.ci) = class_identification;
		(yyval.ci)->id = (yyvsp[(2) - (2)].id);
        (yyval.ci)->line_number = line_number;
		if (DEBUG) printf("\nIN THE BASE CLASS THING: %s\n",cur_class_scopes->id);
		struct symbol *s = addClass((yyval.ci)->id, line_number, NULL, cur_class_scopes);
		//set this cl as current scope. We can refer to this from other productions.
		if (s != NULL)
		{
			if (DEBUG) printf("\nADDING %s TO CUR CLASS SCOPES.\n", (yyvsp[(2) - (2)].id));
			add_to_identifier_list_front(&cur_class_scopes, (yyvsp[(2) - (2)].id));
		}
		else
		{
			add_to_identifier_list_front(&cur_class_scopes, "NOSCOPE");
		}
	}
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 236 "pascal.y"
    {
		struct class_identification_t * class_identification = new_class_identification();
		(yyval.ci) = class_identification;
		(yyval.ci)->id = (yyvsp[(2) - (4)].id);
		(yyval.ci)->extend = (yyvsp[(4) - (4)].id);
        (yyval.ci)->line_number = line_number;
		if (DEBUG) printf("\nIN THE extend CLASS THING: %s\nLine number: %d",cur_class_scopes->id, line_number);
		struct symbol * temp;
		if ((temp = lookupSymbol((yyvsp[(4) - (4)].id), CLASSTYPE,  cur_class_scopes)) == NULL)
		{
			error_extending_missing_class(line_number, (yyvsp[(2) - (4)].id), (yyvsp[(4) - (4)].id));
		}

		struct symbol *s = addClass((yyval.ci)->id, line_number, NULL, cur_class_scopes);
                //set this cl as current scope. We can refer to this from other productions.
                if (s != NULL)
                {
                        if (DEBUG) printf("\nADDING %s TO CUR CLASS SCOPES.\n", (yyvsp[(2) - (4)].id));
                        add_to_identifier_list_front(&cur_class_scopes, (yyvsp[(2) - (4)].id));
                }
                else
                {
                        add_to_identifier_list_front(&cur_class_scopes, "NOSCOPE");
                }
	}
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 266 "pascal.y"
    {
		struct class_block_t * class_block = new_class_block();
		(yyval.cb) = class_block;
		(yyval.cb)->vdl = (yyvsp[(1) - (2)].vdl);
		(yyval.cb)->fdl = (yyvsp[(2) - (2)].fdl);
	}
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 275 "pascal.y"
    {
		struct type_denoter_t * type_denoter = new_type_denoter();
		(yyval.tden) = type_denoter;
		(yyval.tden)->type = TYPE_DENOTER_T_ARRAY_TYPE;
		(yyval.tden)->data.at = (yyvsp[(1) - (1)].at);
		(yyval.tden)->name = (yyval.tden)->data.at->td->name;
	}
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 283 "pascal.y"
    {
		struct type_denoter_t * type_denoter = new_type_denoter();
		(yyval.tden) = type_denoter;
		// this may need to get changed. id may refer to a class, how to handle
		(yyval.tden)->type = lookupType((yyvsp[(1) - (1)].id));
		(yyval.tden)->name = (yyvsp[(1) - (1)].id);
	}
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 293 "pascal.y"
    {
		struct array_type_t * array_type = new_array_type();
		(yyval.at) = array_type;
		(yyval.at)->r = (yyvsp[(3) - (6)].r);
		(yyval.at)->td = (yyvsp[(6) - (6)].tden);
	}
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 302 "pascal.y"
    {
		struct range_t * range = new_range();
		(yyval.r) = range;
		(yyval.r)->min = (yyvsp[(1) - (3)].un);
		(yyval.r)->max = (yyvsp[(3) - (3)].un);
	}
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 311 "pascal.y"
    {
        struct variable_declaration_list_t *vd = new_variable_declaration_list();
		(yyval.vdl) = (yyvsp[(2) - (3)].vdl);
	}
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 316 "pascal.y"
    {
        struct variable_declaration_list_t *vd = new_variable_declaration_list();
        (yyval.vdl) = NULL; 
	}
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 323 "pascal.y"
    {
		struct variable_declaration_list_t * variable_declaration_list = new_variable_declaration_list();
		(yyval.vdl) = variable_declaration_list;
		(yyval.vdl)->next = (yyvsp[(1) - (3)].vdl);
		(yyval.vdl)->vd = (yyvsp[(3) - (3)].vd);
	}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 330 "pascal.y"
    {
		struct variable_declaration_list_t * variable_declaration_list = new_variable_declaration_list();
		(yyval.vdl) = variable_declaration_list;
		(yyval.vdl)->vd = (yyvsp[(1) - (1)].vd);
	}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 339 "pascal.y"
    {
        struct variable_declaration_t * variable_declaration = new_variable_declaration();
        (yyval.vd) = variable_declaration;
		(yyval.vd)->il = (yyvsp[(1) - (3)].idl);
		(yyval.vd)->tden = (yyvsp[(3) - (3)].tden);
		(yyval.vd)->line_number = line_number;
		struct identifier_list_t * _il = (yyvsp[(1) - (3)].idl);
		while(_il != NULL)
		{
			if (DEBUG) printf("\nADDING %s : TYPE: %s SCOPE: %s \n\n", _il->id, (yyvsp[(3) - (3)].tden)->name, cur_class_scopes->id);
			if (strcasecmp(_il->id, "True") == 0)
				error_variable_name_invalid(line_number, _il->id);
			if (strcasecmp(_il->id, "False") == 0)
				error_variable_name_invalid(line_number, _il->id);
			addVariable(_il->id, line_number, (yyvsp[(3) - (3)].tden), cur_class_scopes);
			_il = _il->next;
		}
	}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 360 "pascal.y"
    {
		struct func_declaration_list_t * func_declaration_list = new_func_declaration_list();
		(yyval.fdl) = func_declaration_list;
		(yyval.fdl)->next = (yyvsp[(1) - (3)].fdl);
		(yyval.fdl)->fd = (yyvsp[(3) - (3)].funcd);
	}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 367 "pascal.y"
    {
		struct func_declaration_list_t * func_declaration_list = new_func_declaration_list();
		(yyval.fdl) = func_declaration_list;
		(yyval.fdl)->fd = (yyvsp[(1) - (1)].funcd);
	}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 373 "pascal.y"
    {
		struct func_declaration_list_t * func_declaration_list = new_func_declaration_list();
		(yyval.fdl) = NULL;
	}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 380 "pascal.y"
    {
		struct formal_parameter_section_list_t * formal_parameter_section_list = new_formal_parameter_section_list();
        (yyval.fpsl) = (yyvsp[(2) - (3)].fpsl);
	}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 386 "pascal.y"
    {
		struct formal_parameter_section_list_t * formal_parameter_section_list = new_formal_parameter_section_list();
		(yyval.fpsl) = formal_parameter_section_list;
		(yyval.fpsl)->fps = (yyvsp[(3) - (3)].fps);
		(yyval.fpsl)->next = (yyvsp[(1) - (3)].fpsl);
	}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 393 "pascal.y"
    {
		struct formal_parameter_section_list_t * formal_parameter_section_list = new_formal_parameter_section_list();
		(yyval.fpsl) = formal_parameter_section_list;
		(yyval.fpsl)->fps = (yyvsp[(1) - (1)].fps);
	}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 401 "pascal.y"
    {
		(yyval.fps) = (yyvsp[(1) - (1)].fps);
	}
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 406 "pascal.y"
    {
		(yyval.fps) = (yyvsp[(1) - (1)].fps);
	}
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 412 "pascal.y"
    {
		//Not sure where to fill this one in. 
		struct formal_parameter_section_t * formal_parameter_section = new_formal_parameter_section();
		(yyval.fps) = formal_parameter_section;
		(yyval.fps)->il = (yyvsp[(1) - (3)].idl);
		(yyval.fps)->is_var = 0; //it's a value
		(yyval.fps)->id = (yyvsp[(3) - (3)].id);
   	}
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 423 "pascal.y"
    {
		struct formal_parameter_section_t * formal_parameter_section = new_formal_parameter_section();
		(yyval.fps) = formal_parameter_section;
		(yyval.fps)->il = (yyvsp[(2) - (4)].idl);
		(yyval.fps)->is_var = 1; //it's a variable
		(yyval.fps)->id = (yyvsp[(4) - (4)].id);	
	}
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 434 "pascal.y"
    {
		struct function_declaration_t * function_declaration = new_function_declaration();
        struct function_heading_t * fh = new_function_heading();
		(yyval.funcd) = function_declaration;
        (yyval.funcd)->fh = fh;
        (yyval.funcd)->fh->id = (yyvsp[(1) - (3)].id);
			//what is function_identification?	   the name of the function when there is no return type 
		(yyval.funcd)->fb = (yyvsp[(3) - (3)].fb);
		(yyval.funcd)->line_number = line_number;

	}
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 446 "pascal.y"
    {
		struct function_declaration_t * function_declaration = new_function_declaration();
		(yyval.funcd) = function_declaration;
		(yyval.funcd)->fh = (yyvsp[(1) - (3)].fh);
		(yyval.funcd)->fb = (yyvsp[(3) - (3)].fb);
		(yyval.funcd)->line_number = line_number;
	}
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 456 "pascal.y"
    {
		struct function_heading_t * function_heading = new_function_heading();
		(yyval.fh) = function_heading;
		(yyval.fh)->id = (yyvsp[(2) - (4)].id);
		(yyval.fh)->res = (yyvsp[(4) - (4)].id);
	}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 463 "pascal.y"
    {
		struct function_heading_t * function_heading = new_function_heading();
		(yyval.fh) = function_heading;
		(yyval.fh)->id = (yyvsp[(2) - (5)].id);
		(yyval.fh)->fpsl = (yyvsp[(3) - (5)].fpsl);
		(yyval.fh)->res = (yyvsp[(5) - (5)].id);
}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 476 "pascal.y"
    {
		(yyval.id) = (yyvsp[(2) - (2)].id);
		addFunction((yyval.id), line_number, cur_class_scopes);
	}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 485 "pascal.y"
    {
		struct function_block_t * function_block = new_function_block();
		(yyval.fb) = function_block;
		(yyval.fb)->vdl = (yyvsp[(1) - (2)].vdl);
		(yyval.fb)->ss = (yyvsp[(2) - (2)].ss);
	}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 497 "pascal.y"
    {
        (yyval.ss) = (yyvsp[(2) - (3)].ss);
	}
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 503 "pascal.y"
    {
		struct statement_sequence_t * statement_sequence = new_statement_sequence();
		(yyval.ss) = statement_sequence;
		(yyval.ss)->s = (yyvsp[(1) - (1)].s);
	}
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 509 "pascal.y"
    {
		struct statement_sequence_t * statement_sequence = new_statement_sequence();
		(yyval.ss) = statement_sequence;
		(yyval.ss)->next = (yyvsp[(1) - (3)].ss);
		(yyval.ss)->s = (yyvsp[(3) - (3)].s);
	}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 518 "pascal.y"
    {
		struct statement_t * statement = new_statement();
		(yyval.s) = statement;
		// this needs to be assigned a type, and handle the struct
		(yyval.s)->type = STATEMENT_T_ASSIGNMENT;
		(yyval.s)->data.as = (yyvsp[(1) - (1)].as);
		(yyval.s)->line_number = line_number;
	}
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 527 "pascal.y"
    {
		struct statement_t * statement = new_statement();
		(yyval.s) = statement;
		(yyval.s)->type = STATEMENT_T_SEQUENCE;
		(yyval.s)->data.ss = (yyvsp[(1) - (1)].ss);
		(yyval.s)->line_number = line_number;
	}
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 535 "pascal.y"
    {
		struct statement_t * statement = new_statement();
		(yyval.s) = statement;
		(yyval.s)->type = STATEMENT_T_IF;
		(yyval.s)->data.is = (yyvsp[(1) - (1)].is);
		(yyval.s)->line_number = line_number;
	}
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 543 "pascal.y"
    {
		struct statement_t * statement = new_statement();
		(yyval.s) = statement;
		(yyval.s)->type = STATEMENT_T_WHILE;
		(yyval.s)->data.ws = (yyvsp[(1) - (1)].ws);
		(yyval.s)->line_number = line_number;
	}
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 551 "pascal.y"
    {
		struct statement_t * statement = new_statement();
		(yyval.s) = statement;
		(yyval.s)->type = STATEMENT_T_PRINT;
		(yyval.s)->data.ps = (yyvsp[(1) - (1)].ps);
		(yyval.s)->line_number = line_number;
	}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 561 "pascal.y"
    {
		struct while_statement_t * while_statement = new_while_statement();
		(yyval.ws) = while_statement;
		(yyval.ws)->e = (yyvsp[(2) - (4)].e);
		(yyval.ws)->s = (yyvsp[(4) - (4)].s);
	}
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 570 "pascal.y"
    {
		struct if_statement_t * if_statement = new_if_statement();
		(yyval.is) = if_statement;
		(yyval.is)->e = (yyvsp[(2) - (6)].e);
		(yyval.is)->s1 = (yyvsp[(4) - (6)].s);
		(yyval.is)->s2 = (yyvsp[(6) - (6)].s);
	}
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 580 "pascal.y"
    {
		struct assignment_statement_t * assignment_statement = new_assignment_statement();
		(yyval.as) = assignment_statement;
		(yyval.as)->va = (yyvsp[(1) - (3)].va);
		(yyval.as)->e = (yyvsp[(3) - (3)].e);
		switch ((yyvsp[(1) - (3)].va)->type)
		{
			case VARIABLE_ACCESS_T_IDENTIFIER:
				
				break;
			case VARIABLE_ACCESS_T_INDEXED_VARIABLE:
				
				break;
			case VARIABLE_ACCESS_T_ATTRIBUTE_DESIGNATOR:
	
				break;
			case VARIABLE_ACCESS_T_METHOD_DESIGNATOR:
		
				break;
		}
		if ((yyvsp[(1) - (3)].va)->expr->type != NULL && (yyvsp[(3) - (3)].e)->expr->type != NULL &&
		    (strcasecmp((yyvsp[(1) - (3)].va)->expr->type, (yyvsp[(3) - (3)].e)->expr->type)) != 0)
		{
			error_type_mismatch(line_number, (yyvsp[(1) - (3)].va)->expr->type, (yyvsp[(3) - (3)].e)->expr->type);
		}			
	}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 607 "pascal.y"
    {
		struct assignment_statement_t * assignment_statement = new_assignment_statement();
		(yyval.as) = assignment_statement;
		(yyval.as)->va = (yyvsp[(1) - (3)].va);
		(yyval.as)->oe = (yyvsp[(3) - (3)].os);
	}
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 616 "pascal.y"
    {
		struct object_instantiation_t * object_instantiation = new_object_instantiation();
		(yyval.os) = object_instantiation;
		(yyval.os)->id = (yyvsp[(2) - (2)].id);
	}
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 622 "pascal.y"
    {
		struct object_instantiation_t * object_instantiation = new_object_instantiation();
		(yyval.os) = object_instantiation;
		(yyval.os)->id = (yyvsp[(2) - (3)].id);
		(yyval.os)->apl = (yyvsp[(3) - (3)].apl);
	}
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 631 "pascal.y"
    {
		struct print_statement_t * print_statement = new_print_statement();
		(yyval.ps) = print_statement;
		(yyval.ps)->va = (yyvsp[(2) - (2)].va);
	}
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 639 "pascal.y"
    {
		struct variable_access_t * variable_access = new_variable_access();
		(yyval.va) = variable_access;
        	(yyval.va)->type = 1;
		(yyval.va)->data.id = (yyvsp[(1) - (1)].id);
		(yyval.va)->type = VARIABLE_ACCESS_T_IDENTIFIER;
		struct symbol * s = lookupSymbol((yyvsp[(1) - (1)].id), VARIABLETYPE, cur_class_scopes);
		(yyval.va)->expr = new_expression_data();
		if (s == NULL) s = lookupSymbol((yyvsp[(1) - (1)].id), VARIABLEARRAYTYPE, cur_class_scopes);
		if (s != NULL)
		{
			(yyval.va)->expr->type = s->type;
		}
		else
		{
//			error_variable_not_declared(line_number, $1);
		}
	}
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 658 "pascal.y"
    {
		struct variable_access_t * variable_access = new_variable_access();
		(yyval.va) = variable_access;
	        (yyval.va)->type = 2;
		(yyval.va)->data.iv = (yyvsp[(1) - (1)].iv);
		(yyval.va)->type = VARIABLE_ACCESS_T_INDEXED_VARIABLE;
		(yyval.va)->expr = new_expression_data();
		(yyval.va)->expr->type = (yyvsp[(1) - (1)].iv)->expr->type;
       
	}
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 669 "pascal.y"
    {
		struct variable_access_t * variable_access = new_variable_access();
		(yyval.va) = variable_access;
	        (yyval.va)->type = 3;
		(yyval.va)->data.ad = (yyvsp[(1) - (1)].ad);
		(yyval.va)->type = VARIABLE_ACCESS_T_ATTRIBUTE_DESIGNATOR;
		(yyval.va)->expr = new_expression_data();
		(yyval.va)->expr->type = (yyvsp[(1) - (1)].ad)->va->expr->type;
	}
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 679 "pascal.y"
    {
		struct variable_access_t * variable_access = new_variable_access();
		(yyval.va) = variable_access;
	        (yyval.va)->type = 4;
		(yyval.va)->data.md = (yyvsp[(1) - (1)].md);
		(yyval.va)->type = VARIABLE_ACCESS_T_METHOD_DESIGNATOR;
		(yyval.va)->expr = new_expression_data();
		(yyval.va)->expr->type = (yyvsp[(1) - (1)].md)->va->expr->type;
	}
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 691 "pascal.y"
    {
		struct indexed_variable_t * indexed_variable = new_indexed_variable();
		(yyval.iv) = indexed_variable;
		(yyval.iv)->va = (yyvsp[(1) - (4)].va);
		(yyval.iv)->iel = (yyvsp[(3) - (4)].iel);
		(yyval.iv)->expr = new_expression_data();
		(yyval.iv)->expr->type = (yyvsp[(1) - (4)].va)->expr->type;
		if (strcasecmp((yyvsp[(3) - (4)].iel)->expr->type, PRIMITIVE_TYPE_NAME_INTEGER) != 0)
			error_array_index_is_not_integer(line_number, (yyvsp[(1) - (4)].va)->data.id);
	}
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 704 "pascal.y"
    {
		struct index_expression_list_t * index_expression_list = new_index_expression_list();
		(yyval.iel) = index_expression_list;
		(yyval.iel)->next = (yyvsp[(1) - (3)].iel);
		(yyval.iel)->e = (yyvsp[(3) - (3)].e);
		(yyval.iel)->expr = new_expression_data();
		(yyval.iel)->expr->type = (yyvsp[(1) - (3)].iel)->expr->type;
	}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 713 "pascal.y"
    {
		struct index_expression_list_t * index_expression_list = new_index_expression_list();
		(yyval.iel) = index_expression_list;
		(yyval.iel)->e = (yyvsp[(1) - (1)].e);
		(yyval.iel)->expr = new_expression_data();
		(yyval.iel)->expr->type = (yyvsp[(1) - (1)].e)->expr->type;
	}
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 725 "pascal.y"
    {
		struct attribute_designator_t * attribute_designator = new_attribute_designator();
		(yyval.ad) = attribute_designator;
		(yyval.ad)->va = (yyvsp[(1) - (3)].va);
		(yyval.ad)->id = (yyvsp[(3) - (3)].id);
	}
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 734 "pascal.y"
    {
		struct method_designator_t * method_designator = new_method_designator();
		(yyval.md) = method_designator;
		(yyval.md)->va = (yyvsp[(1) - (3)].va);
		(yyval.md)->fd = (yyvsp[(3) - (3)].fdes);
	}
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 743 "pascal.y"
    {
		struct actual_parameter_list_t * actual_parameter_list = new_actual_parameter_list();
		(yyval.apl) = actual_parameter_list;
		(yyval.apl)->next = (yyvsp[(2) - (3)].apl);
	}
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 751 "pascal.y"
    {
		struct actual_parameter_list_t * actual_parameter_list = new_actual_parameter_list();
		(yyval.apl) = actual_parameter_list;
		(yyval.apl)->next = (yyvsp[(1) - (3)].apl);
		(yyval.apl)->ap = (yyvsp[(3) - (3)].ap);
	}
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 758 "pascal.y"
    {
		struct actual_parameter_list_t * actual_parameter_list = new_actual_parameter_list();
		(yyval.apl) = actual_parameter_list;
		(yyval.apl)->ap = (yyvsp[(1) - (1)].ap);
	}
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 766 "pascal.y"
    {
		struct actual_parameter_t * actual_parameter = new_actual_parameter();
		(yyval.ap) = actual_parameter;
		(yyval.ap)->e1 = (yyvsp[(1) - (1)].e);
	}
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 772 "pascal.y"
    {
		struct actual_parameter_t * actual_parameter = new_actual_parameter();
		(yyval.ap) = actual_parameter;
		(yyval.ap)->e1 = (yyvsp[(1) - (3)].e);
		(yyval.ap)->e2 = (yyvsp[(3) - (3)].e);
	}
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 779 "pascal.y"
    {
		struct actual_parameter_t * actual_parameter = new_actual_parameter();
		(yyval.ap) = actual_parameter;
		(yyval.ap)->e1 = (yyvsp[(1) - (5)].e);
		(yyval.ap)->e2 = (yyvsp[(3) - (5)].e);
		(yyval.ap)->e3 = (yyvsp[(5) - (5)].e);
	}
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 791 "pascal.y"
    {
		struct expression_t * expression = new_expression();
		(yyval.e) = expression;
		(yyval.e)->se1 = (yyvsp[(1) - (1)].se);
		(yyval.e)->expr = new_expression_data();
		(yyval.e)->expr->type = (yyvsp[(1) - (1)].se)->expr->type;
	}
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 799 "pascal.y"
    {
		struct expression_t * expression = new_expression();
		(yyval.e) = expression;
		(yyval.e)->se1 = (yyvsp[(1) - (3)].se);
		(yyval.e)->relop = (yyvsp[(2) - (3)].op);
		(yyval.e)->se2 = (yyvsp[(3) - (3)].se);
		(yyval.e)->expr = new_expression_data();
		(yyval.e)->expr->type = (yyvsp[(1) - (3)].se)->expr->type;
	}
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 811 "pascal.y"
    {
		struct simple_expression_t * simple_expression = new_simple_expression();
		(yyval.se) = simple_expression;
		(yyval.se)->t = (yyvsp[(1) - (1)].t);
		(yyval.se)->expr = new_expression_data();
		(yyval.se)->expr->type = (yyvsp[(1) - (1)].t)->expr->type;
	}
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 819 "pascal.y"
    {
		struct simple_expression_t * simple_expression = new_simple_expression();
		(yyval.se) = simple_expression;
		(yyval.se)->next = (yyvsp[(1) - (3)].se);
		(yyval.se)->addop = (yyvsp[(2) - (3)].op);
		(yyval.se)->t = (yyvsp[(3) - (3)].t);
		(yyval.se)->expr = new_expression_data();
		(yyval.se)->expr->type = (yyvsp[(1) - (3)].se)->expr->type;
	}
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 831 "pascal.y"
    {
		struct term_t * term = new_term();
		(yyval.t) = term;
		(yyval.t)->f = (yyvsp[(1) - (1)].f);
		(yyval.t)->expr = new_expression_data();
		(yyval.t)->expr->type = (yyvsp[(1) - (1)].f)->expr->type;
	}
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 839 "pascal.y"
    {
		struct term_t * term = new_term();
		(yyval.t) = term;
		(yyval.t)->next = (yyvsp[(1) - (3)].t); 
		(yyval.t)->mulop = (yyvsp[(2) - (3)].op);
		(yyval.t)->f = (yyvsp[(3) - (3)].f);
		(yyval.t)->expr = new_expression_data();
		(yyval.t)->expr->type = (yyvsp[(1) - (3)].t)->expr->type;
	}
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 851 "pascal.y"
    {
        (yyval.i)=yylval.i;

	}
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 856 "pascal.y"
    {
        (yyval.i)=yylval.i;
	}
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 862 "pascal.y"
    {
		struct factor_t * factor = new_factor();
		(yyval.f) = factor;
		(yyval.f)->type = FACTOR_T_SIGNFACTOR;
		(yyval.f)->data.f.sign = (yyvsp[(1) - (2)].i);
		(yyval.f)->data.f.next = (yyvsp[(2) - (2)].f);
		(yyval.f)->expr = new_expression_data();
		(yyval.f)->expr->type = (yyvsp[(2) - (2)].f)->expr->type;
	}
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 872 "pascal.y"
    {
		struct factor_t * factor = new_factor();
		(yyval.f) = factor;
		(yyval.f)->type = FACTOR_T_PRIMARY;
		(yyval.f)->data.p = (yyvsp[(1) - (1)].p);
		(yyval.f)->expr = new_expression_data();
		(yyval.f)->expr->type = (yyvsp[(1) - (1)].p)->expr->type;
	}
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 883 "pascal.y"
    {
		struct primary_t * primary = new_primary();
		(yyval.p) = primary;
		(yyval.p)->data.va = (yyvsp[(1) - (1)].va);
		(yyval.p)->type = PRIMARY_T_VARIABLE_ACCESS;
		(yyval.p)->expr = new_expression_data();
		(yyval.p)->expr->type = (yyvsp[(1) - (1)].va)->expr->type;
		if ((yyvsp[(1) - (1)].va)->type == VARIABLE_ACCESS_T_IDENTIFIER && (yyvsp[(1) - (1)].va)->expr->type == NULL)
		{		
			//error_variable_not_declared(line_number, $1->data.id);
		}
	}
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 896 "pascal.y"
    {
		struct primary_t * primary = new_primary();
		(yyval.p) = primary;
		(yyval.p)->data.un = (yyvsp[(1) - (1)].un);
		(yyval.p)->type = PRIMARY_T_UNSIGNED_CONSTANT;
		(yyval.p)->expr = new_expression_data();
		(yyval.p)->expr->type = (yyvsp[(1) - (1)].un)->expr->type;
	}
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 905 "pascal.y"
    {
		struct primary_t * primary = new_primary();
		(yyval.p) = primary;
		(yyval.p)->data.fd = (yyvsp[(1) - (1)].fdes);
		(yyval.p)->type = PRIMARY_T_FUNCTION_DESIGNATOR;
		(yyval.p)->expr = new_expression_data();
		// may have to add return type as the "type" of a function
	//	$$->expr->type = (lookupSymbol($1->id, FUNCTIONTYPE, cur_class_scopes))->type;
	}
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 915 "pascal.y"
    {
		struct primary_t * primary = new_primary();
		(yyval.p) = primary;
		(yyval.p)->data.e = (yyvsp[(2) - (3)].e);
		(yyval.p)->type = PRIMARY_T_EXPRESSION;
		(yyval.p)->expr = new_expression_data();
		(yyval.p)->expr->type = (yyvsp[(2) - (3)].e)->expr->type;
	}
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 924 "pascal.y"
    {
		struct primary_t * primary = new_primary();
		(yyval.p) = primary;
		(yyval.p)->type = PRIMARY_T_PRIMARY;
		(yyval.p)->data.p.not = 1;
		(yyval.p)->data.p.next = (yyvsp[(2) - (2)].p);
		(yyval.p)->expr = new_expression_data();
		(yyval.p)->expr->type = (yyvsp[(2) - (2)].p)->expr->type; 
	}
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 941 "pascal.y"
    {
		struct unsigned_number_t * unsigned_number = new_unsigned_number();
		(yyval.un) = unsigned_number;
        (yyval.un)->ui = *(yylval.i);
		// is this the right way to get the val from yylval
		(yyval.un)->expr = new_expression_data();
		(yyval.un)->expr->val = (float)(*yylval.i);
		(yyval.un)->expr->type = "integer";
	}
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 954 "pascal.y"
    {
		struct function_designator_t * function_designator = new_function_designator();
		(yyval.fdes) = function_designator;
        (yyval.fdes)->id = (yyvsp[(1) - (2)].id);
        (yyval.fdes)->apl = (yyvsp[(2) - (2)].apl);
		
	}
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 964 "pascal.y"
    {
		(yyval.op) = PLUS;
	}
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 968 "pascal.y"
    {
		(yyval.op) = MINUS;
	}
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 972 "pascal.y"
    {
		(yyval.op) = OR;
	}
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 978 "pascal.y"
    {
		(yyval.op) = STAR;
	}
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 982 "pascal.y"
    {
		(yyval.op) = SLASH;
	}
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 986 "pascal.y"
    {
		(yyval.op) = MOD;
	}
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 990 "pascal.y"
    {
		(yyval.op) = AND;
	}
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 996 "pascal.y"
    {
		(yyval.op) = EQUAL;
	}
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 1000 "pascal.y"
    {
		(yyval.op) = NOTEQUAL;
	}
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 1004 "pascal.y"
    {
		(yyval.op) = LT;
	}
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 1008 "pascal.y"
    {
		(yyval.op) = GT;
	}
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 1012 "pascal.y"
    {
		(yyval.op) = LE;
	}
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 1016 "pascal.y"
    {
		(yyval.op) = GE;
	}
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 1022 "pascal.y"
    {
		(yyval.id) = strdup(yylval.id);
	}
    break;



/* Line 1806 of yacc.c  */
#line 2933 "y.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

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
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



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
		      yytoken, &yylval);
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
      if (!yypact_value_is_default (yyn))
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


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


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
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
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



/* Line 2067 of yacc.c  */
#line 1033 "pascal.y"


