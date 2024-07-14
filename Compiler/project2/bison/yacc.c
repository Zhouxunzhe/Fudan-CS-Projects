/* A Bison parser, made by GNU Bison 3.8.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30801

/* Bison version string.  */
#define YYBISON_VERSION "3.8.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "bison.y"

#include <iostream>
#include <string>

#include "lexer.c"
#include "ast.h"
using namespace std;

struct Node *root;

void yyerror(const char* msg){
  cerr << msg << endl;
}

#line 86 "yacc.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "yacc.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_LT = 3,                         /* LT  */
  YYSYMBOL_GT = 4,                         /* GT  */
  YYSYMBOL_EQ = 5,                         /* EQ  */
  YYSYMBOL_NE = 6,                         /* NE  */
  YYSYMBOL_LE = 7,                         /* LE  */
  YYSYMBOL_GE = 8,                         /* GE  */
  YYSYMBOL_ADD = 9,                        /* ADD  */
  YYSYMBOL_SUB = 10,                       /* SUB  */
  YYSYMBOL_MUL = 11,                       /* MUL  */
  YYSYMBOL_DIV = 12,                       /* DIV  */
  YYSYMBOL_ASSIGN = 13,                    /* ASSIGN  */
  YYSYMBOL_COLON = 14,                     /* COLON  */
  YYSYMBOL_LS = 15,                        /* LS  */
  YYSYMBOL_RS = 16,                        /* RS  */
  YYSYMBOL_LP = 17,                        /* LP  */
  YYSYMBOL_RP = 18,                        /* RP  */
  YYSYMBOL_LPAREN = 19,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 20,                    /* RPAREN  */
  YYSYMBOL_LARRAY = 21,                    /* LARRAY  */
  YYSYMBOL_RARRAY = 22,                    /* RARRAY  */
  YYSYMBOL_SEMI = 23,                      /* SEMI  */
  YYSYMBOL_DOT = 24,                       /* DOT  */
  YYSYMBOL_COM = 25,                       /* COM  */
  YYSYMBOL_STRING = 26,                    /* STRING  */
  YYSYMBOL_REAL = 27,                      /* REAL  */
  YYSYMBOL_INTEGER = 28,                   /* INTEGER  */
  YYSYMBOL_ID = 29,                        /* ID  */
  YYSYMBOL_LOOP = 30,                      /* LOOP  */
  YYSYMBOL_PROGRAM = 31,                   /* PROGRAM  */
  YYSYMBOL_VAR = 32,                       /* VAR  */
  YYSYMBOL_ARRAY = 33,                     /* ARRAY  */
  YYSYMBOL_END = 34,                       /* END  */
  YYSYMBOL_READ = 35,                      /* READ  */
  YYSYMBOL_WHILE = 36,                     /* WHILE  */
  YYSYMBOL_MYBEGIN = 37,                   /* MYBEGIN  */
  YYSYMBOL_EXIT = 38,                      /* EXIT  */
  YYSYMBOL_RECORD = 39,                    /* RECORD  */
  YYSYMBOL_WRITE = 40,                     /* WRITE  */
  YYSYMBOL_BY = 41,                        /* BY  */
  YYSYMBOL_FOR = 42,                       /* FOR  */
  YYSYMBOL_RETURN = 43,                    /* RETURN  */
  YYSYMBOL_DO = 44,                        /* DO  */
  YYSYMBOL_TO = 45,                        /* TO  */
  YYSYMBOL_IS = 46,                        /* IS  */
  YYSYMBOL_PROCEDURE = 47,                 /* PROCEDURE  */
  YYSYMBOL_TYPE = 48,                      /* TYPE  */
  YYSYMBOL_NOT = 49,                       /* NOT  */
  YYSYMBOL_IF = 50,                        /* IF  */
  YYSYMBOL_THEN = 51,                      /* THEN  */
  YYSYMBOL_ELSIF = 52,                     /* ELSIF  */
  YYSYMBOL_ELSE = 53,                      /* ELSE  */
  YYSYMBOL_LUM = 54,                       /* LUM  */
  YYSYMBOL_OR = 55,                        /* OR  */
  YYSYMBOL_MOD = 56,                       /* MOD  */
  YYSYMBOL_AND = 57,                       /* AND  */
  YYSYMBOL_OF = 58,                        /* OF  */
  YYSYMBOL_POS = 59,                       /* POS  */
  YYSYMBOL_NEG = 60,                       /* NEG  */
  YYSYMBOL_YYACCEPT = 61,                  /* $accept  */
  YYSYMBOL_program = 62,                   /* program  */
  YYSYMBOL_body = 63,                      /* body  */
  YYSYMBOL_declaration_sm = 64,            /* declaration_sm  */
  YYSYMBOL_declaration = 65,               /* declaration  */
  YYSYMBOL_var_decl_sm = 66,               /* var_decl_sm  */
  YYSYMBOL_var_decl = 67,                  /* var_decl  */
  YYSYMBOL_type_decl_sm = 68,              /* type_decl_sm  */
  YYSYMBOL_type_decl = 69,                 /* type_decl  */
  YYSYMBOL_procedure_decl_sm = 70,         /* procedure_decl_sm  */
  YYSYMBOL_procedure_decl = 71,            /* procedure_decl  */
  YYSYMBOL_type = 72,                      /* type  */
  YYSYMBOL_component_sm = 73,              /* component_sm  */
  YYSYMBOL_component = 74,                 /* component  */
  YYSYMBOL_formal_params = 75,             /* formal_params  */
  YYSYMBOL_fp_section_semicolon_sm = 76,   /* fp_section_semicolon_sm  */
  YYSYMBOL_fp_section = 77,                /* fp_section  */
  YYSYMBOL_id_comma_sm = 78,               /* id_comma_sm  */
  YYSYMBOL_statement_sm = 79,              /* statement_sm  */
  YYSYMBOL_statement = 80,                 /* statement  */
  YYSYMBOL_elseif_then_sm = 81,            /* elseif_then_sm  */
  YYSYMBOL_write_params = 82,              /* write_params  */
  YYSYMBOL_write_expr = 83,                /* write_expr  */
  YYSYMBOL_write_expr_comma_sm = 84,       /* write_expr_comma_sm  */
  YYSYMBOL_expression = 85,                /* expression  */
  YYSYMBOL_expression_comma_sm = 86,       /* expression_comma_sm  */
  YYSYMBOL_expression_id_semicolon_assign_sm = 87, /* expression_id_semicolon_assign_sm  */
  YYSYMBOL_l_value_comma_sm = 88,          /* l_value_comma_sm  */
  YYSYMBOL_l_value = 89,                   /* l_value  */
  YYSYMBOL_actual_params = 90,             /* actual_params  */
  YYSYMBOL_comp_values = 91,               /* comp_values  */
  YYSYMBOL_array_values = 92,              /* array_values  */
  YYSYMBOL_array_value = 93,               /* array_value  */
  YYSYMBOL_array_value_comma_sm = 94,      /* array_value_comma_sm  */
  YYSYMBOL_number = 95                     /* number  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   690

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  35
/* YYNRULES -- Number of rules.  */
#define YYNRULES  105
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  254

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   315


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    64,    64,    65,    66,    68,    71,    72,    74,    75,
      76,    79,    80,    82,    83,    84,    87,    88,    90,    91,
      94,    95,    97,    98,    99,   100,   103,   104,   105,   108,
     109,   111,   112,   115,   116,   119,   120,   122,   124,   125,
     128,   129,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   146,   147,   150,   151,
     153,   154,   156,   157,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   184,   185,   187,
     188,   191,   192,   194,   195,   196,   198,   199,   202,   204,
     206,   207,   209,   210,   212,   213
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "LT", "GT", "EQ", "NE",
  "LE", "GE", "ADD", "SUB", "MUL", "DIV", "ASSIGN", "COLON", "LS", "RS",
  "LP", "RP", "LPAREN", "RPAREN", "LARRAY", "RARRAY", "SEMI", "DOT", "COM",
  "STRING", "REAL", "INTEGER", "ID", "LOOP", "PROGRAM", "VAR", "ARRAY",
  "END", "READ", "WHILE", "MYBEGIN", "EXIT", "RECORD", "WRITE", "BY",
  "FOR", "RETURN", "DO", "TO", "IS", "PROCEDURE", "TYPE", "NOT", "IF",
  "THEN", "ELSIF", "ELSE", "LUM", "OR", "MOD", "AND", "OF", "POS", "NEG",
  "$accept", "program", "body", "declaration_sm", "declaration",
  "var_decl_sm", "var_decl", "type_decl_sm", "type_decl",
  "procedure_decl_sm", "procedure_decl", "type", "component_sm",
  "component", "formal_params", "fp_section_semicolon_sm", "fp_section",
  "id_comma_sm", "statement_sm", "statement", "elseif_then_sm",
  "write_params", "write_expr", "write_expr_comma_sm", "expression",
  "expression_comma_sm", "expression_id_semicolon_assign_sm",
  "l_value_comma_sm", "l_value", "actual_params", "comp_values",
  "array_values", "array_value", "array_value_comma_sm", "number", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-48)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-41)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      15,   -29,   -25,    17,    80,    -4,   118,    10,     8,    75,
      75,   -48,    36,    35,   -48,    80,    57,   -48,    -4,    54,
      37,   -48,   118,   -48,   603,   -48,    72,   -48,    53,    34,
     -48,    70,     6,   -48,   -48,   125,   -48,    85,   101,   603,
     106,   641,   123,   129,   128,   561,   641,   133,   578,    81,
     -48,    35,   641,   125,   -48,    35,   145,    75,   125,    75,
     -48,   130,     3,   163,   -48,   143,   175,   157,   176,   641,
     641,   641,   -48,   -48,   168,   641,   170,    87,   -48,   -48,
     174,   184,   146,   -48,   225,   250,   -48,   -48,   641,   641,
     179,   -48,   260,   186,   183,   187,   195,   194,    11,   198,
     125,   199,   224,    92,   -48,   -48,   283,   -48,   216,   -48,
     136,   -48,   -48,   315,   211,   641,   -48,   -48,   -48,   -48,
     641,   641,   641,   641,   641,   641,   641,   641,   641,   641,
     603,   641,   641,   641,   641,   -48,   -48,   217,   466,   -48,
     641,   -48,   578,   338,   348,   -48,   -48,   641,   125,   145,
     -48,   -48,    75,    75,   -48,   -48,   -48,   125,   209,    92,
     641,   226,   -48,   176,   227,   -48,   231,    31,   220,    19,
      19,   545,   545,    19,    19,     2,     2,   -48,   -48,   215,
     -48,   521,   -48,   555,   638,   230,   370,   221,   -48,   -48,
     425,   -48,   -48,   228,   251,   252,   -48,   -48,   283,   -48,
     136,   253,   641,   641,   641,   255,   261,   217,   -48,   641,
     641,    38,   -48,   -48,   -48,   -48,   -48,   -48,   -48,   435,
     466,   220,   -48,   -48,   -48,    59,   456,   262,   603,   249,
     278,   -48,   641,   603,   578,   -48,   263,   285,   -48,   490,
     265,   221,   277,   641,   603,   279,   -48,   -48,   435,   269,
     -48,   -48,   286,   -48
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     3,     0,     0,     0,    20,     0,     0,     0,     6,
       6,     1,     0,    38,     8,     0,     0,    10,    20,     0,
       0,     9,     0,     4,     0,     7,     0,    15,     0,     0,
      12,     0,     0,    21,    19,     0,    17,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       2,    38,     0,     0,    34,    38,    35,     6,     0,     6,
      26,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,   105,   104,    93,     0,     0,    65,    64,    52,
       0,     0,     0,    54,     0,     0,     5,    41,     0,     0,
       0,    39,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    18,    97,    87,    43,     0,    93,
      91,    67,    68,     0,     0,     0,    84,    85,    86,    69,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,    62,    61,    45,
       0,    53,     0,     0,     0,    95,    14,     0,     0,    35,
      33,    25,     6,     6,    23,    27,    32,     0,     0,     0,
       0,     0,    49,     0,     0,    66,     0,   101,   102,    79,
      78,    80,    83,    82,    81,    70,    71,    72,    74,     0,
      73,    76,    75,    77,     0,     0,     0,    56,    42,    94,
       0,    37,    36,     0,     0,     0,    28,    30,    87,    96,
      91,     0,     0,     0,     0,     0,     0,    62,    58,     0,
       0,     0,    13,    24,    22,    31,    88,    92,    44,    89,
     100,   102,    99,    48,    63,     0,     0,     0,     0,     0,
       0,   103,     0,     0,     0,    47,     0,     0,    98,     0,
       0,    56,     0,     0,     0,     0,    57,    46,    89,     0,
      51,    90,     0,    50
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -48,   -48,    -8,   301,   -48,   296,   -48,   290,   -48,   295,
     -48,   -47,   169,   267,   -48,   181,   236,   -46,   -38,   -48,
      91,   -48,   149,   127,     9,   138,   114,   165,   -24,   289,
     -48,   -48,   162,   147,   -48
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     3,     7,     8,     9,    14,    15,    21,    22,    17,
      18,    63,   158,   159,    32,    96,    56,    29,    47,    48,
     211,    81,   137,   185,   138,   161,   230,   164,    77,    66,
     117,   118,   168,   205,    78
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      49,    67,    26,     4,   101,    91,    93,    57,    -6,    94,
      87,    98,   152,   128,   129,    49,     1,    11,     5,     6,
      58,    10,   -41,   -41,    49,    16,   -41,   -41,   126,   127,
     128,   129,   102,    23,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   110,    24,     2,    52,    53,    97,
      76,    99,    59,   155,    84,    85,   131,   153,   133,    27,
      28,    92,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   227,   131,   106,   133,    31,    34,   111,   112,
     113,    12,    51,    35,   119,   131,   132,   133,   134,   203,
      54,   228,   179,   101,    88,    50,    89,   143,   144,    55,
     232,   191,    89,   233,   187,    90,    49,     4,    64,    13,
     195,    90,   -11,   131,   132,   133,   134,   -11,    49,    19,
      65,   102,     5,     6,   167,    68,   -29,   -11,   -11,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   200,
     180,   181,   182,   183,   193,   194,    79,    20,    80,   186,
     -16,    89,    69,    70,    60,   -16,   190,    82,    61,   140,
      90,   163,    71,   105,    62,   -16,   -16,    86,    95,   198,
      72,    73,    74,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,    69,    70,   114,   104,    65,   100,   115,
     236,   108,    75,    71,   135,   240,   241,   148,   107,   147,
     136,    72,    73,    74,    49,   109,   249,   139,   145,    49,
      49,   219,   220,   167,   130,   150,    55,   151,   225,   226,
      49,   154,   156,    75,   131,   132,   133,   134,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   157,   162,
     166,   239,   184,   196,   202,   204,   199,   201,   141,   206,
     208,   213,   248,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   210,   214,   215,   218,   222,   237,   131,
     132,   133,   134,   146,   223,   235,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   238,   242,   243,   245,
     247,   142,   250,   252,   131,   132,   133,   134,   160,   253,
      25,    30,    36,    33,   131,   132,   133,   134,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   197,   103,
     192,   149,   246,   207,   224,   165,   216,   131,   132,   133,
     134,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   188,   251,   116,   189,   217,   221,     0,   231,   131,
     132,   133,   134,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   131,   132,   133,   134,     0,     0,     0,     0,
       0,     0,   131,   132,   133,   134,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   209,     0,     0,     0,     0,
       0,     0,     0,     0,   131,   132,   133,   134,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   212,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   229,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   131,
     132,   133,   134,     0,     0,     0,     0,     0,     0,   131,
     132,   133,   134,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,     0,     0,     0,     0,   234,     0,     0,
     131,   132,   133,   134,     0,     0,     0,     0,     0,     0,
     131,   132,   133,   134,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   244,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   131,   132,   133,   134,   120,   121,
     -41,   -41,   124,   125,   126,   127,   128,   129,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,     0,     0,
      69,    70,     0,     0,     0,   131,     0,   133,   134,    37,
      71,     0,     0,     0,    83,     0,     0,     0,    72,    73,
      74,     0,     0,     0,     0,     0,     0,     0,     0,   131,
       0,   133,     0,     0,    37,     0,     0,    38,    39,   131,
      75,   133,   -40,    40,    41,     0,    42,     0,    43,     0,
      44,    45,     0,     0,     0,     0,     0,     0,    46,     0,
     -40,   -40,    38,    39,     0,     0,     0,   -40,    40,    41,
       0,    42,     0,    43,     0,    44,    45,    69,    70,     0,
      69,    70,     0,    46,     0,     0,     0,    71,     0,     0,
      71,     0,     0,     0,   136,    72,    73,    74,    72,    73,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,     0,     0,
      75
};

static const yytype_int16 yycheck[] =
{
      24,    39,    10,    32,     1,    51,    53,     1,    37,    55,
      48,    58,     1,    11,    12,    39,     1,     0,    47,    48,
      14,    46,     3,     4,    48,    29,     7,     8,     9,    10,
      11,    12,    29,    23,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    68,    37,    31,    13,    14,    57,
      41,    59,    46,   100,    45,    46,    54,    46,    56,    23,
      25,    52,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    34,    54,    65,    56,    19,    23,    69,    70,
      71,     1,    29,    46,    75,    54,    55,    56,    57,    58,
      20,    53,   130,     1,    13,    23,    15,    88,    89,    29,
      41,   148,    15,    44,   142,    24,   130,    32,    23,    29,
     157,    24,    32,    54,    55,    56,    57,    37,   142,     1,
      19,    29,    47,    48,   115,    19,    34,    47,    48,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   163,
     131,   132,   133,   134,   152,   153,    23,    29,    19,   140,
      32,    15,     9,    10,    29,    37,   147,    29,    33,    13,
      24,    25,    19,    20,    39,    47,    48,    34,    23,   160,
      27,    28,    29,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,     9,    10,    17,    23,    19,    58,    21,
     228,    34,    49,    19,    20,   233,   234,    14,    23,    13,
      26,    27,    28,    29,   228,    29,   244,    23,    29,   233,
     234,   202,   203,   204,    44,    20,    29,    23,   209,   210,
     244,    23,    23,    49,    54,    55,    56,    57,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    14,    23,
      29,   232,    25,    34,    13,    25,    20,    20,    23,    34,
      20,    23,   243,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    52,    23,    23,    23,    22,    29,    54,
      55,    56,    57,    23,    23,    23,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    18,    34,    13,    34,
      23,    51,    23,    34,    54,    55,    56,    57,    25,    23,
       9,    15,    22,    18,    54,    55,    56,    57,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,   159,    62,
     149,    95,   241,   184,   207,    20,   198,    54,    55,    56,
      57,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    23,   248,    74,    16,   200,   204,    -1,   221,    54,
      55,    56,    57,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    54,
      55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      55,    56,    57,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    -1,    -1,    -1,    -1,    51,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    -1,    -1,
       9,    10,    -1,    -1,    -1,    54,    -1,    56,    57,     1,
      19,    -1,    -1,    -1,    23,    -1,    -1,    -1,    27,    28,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      -1,    56,    -1,    -1,     1,    -1,    -1,    29,    30,    54,
      49,    56,    34,    35,    36,    -1,    38,    -1,    40,    -1,
      42,    43,    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,
      52,    53,    29,    30,    -1,    -1,    -1,    34,    35,    36,
      -1,    38,    -1,    40,    -1,    42,    43,     9,    10,    -1,
       9,    10,    -1,    50,    -1,    -1,    -1,    19,    -1,    -1,
      19,    -1,    -1,    -1,    26,    27,    28,    29,    27,    28,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,
      49
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,    31,    62,    32,    47,    48,    63,    64,    65,
      46,     0,     1,    29,    66,    67,    29,    70,    71,     1,
      29,    68,    69,    23,    37,    64,    63,    23,    25,    78,
      66,    19,    75,    70,    23,    46,    68,     1,    29,    30,
      35,    36,    38,    40,    42,    43,    50,    79,    80,    89,
      23,    29,    13,    14,    20,    29,    77,     1,    14,    46,
      29,    33,    39,    72,    23,    19,    90,    79,    19,     9,
      10,    19,    27,    28,    29,    49,    85,    89,    95,    23,
      19,    82,    29,    23,    85,    85,    34,    79,    13,    15,
      24,    78,    85,    72,    78,    23,    76,    63,    72,    63,
      58,     1,    29,    74,    23,    20,    85,    23,    34,    29,
      89,    85,    85,    85,    17,    21,    90,    91,    92,    85,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      44,    54,    55,    56,    57,    20,    26,    83,    85,    23,
      13,    23,    51,    85,    85,    29,    23,    13,    14,    77,
      20,    23,     1,    46,    23,    72,    23,    14,    73,    74,
      25,    86,    23,    25,    88,    20,    29,    85,    93,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    79,
      85,    85,    85,    85,    25,    84,    85,    79,    23,    16,
      85,    72,    76,    63,    63,    72,    34,    73,    85,    20,
      89,    20,    13,    58,    25,    94,    34,    83,    20,    45,
      52,    81,    23,    23,    23,    23,    86,    88,    23,    85,
      85,    93,    22,    23,    84,    85,    85,    34,    53,    23,
      87,    94,    41,    44,    51,    23,    79,    29,    18,    85,
      79,    79,    34,    13,    44,    34,    81,    23,    85,    79,
      23,    87,    34,    23
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    61,    62,    62,    62,    63,    64,    64,    65,    65,
      65,    66,    66,    67,    67,    67,    68,    68,    69,    69,
      70,    70,    71,    71,    71,    71,    72,    72,    72,    73,
      73,    74,    74,    75,    75,    76,    76,    77,    78,    78,
      79,    79,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    81,    81,    82,    82,
      83,    83,    84,    84,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    86,    86,    87,
      87,    88,    88,    89,    89,    89,    90,    90,    91,    92,
      93,    93,    94,    94,    95,    95
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     4,     1,     3,     4,     0,     2,     2,     2,
       2,     0,     2,     7,     5,     2,     0,     2,     4,     2,
       0,     2,     7,     5,     7,     5,     1,     3,     4,     0,
       2,     4,     2,     4,     2,     0,     3,     4,     0,     3,
       0,     2,     4,     3,     6,     3,     9,     7,     6,     4,
      12,    10,     2,     3,     2,     2,     0,     5,     4,     2,
       1,     1,     0,     3,     1,     1,     3,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     0,     3,     0,
       5,     0,     3,     1,     4,     3,     4,     2,     6,     4,
       3,     1,     0,     3,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: PROGRAM IS body SEMI  */
#line 64 "bison.y"
                              { (yyval.node) = newgram("program", (yyvsp[-3].node)->row, (yyvsp[-3].node)->col); addnode((yyval.node), (yyvsp[-1].node)); root = (yyval.node); }
#line 1423 "yacc.c"
    break;

  case 3: /* program: error  */
#line 65 "bison.y"
          { (yyval.node) = newgram("", row, col); }
#line 1429 "yacc.c"
    break;

  case 4: /* program: error body SEMI  */
#line 66 "bison.y"
                   { (yyval.node) = newgram("program_error", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); addnode((yyval.node), (yyvsp[-1].node)); root = (yyval.node); (yyval.node)->error = "Error"; }
#line 1435 "yacc.c"
    break;

  case 5: /* body: declaration_sm MYBEGIN statement_sm END  */
#line 68 "bison.y"
                                              { (yyval.node) = newgram("body", (yyvsp[-3].node)->row, (yyvsp[-3].node)->col); addnode((yyval.node), (yyvsp[-3].node)); addnode((yyval.node), (yyvsp[-1].node)); }
#line 1441 "yacc.c"
    break;

  case 6: /* declaration_sm: %empty  */
#line 71 "bison.y"
                { (yyval.node) = newgram("", row, col); }
#line 1447 "yacc.c"
    break;

  case 7: /* declaration_sm: declaration declaration_sm  */
#line 72 "bison.y"
                               { (yyval.node) = newgram("declaration_sm", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); addnode((yyval.node), (yyvsp[-1].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1453 "yacc.c"
    break;

  case 8: /* declaration: VAR var_decl_sm  */
#line 74 "bison.y"
                             { (yyval.node)  = newgram("declaration", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); addnode((yyval.node), (yyvsp[0].node)); }
#line 1459 "yacc.c"
    break;

  case 9: /* declaration: TYPE type_decl_sm  */
#line 75 "bison.y"
                      { (yyval.node) = newgram("declaration", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); addnode((yyval.node), (yyvsp[0].node));}
#line 1465 "yacc.c"
    break;

  case 10: /* declaration: PROCEDURE procedure_decl_sm  */
#line 76 "bison.y"
                                { (yyval.node) = newgram("declaration", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); addnode((yyval.node), (yyvsp[0].node)); }
#line 1471 "yacc.c"
    break;

  case 11: /* var_decl_sm: %empty  */
#line 79 "bison.y"
             { (yyval.node) = newgram("", row, col); }
#line 1477 "yacc.c"
    break;

  case 12: /* var_decl_sm: var_decl var_decl_sm  */
#line 80 "bison.y"
                         { (yyval.node) = newgram("var_decl_sm", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); addnode((yyval.node), (yyvsp[-1].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1483 "yacc.c"
    break;

  case 13: /* var_decl: ID id_comma_sm COLON type ASSIGN expression SEMI  */
#line 82 "bison.y"
                                                           { (yyval.node) = newgram("var_decl", (yyvsp[-6].node)->row, (yyvsp[-6].node)->col); addnode((yyval.node), (yyvsp[-6].node)); addnode((yyval.node), (yyvsp[-5].node)); addnode((yyval.node), (yyvsp[-3].node)); addnode((yyval.node), (yyvsp[-1].node));}
#line 1489 "yacc.c"
    break;

  case 14: /* var_decl: ID id_comma_sm ASSIGN expression SEMI  */
#line 83 "bison.y"
                                          { (yyval.node) = newgram("var_decl", (yyvsp[-4].node)->row, (yyvsp[-4].node)->col); addnode((yyval.node), (yyvsp[-4].node)); addnode((yyval.node), (yyvsp[-3].node)); addnode((yyval.node), (yyvsp[-1].node));}
#line 1495 "yacc.c"
    break;

  case 15: /* var_decl: error SEMI  */
#line 84 "bison.y"
               { (yyval.node) = newgram("var_decl_error", (yyvsp[0].node)->row, (yyvsp[0].node)->col); (yyval.node)->error = "Error"; }
#line 1501 "yacc.c"
    break;

  case 16: /* type_decl_sm: %empty  */
#line 87 "bison.y"
              { (yyval.node) = newgram("", row, col); }
#line 1507 "yacc.c"
    break;

  case 17: /* type_decl_sm: type_decl type_decl_sm  */
#line 88 "bison.y"
                           { (yyval.node) = newgram("type_decl_sm", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); addnode((yyval.node), (yyvsp[-1].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1513 "yacc.c"
    break;

  case 18: /* type_decl: ID IS type SEMI  */
#line 90 "bison.y"
                           { (yyval.node) = newgram("type_decl", (yyvsp[-3].node)->row, (yyvsp[-3].node)->col); addnode((yyval.node), (yyvsp[-3].node)); addnode((yyval.node), (yyvsp[-1].node)); }
#line 1519 "yacc.c"
    break;

  case 19: /* type_decl: error SEMI  */
#line 91 "bison.y"
               { (yyval.node) = newgram("type_decl_error", (yyvsp[0].node)->row, (yyvsp[0].node)->col); (yyval.node)->error = "Error"; }
#line 1525 "yacc.c"
    break;

  case 20: /* procedure_decl_sm: %empty  */
#line 94 "bison.y"
                   { (yyval.node) = newgram("", row, col); }
#line 1531 "yacc.c"
    break;

  case 21: /* procedure_decl_sm: procedure_decl procedure_decl_sm  */
#line 95 "bison.y"
                                     { (yyval.node) = newgram("procedure_decl_sm", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); addnode((yyval.node), (yyvsp[-1].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1537 "yacc.c"
    break;

  case 22: /* procedure_decl: ID formal_params COLON type IS body SEMI  */
#line 97 "bison.y"
                                                         { (yyval.node) = newgram("procedure_decl", (yyvsp[-6].node)->row, (yyvsp[-6].node)->col); addnode((yyval.node), (yyvsp[-6].node)); addnode((yyval.node), (yyvsp[-5].node)); addnode((yyval.node), (yyvsp[-3].node)); addnode((yyval.node), (yyvsp[-1].node)); }
#line 1543 "yacc.c"
    break;

  case 23: /* procedure_decl: ID formal_params IS body SEMI  */
#line 98 "bison.y"
                                  { (yyval.node) = newgram("procedure_decl", (yyvsp[-4].node)->row, (yyvsp[-4].node)->col); addnode((yyval.node), (yyvsp[-4].node)); addnode((yyval.node), (yyvsp[-3].node)); addnode((yyval.node), (yyvsp[-1].node)); }
#line 1549 "yacc.c"
    break;

  case 24: /* procedure_decl: ID formal_params COLON type error body SEMI  */
#line 99 "bison.y"
                                                { (yyval.node) = newgram("procedure_decl_error", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); addnode((yyval.node), (yyvsp[-6].node)); addnode((yyval.node), (yyvsp[-5].node)); addnode((yyval.node), (yyvsp[-3].node)); addnode((yyval.node), (yyvsp[-1].node)); (yyval.node)->error = "Error"; }
#line 1555 "yacc.c"
    break;

  case 25: /* procedure_decl: ID formal_params error body SEMI  */
#line 100 "bison.y"
                                     { (yyval.node) = newgram("procedure_decl_error", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); addnode((yyval.node), (yyvsp[-4].node)); addnode((yyval.node), (yyvsp[-3].node)); addnode((yyval.node), (yyvsp[-1].node)); (yyval.node)->error = "Error"; }
#line 1561 "yacc.c"
    break;

  case 26: /* type: ID  */
#line 103 "bison.y"
         { (yyval.node) = newgram("type", (yyvsp[0].node)->row, (yyvsp[0].node)->col); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = (yyvsp[0].node)->info; }
#line 1567 "yacc.c"
    break;

  case 27: /* type: ARRAY OF type  */
#line 104 "bison.y"
                  { (yyval.node) = newgram("type", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[0].node)); }
#line 1573 "yacc.c"
    break;

  case 28: /* type: RECORD component component_sm END  */
#line 105 "bison.y"
                                      { (yyval.node) = newgram("type", (yyvsp[-3].node)->row, (yyvsp[-3].node)->col); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[-1].node)); }
#line 1579 "yacc.c"
    break;

  case 29: /* component_sm: %empty  */
#line 108 "bison.y"
              { (yyval.node) = newgram("", row, col); }
#line 1585 "yacc.c"
    break;

  case 30: /* component_sm: component component_sm  */
#line 109 "bison.y"
                           { (yyval.node) = newgram("component_sm", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); addnode((yyval.node), (yyvsp[-1].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1591 "yacc.c"
    break;

  case 31: /* component: ID COLON type SEMI  */
#line 111 "bison.y"
                              { (yyval.node) = newgram("component", (yyvsp[-3].node)->row, (yyvsp[-3].node)->col); addnode((yyval.node), (yyvsp[-3].node)); addnode((yyval.node), (yyvsp[-1].node)); }
#line 1597 "yacc.c"
    break;

  case 32: /* component: error SEMI  */
#line 112 "bison.y"
               { (yyval.node) = newgram("component_error", (yyvsp[0].node)->row, (yyvsp[0].node)->col); (yyval.node)->error = "Error"; }
#line 1603 "yacc.c"
    break;

  case 33: /* formal_params: LPAREN fp_section fp_section_semicolon_sm RPAREN  */
#line 115 "bison.y"
                                                                 { (yyval.node) = newgram("formal_params", (yyvsp[-3].node)->row, (yyvsp[-3].node)->col); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[-1].node)); }
#line 1609 "yacc.c"
    break;

  case 34: /* formal_params: LPAREN RPAREN  */
#line 116 "bison.y"
                  { (yyval.node) = newgram("formal_params", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); }
#line 1615 "yacc.c"
    break;

  case 35: /* fp_section_semicolon_sm: %empty  */
#line 119 "bison.y"
                         { (yyval.node) = newgram("", row, col); }
#line 1621 "yacc.c"
    break;

  case 36: /* fp_section_semicolon_sm: SEMI fp_section fp_section_semicolon_sm  */
#line 120 "bison.y"
                                            { (yyval.node) = newgram("fp_section_semicolon_sm", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-1].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1627 "yacc.c"
    break;

  case 37: /* fp_section: ID id_comma_sm COLON type  */
#line 122 "bison.y"
                                      { (yyval.node) = newgram("fp_section", (yyvsp[-3].node)->row, (yyvsp[-3].node)->col); addnode((yyval.node), (yyvsp[-3].node)); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[0].node)); }
#line 1633 "yacc.c"
    break;

  case 38: /* id_comma_sm: %empty  */
#line 124 "bison.y"
             { (yyval.node) = newgram("", row, col); }
#line 1639 "yacc.c"
    break;

  case 39: /* id_comma_sm: COM ID id_comma_sm  */
#line 125 "bison.y"
                       { (yyval.node) = newgram("id_comma_sm", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-1].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1645 "yacc.c"
    break;

  case 40: /* statement_sm: %empty  */
#line 128 "bison.y"
              { (yyval.node) = newgram("", row, col); }
#line 1651 "yacc.c"
    break;

  case 41: /* statement_sm: statement statement_sm  */
#line 129 "bison.y"
                           { (yyval.node) = newgram("statement_sm", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); addnode((yyval.node), (yyvsp[-1].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1657 "yacc.c"
    break;

  case 42: /* statement: l_value ASSIGN expression SEMI  */
#line 131 "bison.y"
                                          { (yyval.node) = newgram("statement", (yyvsp[-3].node)->row, (yyvsp[-3].node)->col); addnode((yyval.node), (yyvsp[-3].node)); addnode((yyval.node), (yyvsp[-1].node)); }
#line 1663 "yacc.c"
    break;

  case 43: /* statement: ID actual_params SEMI  */
#line 132 "bison.y"
                          { (yyval.node) = newgram("statement", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[-1].node)); }
#line 1669 "yacc.c"
    break;

  case 44: /* statement: READ LPAREN l_value l_value_comma_sm RPAREN SEMI  */
#line 133 "bison.y"
                                                     { (yyval.node) = newgram("statement", (yyvsp[-5].node)->row, (yyvsp[-5].node)->col); addnode((yyval.node), (yyvsp[-3].node)); addnode((yyval.node), (yyvsp[-2].node)); }
#line 1675 "yacc.c"
    break;

  case 45: /* statement: WRITE write_params SEMI  */
#line 134 "bison.y"
                            { (yyval.node) = newgram("statement", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-1].node)); }
#line 1681 "yacc.c"
    break;

  case 46: /* statement: IF expression THEN statement_sm elseif_then_sm ELSE statement_sm END SEMI  */
#line 135 "bison.y"
                                                                              { (yyval.node) = newgram("statement", (yyvsp[-8].node)->row, (yyvsp[-8].node)->col); addnode((yyval.node), (yyvsp[-7].node)); addnode((yyval.node), (yyvsp[-5].node)); addnode((yyval.node), (yyvsp[-4].node)); addnode((yyval.node), (yyvsp[-2].node)); }
#line 1687 "yacc.c"
    break;

  case 47: /* statement: IF expression THEN statement_sm elseif_then_sm END SEMI  */
#line 136 "bison.y"
                                                            { (yyval.node) = newgram("statement", (yyvsp[-6].node)->row, (yyvsp[-6].node)->col); addnode((yyval.node), (yyvsp[-5].node)); addnode((yyval.node), (yyvsp[-3].node)); addnode((yyval.node), (yyvsp[-2].node)); }
#line 1693 "yacc.c"
    break;

  case 48: /* statement: WHILE expression DO statement_sm END SEMI  */
#line 137 "bison.y"
                                              { (yyval.node) = newgram("statement", (yyvsp[-5].node)->row, (yyvsp[-5].node)->col); addnode((yyval.node), (yyvsp[-4].node)); addnode((yyval.node), (yyvsp[-2].node)); }
#line 1699 "yacc.c"
    break;

  case 49: /* statement: LOOP statement_sm END SEMI  */
#line 138 "bison.y"
                               { (yyval.node) = newgram("statement", (yyvsp[-3].node)->row, (yyvsp[-3].node)->col); addnode((yyval.node), (yyvsp[-2].node)); }
#line 1705 "yacc.c"
    break;

  case 50: /* statement: FOR ID ASSIGN expression TO expression BY expression DO statement_sm END SEMI  */
#line 139 "bison.y"
                                                                                  { (yyval.node) = newgram("statement", (yyvsp[-11].node)->row, (yyvsp[-11].node)->col); addnode((yyval.node), (yyvsp[-10].node)); addnode((yyval.node), (yyvsp[-8].node)); addnode((yyval.node), (yyvsp[-6].node)); addnode((yyval.node), (yyvsp[-4].node)); addnode((yyval.node), (yyvsp[-2].node)); }
#line 1711 "yacc.c"
    break;

  case 51: /* statement: FOR ID ASSIGN expression TO expression DO statement_sm END SEMI  */
#line 140 "bison.y"
                                                                    { (yyval.node) = newgram("statement", (yyvsp[-9].node)->row, (yyvsp[-9].node)->col); addnode((yyval.node), (yyvsp[-8].node)); addnode((yyval.node), (yyvsp[-6].node)); addnode((yyval.node), (yyvsp[-4].node)); addnode((yyval.node), (yyvsp[-2].node)); }
#line 1717 "yacc.c"
    break;

  case 52: /* statement: EXIT SEMI  */
#line 141 "bison.y"
              { (yyval.node) = newgram("statement", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); }
#line 1723 "yacc.c"
    break;

  case 53: /* statement: RETURN expression SEMI  */
#line 142 "bison.y"
                           { (yyval.node) = newgram("statement", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-1].node)); }
#line 1729 "yacc.c"
    break;

  case 54: /* statement: RETURN SEMI  */
#line 143 "bison.y"
                { (yyval.node) = newgram("statement", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); }
#line 1735 "yacc.c"
    break;

  case 55: /* statement: error SEMI  */
#line 144 "bison.y"
               { (yyval.node) = newgram("statement_error", (yyvsp[0].node)->row, (yyvsp[0].node)->col); (yyval.node)->error = "Error"; }
#line 1741 "yacc.c"
    break;

  case 56: /* elseif_then_sm: %empty  */
#line 146 "bison.y"
                { (yyval.node) = newgram("", row, col); }
#line 1747 "yacc.c"
    break;

  case 57: /* elseif_then_sm: ELSIF expression THEN statement_sm elseif_then_sm  */
#line 147 "bison.y"
                                                      { (yyval.node) = newgram("elseif_then_sm", (yyvsp[-4].node)->row, (yyvsp[-4].node)->col); addnode((yyval.node), (yyvsp[-3].node)); addnode((yyval.node), (yyvsp[-1].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1753 "yacc.c"
    break;

  case 58: /* write_params: LPAREN write_expr write_expr_comma_sm RPAREN  */
#line 150 "bison.y"
                                                           { (yyval.node) = newgram("write_params", (yyvsp[-3].node)->row, (yyvsp[-3].node)->col); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[-1].node)); }
#line 1759 "yacc.c"
    break;

  case 59: /* write_params: LPAREN RPAREN  */
#line 151 "bison.y"
                  { (yyval.node) = newgram("write_params", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); }
#line 1765 "yacc.c"
    break;

  case 60: /* write_expr: STRING  */
#line 153 "bison.y"
                   { (yyval.node) = newgram("write_expr", (yyvsp[0].node)->row, (yyvsp[0].node)->col); addnode((yyval.node), (yyvsp[0].node)); }
#line 1771 "yacc.c"
    break;

  case 61: /* write_expr: expression  */
#line 154 "bison.y"
               { (yyval.node) = newgram("write_expr", (yyvsp[0].node)->row, (yyvsp[0].node)->col); addnode((yyval.node), (yyvsp[0].node)); }
#line 1777 "yacc.c"
    break;

  case 62: /* write_expr_comma_sm: %empty  */
#line 156 "bison.y"
                     { (yyval.node) = newgram("", row, col); }
#line 1783 "yacc.c"
    break;

  case 63: /* write_expr_comma_sm: COM write_expr write_expr_comma_sm  */
#line 157 "bison.y"
                                       { (yyval.node) = newgram("write_expr_comma_sm", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-1].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1789 "yacc.c"
    break;

  case 64: /* expression: number  */
#line 160 "bison.y"
                   { (yyval.node) = newgram("expression", (yyvsp[0].node)->row, (yyvsp[0].node)->col); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = (yyvsp[0].node)->info; }
#line 1795 "yacc.c"
    break;

  case 65: /* expression: l_value  */
#line 161 "bison.y"
            { (yyval.node) = newgram("expression", (yyvsp[0].node)->row, (yyvsp[0].node)->col); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = (yyvsp[0].node)->info; }
#line 1801 "yacc.c"
    break;

  case 66: /* expression: LPAREN expression RPAREN  */
#line 162 "bison.y"
                             { (yyval.node) = newgram("expression", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-1].node)); (yyval.node)->info = "("+(yyvsp[-1].node)->info+")"; }
#line 1807 "yacc.c"
    break;

  case 67: /* expression: ADD expression  */
#line 163 "bison.y"
                             { (yyval.node) = newgram("expression", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = "POS"+(yyvsp[0].node)->info; }
#line 1813 "yacc.c"
    break;

  case 68: /* expression: SUB expression  */
#line 164 "bison.y"
                             { (yyval.node) = newgram("expression", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = "NEG"+(yyvsp[0].node)->info; }
#line 1819 "yacc.c"
    break;

  case 69: /* expression: NOT expression  */
#line 165 "bison.y"
                   { (yyval.node) = newgram("expression", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = "NOT"+(yyvsp[0].node)->info; }
#line 1825 "yacc.c"
    break;

  case 70: /* expression: expression ADD expression  */
#line 166 "bison.y"
                              { (yyval.node) = newgram("expression", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = (yyvsp[-2].node)->info+"ADD"+(yyvsp[0].node)->info; }
#line 1831 "yacc.c"
    break;

  case 71: /* expression: expression SUB expression  */
#line 167 "bison.y"
                              { (yyval.node) = newgram("expression", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = (yyvsp[-2].node)->info+"SUB"+(yyvsp[0].node)->info; }
#line 1837 "yacc.c"
    break;

  case 72: /* expression: expression MUL expression  */
#line 168 "bison.y"
                              { (yyval.node) = newgram("expression", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = (yyvsp[-2].node)->info+"MUL"+(yyvsp[0].node)->info; }
#line 1843 "yacc.c"
    break;

  case 73: /* expression: expression LUM expression  */
#line 169 "bison.y"
                              { (yyval.node) = newgram("expression", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = (yyvsp[-2].node)->info+"LUM"+(yyvsp[0].node)->info; }
#line 1849 "yacc.c"
    break;

  case 74: /* expression: expression DIV expression  */
#line 170 "bison.y"
                              { (yyval.node) = newgram("expression", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = (yyvsp[-2].node)->info+"DIV"+(yyvsp[0].node)->info; }
#line 1855 "yacc.c"
    break;

  case 75: /* expression: expression MOD expression  */
#line 171 "bison.y"
                              { (yyval.node) = newgram("expression", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = (yyvsp[-2].node)->info+"MOD"+(yyvsp[0].node)->info; }
#line 1861 "yacc.c"
    break;

  case 76: /* expression: expression OR expression  */
#line 172 "bison.y"
                             { (yyval.node) = newgram("expression", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = (yyvsp[-2].node)->info+"OR"+(yyvsp[0].node)->info; }
#line 1867 "yacc.c"
    break;

  case 77: /* expression: expression AND expression  */
#line 173 "bison.y"
                              { (yyval.node) = newgram("expression", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = (yyvsp[-2].node)->info+"AND"+(yyvsp[0].node)->info; }
#line 1873 "yacc.c"
    break;

  case 78: /* expression: expression GT expression  */
#line 174 "bison.y"
                             { (yyval.node) = newgram("expression", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = (yyvsp[-2].node)->info+"GT"+(yyvsp[0].node)->info; }
#line 1879 "yacc.c"
    break;

  case 79: /* expression: expression LT expression  */
#line 175 "bison.y"
                             { (yyval.node) = newgram("expression", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = (yyvsp[-2].node)->info+"LT"+(yyvsp[0].node)->info; }
#line 1885 "yacc.c"
    break;

  case 80: /* expression: expression EQ expression  */
#line 176 "bison.y"
                             { (yyval.node) = newgram("expression", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = (yyvsp[-2].node)->info+"EQ"+(yyvsp[0].node)->info; }
#line 1891 "yacc.c"
    break;

  case 81: /* expression: expression GE expression  */
#line 177 "bison.y"
                             { (yyval.node) = newgram("expression", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = (yyvsp[-2].node)->info+"GE"+(yyvsp[0].node)->info; }
#line 1897 "yacc.c"
    break;

  case 82: /* expression: expression LE expression  */
#line 178 "bison.y"
                             { (yyval.node) = newgram("expression", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = (yyvsp[-2].node)->info+"LE"+(yyvsp[0].node)->info; }
#line 1903 "yacc.c"
    break;

  case 83: /* expression: expression NE expression  */
#line 179 "bison.y"
                             { (yyval.node) = newgram("expression", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = (yyvsp[-2].node)->info+"NE"+(yyvsp[0].node)->info; }
#line 1909 "yacc.c"
    break;

  case 84: /* expression: ID actual_params  */
#line 180 "bison.y"
                     { (yyval.node) = newgram("expression", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); addnode((yyval.node), (yyvsp[-1].node)); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = (yyvsp[-1].node)->info+(yyvsp[0].node)->info; }
#line 1915 "yacc.c"
    break;

  case 85: /* expression: ID comp_values  */
#line 181 "bison.y"
                   { (yyval.node) = newgram("expression", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); addnode((yyval.node), (yyvsp[-1].node)); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = (yyvsp[-1].node)->info+(yyvsp[0].node)->info; }
#line 1921 "yacc.c"
    break;

  case 86: /* expression: ID array_values  */
#line 182 "bison.y"
                    { (yyval.node) = newgram("expression", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); addnode((yyval.node), (yyvsp[-1].node)); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = (yyvsp[-1].node)->info+(yyvsp[0].node)->info; }
#line 1927 "yacc.c"
    break;

  case 87: /* expression_comma_sm: %empty  */
#line 184 "bison.y"
                     { (yyval.node) = newgram("", row, col); }
#line 1933 "yacc.c"
    break;

  case 88: /* expression_comma_sm: COM expression expression_comma_sm  */
#line 185 "bison.y"
                                       { (yyval.node) = newgram("expression_comma_sm", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-1].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1939 "yacc.c"
    break;

  case 89: /* expression_id_semicolon_assign_sm: %empty  */
#line 187 "bison.y"
                                   { (yyval.node) = newgram("", row, col); }
#line 1945 "yacc.c"
    break;

  case 90: /* expression_id_semicolon_assign_sm: SEMI ID ASSIGN expression expression_id_semicolon_assign_sm  */
#line 188 "bison.y"
                                                                { (yyval.node) = newgram("expression_id_semicolon_assign_sm", (yyvsp[-4].node)->row, (yyvsp[-4].node)->col); addnode((yyval.node), (yyvsp[-3].node)); addnode((yyval.node), (yyvsp[-1].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1951 "yacc.c"
    break;

  case 91: /* l_value_comma_sm: %empty  */
#line 191 "bison.y"
                  { (yyval.node) = newgram("", row, col); }
#line 1957 "yacc.c"
    break;

  case 92: /* l_value_comma_sm: COM l_value l_value_comma_sm  */
#line 192 "bison.y"
                                 { (yyval.node) = newgram("l_value_comma_sm", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-1].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1963 "yacc.c"
    break;

  case 93: /* l_value: ID  */
#line 194 "bison.y"
            { (yyval.node) = newgram("l_value", (yyvsp[0].node)->row, (yyvsp[0].node)->col); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = (yyvsp[0].node)->info; }
#line 1969 "yacc.c"
    break;

  case 94: /* l_value: l_value LS expression RS  */
#line 195 "bison.y"
                             { (yyval.node) = newgram("l_value", (yyvsp[-3].node)->row, (yyvsp[-3].node)->col); addnode((yyval.node), (yyvsp[-3].node)); addnode((yyval.node), (yyvsp[-1].node)); (yyval.node)->info = (yyvsp[-3].node)->info+"["+(yyvsp[-1].node)->info+"]"; }
#line 1975 "yacc.c"
    break;

  case 95: /* l_value: l_value DOT ID  */
#line 196 "bison.y"
                   { (yyval.node) = newgram("l_value", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[0].node)); (yyval.node)->info = (yyvsp[-2].node)->info+"."+(yyvsp[0].node)->info; }
#line 1981 "yacc.c"
    break;

  case 96: /* actual_params: LPAREN expression expression_comma_sm RPAREN  */
#line 198 "bison.y"
                                                            { (yyval.node) = newgram("actual_params", (yyvsp[-3].node)->row, (yyvsp[-3].node)->col); addnode((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[-1].node)); }
#line 1987 "yacc.c"
    break;

  case 97: /* actual_params: LPAREN RPAREN  */
#line 199 "bison.y"
                  { (yyval.node) = newgram("actual_params", (yyvsp[-1].node)->row, (yyvsp[-1].node)->col); }
#line 1993 "yacc.c"
    break;

  case 98: /* comp_values: LP ID ASSIGN expression expression_id_semicolon_assign_sm RP  */
#line 202 "bison.y"
                                                                          { (yyval.node) = newgram("comp_values", (yyvsp[-5].node)->row, (yyvsp[-5].node)->col); addnode((yyval.node), (yyvsp[-4].node)); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[-1].node)); (yyval.node)->info = "{"+(yyvsp[-4].node)->info+":="+(yyvsp[-2].node)->info+(yyvsp[-1].node)->info+"}"; }
#line 1999 "yacc.c"
    break;

  case 99: /* array_values: LARRAY array_value array_value_comma_sm RARRAY  */
#line 204 "bison.y"
                                                             { (yyval.node) = newgram("array_values", (yyvsp[-3].node)->row, (yyvsp[-3].node)->col); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[-1].node)); }
#line 2005 "yacc.c"
    break;

  case 100: /* array_value: expression OF expression  */
#line 206 "bison.y"
                                      { (yyval.node) = newgram("array_value", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-2].node)); addnode((yyval.node), (yyvsp[0].node)); }
#line 2011 "yacc.c"
    break;

  case 101: /* array_value: expression  */
#line 207 "bison.y"
               { (yyval.node) = newgram("array_value", (yyvsp[0].node)->row, (yyvsp[0].node)->col); addnode((yyval.node), (yyvsp[0].node)); }
#line 2017 "yacc.c"
    break;

  case 102: /* array_value_comma_sm: %empty  */
#line 209 "bison.y"
                      { (yyval.node) = newgram("", row, col); }
#line 2023 "yacc.c"
    break;

  case 103: /* array_value_comma_sm: COM array_value array_value_comma_sm  */
#line 210 "bison.y"
                                         { (yyval.node) = newgram("array_value_comma_sm", (yyvsp[-2].node)->row, (yyvsp[-2].node)->col); addnode((yyval.node), (yyvsp[-1].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 2029 "yacc.c"
    break;

  case 104: /* number: INTEGER  */
#line 212 "bison.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2035 "yacc.c"
    break;

  case 105: /* number: REAL  */
#line 213 "bison.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 2041 "yacc.c"
    break;


#line 2045 "yacc.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 215 "bison.y"

