// xasmdir.c - directive parsing for xasm.c

#include "xasm.h"
#include <time.h>

// end of source marker
static void dir_end(void)
{
	if (!IFFON)
		return;
	ENDFLAG = TRUE;
	prblank();
}

// define byte reversal
static void dir_brev(void)
{
	REVFLAG = TRUE;
	prblank();
}

// define empty ROM fill char
static void dir_fill(void)
{
	fillc = atoh(arg);
	prblank();
}

// convert short to bcd
static short itobcd(short n)
{
	char buf[6];

	sprintf(buf,"%5d",n);
	return(atoh(buf));
}

// output current date string to object file
static void dir_date(void)
{
	struct tm *t;
	short mn,dy,yr;
	long secs;

	if (!IFFON)
		return;
	secs = time(NULL);
	t = localtime(&secs);
	mn = itobcd(t->tm_mon + 1);
	dy = itobcd(t->tm_mday);
	yr = itobcd(t->tm_year);
	prdata(mn,dy,yr);
	pc += 3L;
}

// output current time string to object file
static void dir_time(void)
{
	struct tm *t;
	short hr,mn,sc;
	long secs;

	if (!IFFON)
		return;
	secs = time(NULL);
	t = localtime(&secs);
	hr = itobcd(t->tm_hour);
	mn = itobcd(t->tm_min);
	sc = itobcd(t->tm_sec);
	prdata(hr,mn,sc);
	pc += 3L;
}

// define cpu
static void dir_cpu(void)
{
	if (!IFFON)
		return;
	strcpy(cpu,arg);
	prblank();
	cpupars();
}

// define object code format
static void dir_obj(void)
{
	if (!IFFON)
		return;
	if (streq(arg,"OFF"))
		OBJFLAG=FALSE;
	else
		OBJFLAG=TRUE;
	prblank();
}

// define listing output
static void dir_list(void)
{
	if (!IFFON)
		return;
	LSTON++;
	if (streq(arg,"OFF"))
		LSTON -= 2;
	else if (streq(arg,"ON"))
		LSTON = TRUE;
	else if (streq(arg,"SCREEN"))
		lstfile = stdout;
	else if (streq(arg,"PRINT"))
		lstfile = stdprn;
	else
		lstopen(arg);
	prblank();
}

// define error text output
static void dir_error(void)
{
	if (!IFFON)
		return;
	if (apass > 1)
	{
		ERRON = TRUE;
		if (streq(arg,"OFF"))
			ERRON = FALSE;
		else if (streq(arg,"ON"))
			ERRON = TRUE;
		else if (streq(arg,"SCREEN"))
			errfile = stderr;
		else if (!ERRF)
			erropen(arg);
	}
	prblank();
}

// define symbol table output
static void dir_symbol(void)
{
	if (!IFFON)
		return;
	if (apass > 1)
	{
		SYMON = TRUE;
		if (streq(arg,"OFF"))
			SYMON = FALSE;
		else if (streq(arg,"ON"))
			SYMON = TRUE;
		else if (streq(arg,"SCREEN"))
			symfile = stdout;
		else if (!SYMF)
			symopen(arg);
	}
	prblank();
}

// include a source file (JSR)
static void dir_include(void)
{
	if (!IFFON)
		return;
	inclpush(arg);
	prblank();
}

// append a source file (JMP)
static void dir_append(void)
{
	if (!IFFON)
		return;
	appopen(arg);
	prblank();
}

// start macro
static void dir_macro(void)
{
	if (!IFFON)
		return;
	genmac(arg);
	MACON = TRUE;
	prblank();
}

// end macro
static void dir_endm(void)
{
	if (!IFFON)
		return;
	MACON = FALSE;
	prblank();
}

// if for conditional assembly
static void dir_if(void)
{
	WORD deq;

	getword(arg,&deq);
	IFFON = (deq != 0L);
	prequ(deq);
}

// if not for conditional assembly
static void dir_ifn(void)
{
	WORD deq;

	getword(arg,&deq);
	IFFON = (deq == 0L);
	prequ(deq);
}

// else for conditional assembly
static void dir_else(void)
{
	IFFON = !IFFON;
	prblank();
}

// else if for conditional assembly
static void dir_elif(void)
{
	WORD deq;

	IFFON = !IFFON;
	if (!IFFON)
	{
		getword(arg,&deq);
		IFFON = (deq != 0L);
		prequ(deq);
	}
	else
		prblank();
}

// endif for conditional assembly
static void dir_endif(void)
{
	IFFON = TRUE;
	prblank();
}

// symbol equate
static void dir_equ(void)
{
	WORD deq;

	if (!IFFON)
		return;
	if (streq(arg,"TRUE"))
		deq = TRUE;
	else if (streq(arg,"FALSE"))
		deq = FALSE;
	else
		getword(arg,&deq);
	genlbl((ADDRESS)deq,lbl,TRUE);
	prequ(deq);
}

// set object code address
static void dir_org(void)
{
	ADDRESS oldpc, i;

	if (!IFFON)
		return;
	if (FIRSTORG)
	{
		FIRSTORG = FALSE;
		pc = makenum(arg);
		ipc = pc;
	}
	else
	{
		oldpc = pc;
		pc = makenum(arg);
		if (pc < oldpc)
			orgerr();
		else if (apass == 2)
		{
			for (i=oldpc; i<pc; i++)
			{
				outbyte(fillc);
				freespc++;
			}
		}
	}
	prblank();
}

// set object code address backwards
static void dir_rorg(void)
{
	if (!IFFON)
		return;
	pc = makenum(arg);
	prblank();
}

// set hex output code address
static void dir_horg(void)
{
	if (!IFFON)
		return;
	oline = makenum(arg);
	prblank();
}

// output ROM page number to hex output file
static void dir_hpag(void)
{
	if (!IFFON)
		return;
	outhpag(makenum(arg));
	prblank();
}

// define public for later link
// not yet implemented
static void dir_xdef(void)
{
	prblank();
}

// reference public for later link
// not yet implemented
static void dir_xref(void)
{
	prblank();
}

// output data byte(s)
static void dir_db(void)
{
	WORD d1, d2, d3;
	char *tmp;

	if (!IFFON)
		return;
	genlbl(pc,lbl,FALSE);
	scanptr = arg;
	tmp = scancom();
	getword(tmp,&d1);
	tmp = scancom();
	if (!*tmp)
	{
		prdata(d1,UNUSED,UNUSED);
		pc += 1L;
		return;
	}
	getword(tmp,&d2);
	tmp = scancom();
	if (!*tmp)
	{
		prdata(d1,d2,UNUSED);
		pc += 2L;
		return;
	}
	getword(tmp,&d3);
	tmp = scancom();
	prdata(d1,d2,d3);
	pc += 3L;
	while (*tmp)
	{
		getword(tmp,&d1);
		pc++;
		if (apass == 2)
			outbyte(d1);
		tmp = scancom();
	}
}

// output data word(s)
static void dir_dw(void)
{
	short cnt;
	WORD d1, d2;
	char *tmp;

	if (!IFFON)
		return;
	genlbl(pc,lbl,FALSE);
	scanptr = arg;
	tmp = scancom();
	getword(tmp,&d1);
	tmp = scancom();
	if (!*tmp)
	{
		if (REVFLAG)
			prdata(d1&0xFF,(d1/256)&0xFF,UNUSED);
		else
			prdata((d1/256)&0xFF,d1&0xFF,UNUSED);
		pc += 2L;
		return;
	}
	getword(tmp,&d2);
	cnt = 4;
	if (REVFLAG)
		prdata(d1&0xFF,(d1/256)&0xFF,d2&0xFF);
	else
		prdata((d1/256)&0xFF,d1&0xFF,(d2/256)&0xFF);
	if (apass == 2)
	{
		if (REVFLAG)
			outbyte(d2/256);
		else
			outbyte(d2);
	}
	tmp = scancom();
	while (*tmp)
	{
		getword(tmp,&d1);
		cnt += 2;
		if (apass == 2)
		{
			if (REVFLAG)
			{
				outbyte(d1);
				outbyte(d1/256);
			}
			else
			{
				outbyte(d1/256);
				outbyte(d1);
			}
		}
		tmp = scancom();
	}
	pc += (ADDRESS) cnt;
}

// output data space
static void dir_ds(void)
{
	register short i;
	WORD cnt;

	if (!IFFON)
		return;
	genlbl(pc,lbl,FALSE);
	getword(arg,&cnt);
	if (cnt == 1L)
		prdata(0,UNUSED,UNUSED);
	else if (cnt == 2L)
		prdata(0,0,UNUSED);
	else if (cnt == 3L)
		prdata(0,0,0);
	else
	{
		prdata(0,0,0);
		if (apass == 2)
			for (i=3; i<cnt; i++)
				outbyte(0);
	}
	pc += cnt;
}

// output ascii data general routine
static void proc_as(boolean zero)
{
	short i,cnt,ter;
	register char *tmp,c1,c2,c3;

	if (!IFFON)
		return;
	genlbl(pc,lbl,FALSE);
	tmp = arg++;
	ter = *tmp++;
	for (cnt=1; *tmp++!=ter; cnt++)
		;
	cnt--;
	tmp = arg;
	c1 = (*tmp ? *tmp : ' ');
	c2 = (*(tmp+1) ? *(tmp+1) : ' ');
	c3 = (*(tmp+2) ? *(tmp+2) : ' ');
	if (cnt==1)
	{
		if (zero)
			prdata(c1,0,UNUSED);
		else
			prdata(c1,UNUSED,UNUSED);
	}
	else if (cnt==2)
	{
		if (zero)
			prdata(c1,c2,0);
		else
			prdata(c1,c2,UNUSED);
	}
	else if (cnt==3)
	{
		prdata(c1,c2,c3);
		if (zero)
			outbyte(0);
	}
	else
	{
		prdata(c1,c2,c3);
		tmp += 3;
		if (apass == 2)
		{
			for (i=3; i<cnt; i++)
			{
				c1 = (*tmp ? *tmp : ' ');
				outbyte(c1);
				tmp++;
			}
			if (zero)
				outbyte(0);
		}
	}
	pc += (ADDRESS) cnt;
	if (zero)
		pc++;
}

// output ascii data, non-terminated
static void dir_as(void)
{
	proc_as(FALSE);
}

// output ascii data, zero terminated
static void dir_asz(void)
{
	proc_as(TRUE);
}

// NOTE: linear search; most common to front
vecdef directs[] = {
// most common
	{ ".EQU",	dir_equ,		},
	{ ".DB",		dir_db,		},
	{ ".DW",		dir_dw,		},
	{ ".AS",		dir_as,		},
	{ ".ASZ",	dir_asz,		},
	{ ".DS",		dir_ds,		},
// used, but not as often
	{ ".IF",		dir_if,		},
	{ ".ELSE",	dir_else,	},
	{ ".ENDIF",	dir_endif,	},
	{ ".IFN",	dir_ifn,		},
	{ ".ELIF",	dir_elif,	},
	{ ".XDEF",	dir_xdef,	},
	{ ".XREF",	dir_xref,	},
	{ ".MACRO",	dir_macro,	},
	{ ".ENDM",	dir_endm,	},
	{ ".INCLUDE",dir_include,},
	{ ".APPEND",dir_append,	},
	{ ".LIST",	dir_list,	},
// once if ever
	{ ".AORG",	dir_horg,	}, // backward compatibility
	{ ".HORG",	dir_horg,	},
	{ ".ORG",	dir_org,		},
	{ ".RORG",	dir_rorg,	},
	{ ".HPAG",	dir_hpag,	},
	{ ".CPU",	dir_cpu,		},
	{ ".SYMBOL",dir_symbol,	},
	{ ".END",	dir_end,		},
// rarely used at all
	{ ".DATE",	dir_date,	},
	{ ".SET",	dir_equ,		},
	{ ".TIME",	dir_time,	},
	{ ".BREV",	dir_brev,	},
	{ ".ERROR",	dir_error,	},
	{ ".FILLC",	dir_fill,	},
	{ ".OBJ",	dir_obj,		}
};

// process directives
void procdirect(void)
{
	register short i;

	for (i=0; i<dim(directs); i++)
	{
		if (streq(op,directs[i].name))
		{
			(directs[i].func)();
			return;
		}
	}
	direrr();
}

