/*
  regex.h - POSIX.2 compatible regexp interface and TRE extensions

  Copyright (C) 2001-2004 Ville Laurikari <vl@iki.fi>.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 2 (June
  1991) as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#ifndef TRE_REGEX_H
#define TRE_REGEX_H 1

#ifndef __GNUC__
# define __DLL_IMPORT__  __declspec(dllimport)
# define __DLL_EXPORT__  __declspec(dllexport)
#else
# define __DLL_IMPORT__  __attribute__((dllimport)) extern
# define __DLL_EXPORT__  __attribute__((dllexport)) extern
#endif 

#if (defined __WIN32__) || (defined _WIN32)
# if defined BUILD_LIBTRE_DLL || defined TRE_EXPORTS
#  define LIBTRE_DLL_IMPEXP     __DLL_EXPORT__
# elif defined(LIBTRE_STATIC)
#  define LIBTRE_DLL_IMPEXP      
# elif defined (USE_LIBTRE_DLL)
#  define LIBTRE_DLL_IMPEXP     __DLL_IMPORT__
# elif defined (USE_LIBTRE_STATIC)
#  define LIBTRE_DLL_IMPEXP      
# else /* assume USE_LIBTRE_DLL */
#  define LIBTRE_DLL_IMPEXP     __DLL_IMPORT__
# endif
#else /* __WIN32__ */
# define LIBTRE_DLL_IMPEXP  
#endif

#include "tre-config.h"

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */

#ifdef HAVE_LIBUTF8_H
#include <libutf8.h>
#endif /* HAVE_LIBUTF8_H */

#ifdef TRE_USE_SYSTEM_REGEX_H
/* Include the system regex.h to make TRE ABI compatible with the
   system regex. */
#include TRE_SYSTEM_REGEX_H_PATH
#endif /* TRE_USE_SYSTEM_REGEX_H */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TRE_USE_SYSTEM_REGEX_H

#ifndef REG_OK
#define REG_OK 0
#endif /* !REG_OK */

#ifndef HAVE_REG_ERRCODE_T
typedef int reg_errcode_t;
#endif /* !HAVE_REG_ERRCODE_T */

#if !defined(REG_NOSPEC) && !defined(REG_LITERAL)
#define REG_LITERAL 0x1000
#endif

/* Extra regcomp() flags. */
#define REG_RIGHT_ASSOC (REG_LITERAL << 1)

/* Extra regexec() flags. */
#define REG_APPROX_MATCHER	 0x1000
#define REG_BACKTRACKING_MATCHER (REG_APPROX_MATCHER << 1)

#else /* !TRE_USE_SYSTEM_REGEX_H */

/* If the we're not using system regex.h, we need to define the
   structs and enums ourselves. */

typedef int regoff_t;
typedef struct {
  size_t re_nsub;  /* Number of parenthesized subexpressions. */
  void *value;	   /* For internal use only. */
} regex_t;

typedef struct {
  regoff_t rm_so;
  regoff_t rm_eo;
} regmatch_t;


typedef enum {
  REG_OK = 0,		/* No error. */
  /* POSIX regcomp() return error codes.  (In the order listed in the
     standard.)	 */
  REG_NOMATCH,		/* No match. */
  REG_BADPAT,		/* Invalid regexp. */
  REG_ECOLLATE,		/* Unknown collating element. */
  REG_ECTYPE,		/* Unknown character class name. */
  REG_EESCAPE,		/* Trailing backslash. */
  REG_ESUBREG,		/* Invalid back reference. */
  REG_EBRACK,		/* "[]" imbalance */
  REG_EPAREN,		/* "\(\)" or "()" imbalance */
  REG_EBRACE,		/* "\{\}" or "{}" imbalance */
  REG_BADBR,		/* Invalid content of {} */
  REG_ERANGE,		/* Invalid use of range operator */
  REG_ESPACE,		/* Out of memory.  */
  REG_BADRPT
} reg_errcode_t;

/* POSIX regcomp() flags. */
#define REG_EXTENDED	1
#define REG_ICASE	(REG_EXTENDED << 1)
#define REG_NEWLINE	(REG_ICASE << 1)
#define REG_NOSUB	(REG_NEWLINE << 1)

/* Extra regcomp() flags. */
#define REG_BASIC	0
#define REG_LITERAL	(REG_NOSUB << 1)
#define REG_RIGHT_ASSOC (REG_LITERAL << 1)

/* POSIX regexec() flags. */
#define REG_NOTBOL 1
#define REG_NOTEOL (REG_NOTBOL << 1)

/* Extra regexec() flags. */
#define REG_APPROX_MATCHER	 (REG_NOTEOL << 1)
#define REG_BACKTRACKING_MATCHER (REG_APPROX_MATCHER << 1)

#endif /* !TRE_USE_SYSTEM_REGEX_H */

/* REG_NOSPEC and REG_LITERAL mean the same thing. */
#ifdef REG_LITERAL
#define REG_NOSPEC	REG_LITERAL
#elif defined(REG_NOSPEC)
#define REG_LITERAL	REG_NOSPEC
#endif /* defined(REG_NOSPEC) */

/* The maximum number of iterations in a bound expression. */
#undef RE_DUP_MAX
#define RE_DUP_MAX 255

/* The POSIX.2 regexp functions */
LIBTRE_DLL_IMPEXP int regcomp(regex_t *preg, const char *regex, int cflags);
LIBTRE_DLL_IMPEXP int regexec(const regex_t *preg, const char *string, size_t nmatch,
	    regmatch_t pmatch[], int eflags);
LIBTRE_DLL_IMPEXP size_t regerror(int errcode, const regex_t *preg, char *errbuf,
		size_t errbuf_size);
LIBTRE_DLL_IMPEXP void regfree(regex_t *preg);

#ifdef TRE_WCHAR
#ifdef HAVE_WCHAR_H
#include <wchar.h>
#endif /* HAVE_WCHAR_H */

/* Wide character versions (not in POSIX.2). */
LIBTRE_DLL_IMPEXP int regwcomp(regex_t *preg, const wchar_t *regex, int cflags);
LIBTRE_DLL_IMPEXP int regwexec(const regex_t *preg, const wchar_t *string, size_t nmatch,
	     regmatch_t pmatch[], int eflags);
#endif /* TRE_WCHAR */

/* Versions with a maximum length argument and therefore the capability to
   handle null characters in the middle of the strings (not in POSIX.2). */
LIBTRE_DLL_IMPEXP int regncomp(regex_t *preg, const char *regex, size_t len, int cflags);
LIBTRE_DLL_IMPEXP int regnexec(const regex_t *preg, const char *string, size_t len,
	     size_t nmatch, regmatch_t pmatch[], int eflags);
#ifdef TRE_WCHAR
LIBTRE_DLL_IMPEXP int regwncomp(regex_t *preg, const wchar_t *regex, size_t len, int cflags);
LIBTRE_DLL_IMPEXP int regwnexec(const regex_t *preg, const wchar_t *string, size_t len,
	      size_t nmatch, regmatch_t pmatch[], int eflags);
#endif /* TRE_WCHAR */

#ifdef TRE_APPROX

/* Approximate matching parameter struct. */
typedef struct {
  int cost_ins;	       /* Default cost of an inserted character. */
  int cost_del;	       /* Default cost of a deleted character. */
  int cost_subst;      /* Default cost of a substituted character. */
  int max_cost;	       /* Maximum allowed cost of a match. */

  int max_ins;	       /* Maximum allowed number of inserts. */
  int max_del;	       /* Maximum allowed number of deletes. */
  int max_subst;       /* Maximum allowed number of substitutes. */
  int max_err;	       /* Maximum allowed number of errors total. */
} regaparams_t;

/* Approximate matching result struct. */
typedef struct {
  size_t nmatch;       /* Length of pmatch[] array. */
  regmatch_t *pmatch;  /* Submatch data. */
  int cost;	       /* Cost of the match. */
  int num_ins;	       /* Number of inserts in the match. */
  int num_del;	       /* Number of deletes in the match. */
  int num_subst;       /* Number of substitutes in the match. */
} regamatch_t;


/* Approximate matching functions. */
LIBTRE_DLL_IMPEXP int regaexec(const regex_t *preg, const char *string,
	     regamatch_t *match, regaparams_t params, int eflags);
LIBTRE_DLL_IMPEXP int reganexec(const regex_t *preg, const char *string, size_t len,
	      regamatch_t *match, regaparams_t params, int eflags);
#ifdef TRE_WCHAR
/* Wide character approximate matching. */
LIBTRE_DLL_IMPEXP int regawexec(const regex_t *preg, const wchar_t *string,
	      regamatch_t *match, regaparams_t params, int eflags);
LIBTRE_DLL_IMPEXP int regawnexec(const regex_t *preg, const wchar_t *string, size_t len,
	       regamatch_t *match, regaparams_t params, int eflags);
#endif /* TRE_WCHAR */

/* Sets the parameters to default values. */
LIBTRE_DLL_IMPEXP void regaparams_default(regaparams_t *params);
#endif /* TRE_APPROX */

#ifdef TRE_WCHAR
typedef wchar_t tre_char_t;
#else /* !TRE_WCHAR */
typedef unsigned char tre_char_t;
#endif /* !TRE_WCHAR */

typedef struct {
  int (*get_next_char)(tre_char_t *c, unsigned int *pos_add, void *context);
  void (*rewind)(size_t pos, void *context);
  int (*compare)(size_t pos1, size_t pos2, size_t len, void *context);
  void *context;
} tre_str_source;

LIBTRE_DLL_IMPEXP int reguexec(const regex_t *preg, const tre_str_source *string,
	     size_t nmatch, regmatch_t pmatch[], int eflags);

/* Returns the version string.	The returned string is static. */
LIBTRE_DLL_IMPEXP char *tre_version(void);

/* Returns the value for a config parameter.  The type to which `result'
   must point to depends of the value of `query', see documentation for
   more details. */
LIBTRE_DLL_IMPEXP int tre_config(int query, void *result);

enum {
  TRE_CONFIG_APPROX,
  TRE_CONFIG_WCHAR,
  TRE_CONFIG_MULTIBYTE,
  TRE_CONFIG_SYSTEM_ABI,
  TRE_CONFIG_VERSION
};

/* Returns 1 if the compiled pattern has back references, 0 if not. */
LIBTRE_DLL_IMPEXP int tre_have_backrefs(const regex_t *preg);

/* Returns 1 if the compiled pattern uses approximate matching features,
   0 if not. */
LIBTRE_DLL_IMPEXP int tre_have_approx(const regex_t *preg);

#ifdef __cplusplus
}
#endif
#endif				/* TRE_REGEX_H */

/* EOF */
