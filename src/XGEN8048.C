// XGEN8048.C - 8048 source parser for XASM.C

#include "xasm.h"

// addressing modes
enum mod8048 {
	ADIR,		// (A,Rx)	register -> accum
	AIND,		// (A,@Rx)	indirect register -> accum
	AIMM,		// (A,#xxx)	immediate -> accum
	ACC,		// (A)		accumulator
	REG,		// (Rx)		register
	IREG,		// (@Rx)	indirect register
	PORT,		// (Px,#xxx) I/O port
	APOR,		// (A,Px)
	JMP,		// jump
	NONE		// parsed individually
};

// special handling codes
enum nonecodes {
	N_NONE=1,	// no arg, opcode in JMP
	N_JMP,		// 12-bit jump, opcode in JMP
	N_RJMP,		// register jump, opcode in JMP
	N_MOV,		// move type, opcodes in routine
	N_INT,		// interrupt type, I in ADIR, TCNTI in AIND
	N_TIM,		// timer type, T in ADIR, CNT in AIND
	N_SEL,		// select type, opcodes in routine
	N_CLK,		// clock type, CLK in ADIR
	N_INDA,		// @A - opcode in AIND
	N_INDB,		// A,@A - opcode in AIND
	N_FLAG,		// A/C/F0/F1, A in ACC, C in ADIR, F0 in AIND, F1 in AIMM
};

struct entry8048 {
	char *opname;
	char opcodes[10];
};

static struct entry8048 ops8048[] = {
//				ADIR AIND AIMM ACC  REG  IREG PORT APOR JMP  NONE
{ "ADD",  { 0x68,0x60,0x03,0,   0,   0,   0,   0,   0,   0			}},
{ "ADDC", { 0x78,0x70,0x13,0,   0,   0,   0,   0,   0,   0			}},
{ "ANL",  { 0x58,0x50,0x53,0,   0,   0,   0x98,0,   0,   0			}},
{ "ANLD", { 0,   0,   0,   0,   0,   0,   0x98,0,   0,   0			}},
{ "CALL", { 0,   0,   0,   0,   0,   0,   0,   0,   0x14,N_JMP		}},
{ "CLR",  { 0x97,0x85,0xA5,0x27,0,   0,   0,   0,   0,   N_FLAG	}},
{ "CPL",  { 0xA7,0x95,0xB5,0x37,0,   0,   0,   0,   0,   N_FLAG	}},
{ "DA",   { 0,   0,   0,   0x57,0,   0,   0,   0,   0,   0			}},
{ "DEC",  { 0,   0,   0,   0x07,0xC8,0,   0,   0,   0,   0			}},
{ "DIS",  { 0x15,0x35,0,   0,   0,   0,   0,   0,   0,   N_INT		}},
{ "DJNZ", { 0,   0,   0,   0,   0,   0,   0,   0,   0xE8,N_RJMP	}},
{ "EN",   { 0x05,0x25,0,   0,   0,   0,   0,   0,   0,   N_INT		}},
{ "ENT0", { 0x75,0,   0,   0,   0,   0,   0,   0,   0,   N_CLK		}},
{ "IN",   { 0,   0,   0,   0,   0,   0,   0,   0x08,0,   0			}},
{ "INC",  { 0,   0,   0,   0x17,0x18,0x10,0,   0,   0,   0			}},
{ "INS",  { 0,   0,   0,   0,   0,   0,   0,   0x08,0,   0			}},
{ "JB0",  { 0,   0,   0,   0,   0,   0,   0,   0,   0x12,0			}},
{ "JB1",  { 0,   0,   0,   0,   0,   0,   0,   0,   0x32,0			}},
{ "JB2",  { 0,   0,   0,   0,   0,   0,   0,   0,   0x52,0			}},
{ "JB3",  { 0,   0,   0,   0,   0,   0,   0,   0,   0x72,0			}},
{ "JB4",  { 0,   0,   0,   0,   0,   0,   0,   0,   0x92,0			}},
{ "JB5",  { 0,   0,   0,   0,   0,   0,   0,   0,   0xB2,0			}},
{ "JB6",  { 0,   0,   0,   0,   0,   0,   0,   0,   0xD2,0			}},
{ "JB7",  { 0,   0,   0,   0,   0,   0,   0,   0,   0xF2,0			}},
{ "JC",   { 0,   0,   0,   0,   0,   0,   0,   0,   0xF6,0			}},
{ "JF0",  { 0,   0,   0,   0,   0,   0,   0,   0,   0xB6,0			}},
{ "JF1",  { 0,   0,   0,   0,   0,   0,   0,   0,   0x76,0			}},
{ "JMP",  { 0,   0,   0,   0,   0,   0,   0,   0,   0x04,N_JMP		}},
{ "JMPP", { 0,   0xB3,0,   0,   0,   0,   0,   0,   0,   N_INDA	}},
{ "JNC",  { 0,   0,   0,   0,   0,   0,   0,   0,   0xE6,0			}},
{ "JNI",  { 0,   0,   0,   0,   0,   0,   0,   0,   0x86,0			}},
{ "JNT0", { 0,   0,   0,   0,   0,   0,   0,   0,   0x26,0			}},
{ "JNT1", { 0,   0,   0,   0,   0,   0,   0,   0,   0x46,0			}},
{ "JNZ",  { 0,   0,   0,   0,   0,   0,   0,   0,   0x96,0			}},
{ "JT0",  { 0,   0,   0,   0,   0,   0,   0,   0,   0x36,0			}},
{ "JT1",  { 0,   0,   0,   0,   0,   0,   0,   0,   0x56,0			}},
{ "JTF",  { 0,   0,   0,   0,   0,   0,   0,   0,   0x16,0			}},
{ "JZ",   { 0,   0,   0,   0,   0,   0,   0,   0,   0xC6,0			}},
{ "MOV",  { 0xF8,0xF0,0x23,0,   0,   0,   0x38,0x08,0,   N_MOV		}},
{ "MOVD", { 0,   0,   0,   0,   0,   0,   0x38,0x08,0,   0			}},
{ "MOVP", { 0,   0xA3,0,   0,   0,   0,   0,   0,   0,   N_INDB	}},
{ "MOVP3",{ 0,   0xE3,0,   0,   0,   0,   0,   0,   0,   N_INDB	}},
{ "MOVX", { 0,   0x80,0,   0,   0,   0x90,0,   0,   0,   0			}},
{ "NOP",  { 0,   0,   0,   0,   0,   0,   0,   0,   0x00,N_NONE	}},
{ "ORL",  { 0x48,0x40,0x43,0,   0,   0,   0x88,0,   0,   0			}},
{ "ORLD", { 0,   0,   0,   0,   0,   0,   0x88,0,   0,   0			}},
{ "OUTL", { 0,   0,   0,   0,   0,   0,   0x38,0,   0,   0			}},
{ "RET",  { 0,   0,   0,   0,   0,   0,   0,   0,   0x83,N_NONE	}},
{ "RETR", { 0,   0,   0,   0,   0,   0,   0,   0,   0x93,N_NONE	}},
{ "RL",   { 0,   0,   0,   0xE7,0,   0,   0,   0,   0,   0			}},
{ "RLC",  { 0,   0,   0,   0xF7,0,   0,   0,   0,   0,   0			}},
{ "RR",   { 0,   0,   0,   0x77,0,   0,   0,   0,   0,   0			}},
{ "RRC",  { 0,   0,   0,   0x67,0,   0,   0,   0,   0,   0			}},
{ "SEL",  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   N_SEL		}},
{ "STOP", { 0x65,0x65,0,   0,   0,   0,   0,   0,   0,   N_TIM		}},
{ "STRT", { 0x55,0x45,0,   0,   0,   0,   0,   0,   0,   N_TIM		}},
{ "SWAP", { 0,   0,   0,   0x47,0,   0,   0,   0,   0,   0			}},
{ "XCH",  { 0x28,0x20,0,   0,   0,   0,   0,   0,   0,   0			}},
{ "XCHD", { 0,   0x30,0,   0,   0,   0,   0,   0,   0,   0			}},
{ "XRL",  { 0xD8,0xD0,0xD3,0,   0,   0,   0,   0,   0,   0			}}
};
static struct entry8048 *info8048;

// test for valid opcode
static short okop8048(short mode)
{
	register short op;

	if ((op = info8048->opcodes[mode]) == 0)
		if (!streq(info8048->opname,"NOP"))
			moderr();
	return(op & 0xFF);
}

// binary search for opcode mnemonic
static struct entry8048 *find8048(char *searchfor)
{
	struct entry8048 *ent;
	register short lo, hi, mid, nfind;

	lo = 0;
	hi = dim(ops8048)-1;
	do {
		ent = ops8048 + (mid = (lo + hi) >> 1);
		nfind = strcmp(ent->opname, searchfor);
		if (nfind < 0)
			lo = mid + 1;
		else if (nfind > 0)
			hi = mid - 1;	// must be +1
		else
			break;		// got a match
	} while (lo <= hi);
	if (nfind != 0)	// not found
	{
		if ((mid = findmac(searchfor)) == -1)
			return(NULL);
		prblank();
		MACACT = TRUE;
		getmac(mid);
		return NULL;
	}
	else
		return ent;
}

static void genrjmp(short op)
{
	WORD dat;
	short reg = arg[1] - '0';

	test_oor(&reg, 0, 7);
	getword(arg+3, &dat);
	gen2b(op + reg,(WORD)dat);
}

static void genjxx(short op)
{
	WORD dat;

	getword(arg, &dat);
	gen2b(op,(WORD)dat);
}

static void genjmp(short op)
{
	WORD dat, hib;
	
	getword(arg, &dat);
	hib = (dat >> 3) & 0xE0;
	gen2b(op + hib, dat & 0xFF);
}

static void genr07(short op)
{
	short reg = arg[1] - '0';

	test_oor(&reg, 0, 7);
	if (arg[2] == ',' && arg[3] == '#')
	{
		WORD dat;

		getword(arg+4, &dat);
		gen2b(op + reg, (WORD)dat);
	}
	else
		gen1b(op + reg);
}

static void genr01(short op)
{
	short reg = arg[1] - '0';

	test_oor(&reg, 0, 1);
	if (arg[2] == ',' && arg[3] == '#')
	{
		WORD dat;

		getword(arg+4, &dat);
		gen2b(op + reg, (WORD)dat);
	}
	else
		gen1b(op + reg);
}

static void genp(short op)
{
	short port;

	if (arg[0] == 'B')	// "BUS"
	{
		port = 0;
		if (op == 0x38)	// handle OUTL bogusness
			op = 0x02;
	}
	else if (arg[0] == 'P')
		port = arg[1] - '0';
	if (arg[2] == ',')
	{
		if (arg[3] == '#')
		{
			arg += 3;	// point to #
			gendb(op + port);
		}
		else if (arg[3] == 'A')
			gen1b(op + port);
		else
		{
			argerr();
			gen1b(op + port);
		}
	}
	else
		gen1b(op + port);
}

// call proper code generator
void prs8048(void)
{
	enum mod8048 mod = NONE;

	if (!*op)
	{
		prblank();
		return;
	}
	if ((info8048 = find8048(op)) == NULL)
	{
		if (MACACT)
			MACACT = FALSE;
		else
			opcerr();
		return;
	}
	if (arg[0] == 'A')
	{
		if (arg[1] == ',')
		{
			if (arg[2] == 'R')
			{
				mod = ADIR;
				arg += 2;	// point arg to R
			}
			else if (arg[2] == 'P' && arg[3] >= '1' && arg[3] <= '7')
			{
				mod = APOR;
				arg += 2;	// point arg to P
			}
			else if (arg[2] == 'B' && arg[3] == 'U' && arg[4] == 'S')
			{
				mod = APOR;
				arg += 2;
			}
			else if (arg[2] == '@' && arg[3] == 'R')
			{
				mod = AIND;
				arg += 3;	// point arg to R
			}
			else if (arg[2] == '#')
			{
				mod = AIMM;
				arg += 2;	// point arg to #
			}
		}
		else if (arg[1] == 0)
			mod = ACC;
	}
	else if (arg[0] == 'R')
	{
		if (info8048->opcodes[NONE])
			mod = NONE;
		else
			mod = REG;
	}
	else if (arg[0] == '@' && arg[1] == 'R')
	{
		if (info8048->opcodes[NONE])
			mod = NONE;
		else
		{
			mod = IREG;
			arg++;
		}
	}
	else if (arg[0] == 'B' && arg[1] == 'U' && arg[2] == 'S' && arg[3] == ',')
		mod = PORT;
	else if (arg[0] == 'P' && arg[1] >= '1' && arg[1] <= '7' && arg[2] == ',')
		mod = PORT;
	else
	{
		if (info8048->opcodes[NONE])
			mod = NONE;
		else
			mod = JMP;
	}
	switch (mod)
	{
		case ADIR:
		case REG:
			genr07(okop8048(mod));
			break;
		case AIND:
		case IREG:
			genr01(okop8048(mod));
			break;
		case AIMM:
			gendb(okop8048(AIMM));
			break;
		case ACC:
			gen1b(okop8048(ACC));
			break;
		case APOR:
		case PORT:
			genp(okop8048(mod));
			break;
		case JMP:
			genjxx(okop8048(JMP));
			break;
		case NONE:
			switch (info8048->opcodes[NONE])
			{
				case N_NONE:
					gen1b(okop8048(JMP));
					break;
				case N_JMP:
					genjmp(okop8048(JMP));
					break;
				case N_RJMP:
					genrjmp(okop8048(JMP));
					break;
				case N_MOV:
					if (arg[0] == '@')
					{
						arg++;
						if (arg[3] == '#')
							genr01(0xB0);
						else
							genr01(0xA0);
					}
					else if (arg[0] == 'R')
					{
						if (arg[3] == '#')
							genr07(0xB8);
						else
							genr07(0xA8);
					}
					else if (streq(arg, "A,T"))
						gen1b(0x42);
					else if (streq(arg, "T,A"))
						gen1b(0x62);
					else if (streq(arg, "A,PSW"))
						gen1b(0xC7);
					else if (streq(arg, "PSW,A"))
						gen1b(0xD7);
					else
						argerr();
					break;
				case N_INT:
					if (streq(arg, "I"))
						gen1b(okop8048(ADIR));
					else if (streq(arg, "TCNTI"))
						gen1b(okop8048(AIND));
					else
						argerr();
					break;
				case N_TIM:
					if (streq(arg, "T"))
						gen1b(okop8048(ADIR));
					else if (streq(arg, "CNT"))
						gen1b(okop8048(AIND));
					else if (streq(arg, "TCNT"))
						gen1b(okop8048(AIND));
					else
						argerr();
					break;
				case N_SEL:
					if (streq(arg, "MB0"))
						gen1b(0xE5);
					else if (streq(arg, "MB1"))
						gen1b(0xF5);
					else if (streq(arg, "RB0"))
						gen1b(0xC5);
					else if (streq(arg, "RB1"))
						gen1b(0xD5);
					else
						argerr();
					break;
				case N_CLK:
					if (streq(arg, "CLK"))
						gen1b(okop8048(ADIR));
					else
						argerr();
					break;
				case N_INDA:
					if (streq(arg, "@A"))
						gen1b(okop8048(AIND));
					else
						argerr();
					break;
				case N_INDB:
					if (streq(arg, "A,@A"))
						gen1b(okop8048(AIND));
					else
						argerr();
					break;
				case N_FLAG:
					if (streq(arg, "A"))
						gen1b(okop8048(ACC));
					else if (streq(arg, "C"))
						gen1b(okop8048(ADIR));
					else if (streq(arg, "F0"))
						gen1b(okop8048(AIND));
					else if (streq(arg, "F1"))
						gen1b(okop8048(AIMM));
					else
						argerr();
					break;
			}
			break;
	}
}

