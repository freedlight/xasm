// xasm.h - constants and variables for generic cross assembler

#include <stdio.h>
#include <ctype.h>
#include <alloc.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char	boolean;
typedef unsigned short	WORD;

#ifndef TRUE
#define TRUE	1
#define FALSE	0
#endif

#include "hash.h"
#include "cputype.h"

//***********************
// configuration options
//***********************

#define IBM TRUE
#define MAC FALSE
#define MONITOR FALSE
#define BIGBUF FALSE
#define VERSION "3.9"

//*******************
// lengths and sizes
//*******************

#define HTABSIZ 401	// size of hash table
#define SYMLEN 16	// length of symbol
#define MAXMAC 100	// size of macro table
#define NAMELEN 20	// length of file name
#define WIDTH 80	// length of input line
#define UNUSED 300	// value for unused data byte
#define BSIZE 8192	// size of large file buffers

//*****************
// data structures
//*****************

typedef	unsigned long ADDRESS;

// macro storage
typedef struct _Macr {
	char mnam[SYMLEN];
	char mtxt[WIDTH];
	struct _Macr *mnxt;
} MACR;

// symbol storage
typedef struct {
	ADDRESS syma;	// symbol value
} STAB;

// vector tables
typedef void (* proc)(void);
typedef struct {
	char *name;
	proc  func;
} vecdef;

//***********
// macros
//***********

#define dim(s)			(sizeof(s)/sizeof(s[0]))
#define streq(s1,s2)	(strcmp((s1),(s2)) == 0)
#define atoh(s)			(strtol(s, NULL, 16))

//***********
// variables
//***********

#ifdef __MAIN
#define	GLOBAL	
#else
#define	GLOBAL	extern
#endif

GLOBAL FILE		*inpfile;			// source file
GLOBAL FILE		*incfile;			// include file
GLOBAL FILE		*outfile;			// output file
GLOBAL FILE		*lstfile;			// listing file
GLOBAL FILE		*symfile;			// symbol file
GLOBAL FILE		*errfile;			// error file
extern FILE		*lp;				// printer - defined in library

GLOBAL MACR		*mptr;				// current macro data ptr
GLOBAL MACR		*mac[MAXMAC];		// macro data ptrs
GLOBAL HASH_TAB *htab;				// hashtable pointer

GLOBAL char		fname[NAMELEN];		// source filename
GLOBAL char		iname[NAMELEN];		// include filename
GLOBAL char		*lbl;
GLOBAL char		*op;
GLOBAL char		*arg;
GLOBAL char		cpu[10];
GLOBAL char		stmt[WIDTH+1];
GLOBAL char		orig[WIDTH+1];
GLOBAL char		*scanptr;
GLOBAL char		*argend;
GLOBAL char		*symname;
GLOBAL char		lbuf[SYMLEN];
GLOBAL char		lastlab[SYMLEN];
GLOBAL char		*margs[10];
GLOBAL char		argb[40];
extern char		hx[];				// in libraries

GLOBAL ADDRESS	maxmem;
GLOBAL ADDRESS	pc;
GLOBAL ADDRESS	ipc;
GLOBAL ADDRESS	freespc;

GLOBAL short	apass;
GLOBAL short	maccnt;
GLOBAL short	symcnt;
GLOBAL short	REVFLAG;
GLOBAL short	OBJFLAG;
GLOBAL short	LFLAG;
GLOBAL short	DFLAG;
GLOBAL short	UFLAG;
GLOBAL short	DEBUGF;
GLOBAL short	errcnt;
GLOBAL short	serr;
GLOBAL short	maxrel;
GLOBAL short	minrel;
GLOBAL short	outcnt;
GLOBAL short	cpuval;
GLOBAL short	cpusub;
GLOBAL short	chks;
GLOBAL short	fillc;
GLOBAL short	FIRSTOUT;
GLOBAL short	LSTON;

GLOBAL WORD	autol;
GLOBAL WORD	fline;
GLOBAL WORD	iline;
GLOBAL WORD	oline;

GLOBAL boolean	INCLFLAG;
GLOBAL boolean	APPEFLAG;
GLOBAL boolean	IFFON;
GLOBAL boolean	MACON;
GLOBAL boolean	MACACT;
GLOBAL boolean	FIRSTORG;
GLOBAL boolean	LSTF;
GLOBAL boolean	SYMON;
GLOBAL boolean	SYMF;
GLOBAL boolean	ENDFLAG;
GLOBAL boolean	ERRMSG;
GLOBAL boolean	ERRPAUS;
GLOBAL boolean	ERRON;
GLOBAL boolean	ERRF;

//*********************
// function prototypes
//*********************

void aorerr(void);										// xasmerr
void appopen(char *name);								// xasminp
void argerr(void);										// xasmerr
void cduerr(void);										// xasmerr
void cimerr(void);										// xasmerr
void cpupars(void);										// xasm
void cpuerr(void);										// xasmerr
void defext(char *s, char *ext);						// xasm
void direrr(void);										// xasmerr
void dsmerr(void);										// xasmerr
void erropen(char *name);								// xasmout
short findmac(char *s);									// xasmmac
void fnderr(char *s);									// xasmerr
void gen1b(WORD op);										// xasmgen
void gen1db(WORD op);									// xasmgen
void gen2b(WORD i, WORD d);							// xasmgen
void gen2db(WORD i, WORD d);							// xasmgen
void gen3b(WORD i, WORD d);							// xasmgen
void gen3br(WORD i, WORD d);							// xasmgen
void gen3db(WORD i, WORD d);							// xasmgen
void gen3dsb(WORD op, WORD da1, WORD da2);		// xasmgen
void gen3sb(WORD opc, WORD da1, WORD da2);		// xasmgen
void gen4db(WORD op, WORD da1, WORD da2,			// xasmgen
	WORD da3);									 			// xasmgen
void gendb(WORD i);										// xasmgen
void genlbl(ADDRESS lblval, char *lblstr,			// xasmsym
	boolean needslbl);									// xasmsym
void genmac(char *xs);									// xasmmac
void genpars(void);										// xasm
void genrb(WORD i, ADDRESS d);						// xasmgen
boolean getlbl(char *lblstr, ADDRESS *lblval);	// xasmsym
boolean getword(char *lblstr, WORD *lblval);		// xasmsym
void getmac(short n);									// xasmmac
void getmparm(char *s);									// xasmmac
boolean getstmt(void);									// xasminp
void help(void);											// xasmout
void incerr(char *s);									// xasmerr
void inclpush(char *name);								// xasminp
void inpopen(char *name);								// xasminp
void ioverr(void);										// xasmerr
void lstopen(char *name);								// xasmout
void macerr(void);										// xasmerr
void maclist(void);										// xasmmac
ADDRESS makenum(char *s);								// xasmsym
short make_rel(ADDRESS da, short off);				// xasmgen
void miserr(void);										// xasmerr
void moderr(void);										// xasmerr
void nocerr(void);										// xasmerr
void nolerr(void);										// xasmerr
void norerr(void);										// xasmerr
void oorerr(WORD val, WORD mn, WORD mx);			// xasmerr
void opcerr(void);										// xasmerr
void opnerr(char *s);									// xasmerr
void orgerr(void);										// xasmerr
void outbyte(short dat);								// xasmout
void outend(void);										// xasmout
void outhpag(short pg);									// xasmout
void outopen(char *name);								// xasmout
void phaerr(void);										// xasmerr
void prblank(void);										// xasmout
void prdata(WORD i, WORD j, WORD k);				// xasmout
void prequ(short i);										// xasmout
void procdirect(void);									// xasmdir
void putmac(void);										// xasmmac
void pagerr(WORD val);									// xasmerr
void regerr(short val);									// xasmerr
void relerr(WORD val);									// xasmerr
void rmext(char *s);										// xasm
char *scan(void);											// xasminp
char *scanarg(void);										// xasminp
char *scancom(void);										// xasminp
void symerr(void);										// xasmerr
void symlist(void);										// xasmout
void symopen(char *name);								// xasmout
void test_byte(WORD *da);								// xasmgen
void test_oor(short *da, short lo, short hi);	// xasmgen
void test_pag(WORD *da, WORD mask);					// xasmgen
void test_rel(short *da);								// xasmgen

// module-specific parsing functions
// implemented
void prs6502(void);
void prs6801(void);
void prs6805(void);
void prs6811(void);
void prs8048(void);
void prs8051(void);
void prs8080(void);
// un-implemented
void prs1800(void);
void prs32000(void);
void prs34000(void);
void prs3800(void);
void prs56000(void);
void prs62800(void);
void prs6800(void);
void prs6804(void);
void prs6809(void);
void prs68000(void);
void prs7500(void);
void prs7700(void);
void prs7800(void);
void prs8086(void);
void prs8096(void);
void prsCLIP(void);
void prsCOP400(void);
void prsNCR32(void);
void prsNOVA(void);
void prsPDP8(void);
void prsPDP11(void);
void prsZ8(void);
void prsZ80(void);
void prsZ8000(void);

