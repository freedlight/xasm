// XGEN8051.C - 8051 source parser for XASM.C

#include "xasm.h"

// addressing modes
enum mod8051 {
	AREG,		// (A,Rx)	register -> accum
	AIND,		// (A,@Rx)	indirect register -> accum
	AIMM,		// (A,#xxx)	immediate -> accum
	ADIR,		// (A,loc)	addr -> accum
	ACC,		// (A)		accumulator
	MDIR,		// (loc,A)		accum -> memory
	MIMM,		// (loc,#xxx)	immediatae -> memory
	REG,		// (Rx)		register
	IREG,		// (@Rx)	indirect register
	CARR,		// (C,xxx)	carry flag
	CINV,		// (C,/xxx)	carry flag
	JMP,		// jump
	NONE		// parsed individually
};

// special handling codes
enum nonecodes {
	N_NONE=1,	// no arg, opcode in JMP
	N_JMP,		// 12-bit jump, opcode in JMP
	N_BJMP,		// 16-bit jump, opcode in JMP
	N_DJNZ,		// special handling for DJNZ
	N_CJNE,		// special handling for CJNE
	N_MDIR,		// handle direct memory, opcode in JMP or IREG
	N_MATH,		// 16-bit math (op AB) opcode in ACC
	N_MOV,		// move type, opcodes in routine
	N_INDA,		// @A+DPTR - opcode in AIND
	N_INDB,		// A,@A+PC (opcode in AREG) A,@A+DPTR (opcode in AIND)
	N_FLAG,		// A/C/bitf, A in ACC, C in AREG, bitf in AIND
	N_BITF,		// rel jump w/bitflag, opcode in JMP
};

struct entry8051 {
	char *opname;
	char opcodes[13];
};

static struct entry8051 ops8051[] = {
//				AREG AIND AIMM ADIR ACC  MDIR MIMM REG  IREG CARR CINV JMP  NONE
{ "ACALL",{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0x11,N_JMP	}},
{ "ADD",  { 0x28,0x26,0x24,0x25,0,   0,   0,   0,   0,   0,   0,   0,   0			}},
{ "ADDC", { 0x38,0x36,0x34,0x35,0,   0,   0,   0,   0,   0,   0,   0,   0			}},
{ "AJMP", { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0x01,N_JMP	}},
{ "ANL",  { 0x58,0x56,0x54,0x55,0,   0x52,0x53,0,   0,   0x82,0xB0,0,   0			}},
{ "CALL", { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0x12,N_BJMP	}},
{ "CJNE", { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0xB4,N_CJNE	}},
{ "CLR",  { 0xC3,0xC2,0,   0,   0xE4,0,   0,   0,   0,   0,   0,   0,   N_FLAG	}},
{ "CPL",  { 0xB3,0xB2,0,   0,   0xF4,0,   0,   0,   0,   0,   0,   0,   N_FLAG	}},
{ "DA",   { 0,   0,   0,   0,   0xD4,0,   0,   0,   0,   0,   0,   0,   0			}},
{ "DEC",  { 0,   0,   0,   0,   0x14,0,   0,   0x18,0x16,0,   0,   0x15,N_MDIR	}},
{ "DIV",  { 0,   0,   0,   0,   0x84,0,   0,   0,   0,   0,   0,   0,   N_MATH	}},
{ "DJNZ", { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0xD8,N_DJNZ	}},
{ "INC",  { 0xA3,0,   0,   0,   0x04,0,   0,   0x08,0x06,0,   0,   0x05,N_MDIR	}},
{ "JB",   { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0x20,N_BITF	}},
{ "JBC",  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0x10,N_BITF	}},
{ "JC",   { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0x40,0			}},
{ "JMP",  { 0,   0x73,0,   0,   0,   0,   0,   0,   0,   0,   0,   0x02,N_INDA	}},
{ "JNB",  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0x30,N_BITF	}},
{ "JNC",  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0x50,0			}},
{ "JNZ",  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0x70,0			}},
{ "JZ",   { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0x60,0			}},
{ "LCALL",{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0x12,N_BJMP	}},
{ "LJMP", { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0x02,N_BJMP	}},
{ "MOV",  { 0xE8,0xE6,0x74,0xE5,0,   0,   0,   0,   0,   0xA2,0,   0,   N_MOV	}},
{ "MOVC", { 0x83,0x93,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   N_INDB	}},
{ "MOVX", { 0,   0xE2,0,   0,   0,   0,   0,   0,   0xF2,0,   0,   0,   0			}},
{ "MUL",  { 0,   0,   0,   0,   0xA4,0,   0,   0,   0,   0,   0,   0,   N_MATH	}},
{ "NOP",  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0x00,N_NONE	}},
{ "ORL",  { 0x48,0x46,0x44,0x45,0,   0x42,0x43,0,   0,   0x72,0xA0,0,   0			}},
{ "POP",  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0xD0,N_MDIR	}},
{ "PUSH", { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0xC0,N_MDIR	}},
{ "RET",  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0x22,N_NONE	}},
{ "RETI", { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0x32,N_NONE	}},
{ "RL",   { 0,   0,   0,   0,   0x23,0,   0,   0,   0,   0,   0,   0,   0			}},
{ "RLC",  { 0,   0,   0,   0,   0x33,0,   0,   0,   0,   0,   0,   0,   0			}},
{ "RR",   { 0,   0,   0,   0,   0x03,0,   0,   0,   0,   0,   0,   0,   0			}},
{ "RRC",  { 0,   0,   0,   0,   0x13,0,   0,   0,   0,   0,   0,   0,   0			}},
{ "RTS",  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0x22,N_NONE	}},
{ "SETB", { 0xD3,0xD2,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   N_FLAG	}},
{ "SJMP", { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0x80,0			}},
{ "SUBB", { 0x98,0x96,0x94,0x95,0,   0,   0,   0,   0,   0,   0,   0,   0			}},
{ "SWAP", { 0,   0,   0,   0,   0xC4,0,   0,   0,   0,   0,   0,   0,   0			}},
{ "XCH",  { 0xC8,0xC6,0,   0xC5,0,   0,   0,   0,   0,   0,   0,   0,   0			}},
{ "XCHD", { 0,   0xD6,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0			}},
{ "XRL",  { 0x68,0x66,0x64,0x65,0,   0x62,0x63,0,   0,   0,   0,   0,   0			}}
};
static struct entry8051 *info8051;

// test for valid opcode
static short okop8051(short mode)
{
	register short op;

	if ((op = info8051->opcodes[mode]) == 0)
		if (!streq(info8051->opname,"NOP"))
			moderr();
	return(op & 0xFF);
}

// binary search for opcode mnemonic
static struct entry8051 *find8051(char *searchfor)
{
	struct entry8051 *ent;
	register short lo, hi, mid, nfind;

	lo = 0;
	hi = dim(ops8051)-1;
	do {
		ent = ops8051 + (mid = (lo + hi) >> 1);
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

static char *splitarg(char *arg)
{
	char *p;
	if ((p = strchr(arg, ',')) == NULL)
	{
		miserr();
		return "";
	}
	*p++ = 0;
	return p;
}

static short isdptr(char *s)
{
	if (s[0]=='D' && s[1]=='P' && s[2]=='T' && s[3]=='R')
		return 1;
	return 0;
}

static short isreg(char *s)
{
	if (s[0]=='R' && s[1]>='0' && s[1]<='7')
		return 1;
	return 0;
}

static WORD specialloc(char *s, short bitf)
{
	if (streq(s, "ACC"))
		return 0xE0;
	else if (streq(s, "B"))
		return 0xF0;
	else if (streq(s, "PSW"))
		return 0xD0;
	else if (streq(s, "P0"))
		return 0x80;
	else if (streq(s, "P1"))
		return 0x90;
	else if (streq(s, "P2"))
		return 0xA0;
	else if (streq(s, "P3"))
		return 0xB0;
	else if (streq(s, "IP"))
		return 0xB8;
	else if (streq(s, "IE"))
		return 0xA8;
	else if (streq(s, "TCON"))
		return 0x88;
	else if (streq(s, "T2CON"))
		return 0xC8;
	else if (streq(s, "SCON"))
		return 0x98;
	if (bitf)
		return 0;
	if (streq(s, "SBUF"))
		return 0x99;
	else if (streq(s, "TMOD"))
		return 0x89;
	else if (streq(s, "SP"))
		return 0x81;
	else if (streq(s, "TL0"))
		return 0x8A;
	else if (streq(s, "TL1"))
		return 0x8B;
	else if (streq(s, "TH0"))
		return 0x8C;
	else if (streq(s, "TH1"))
		return 0x8D;
	else if (streq(s, "DPL"))
		return 0x82;
	else if (streq(s, "DPH"))
		return 0x83;
	else if (streq(s, "PCON"))
		return 0x87;
	else if (streq(s, "TL2"))
		return 0xCC;
	else if (streq(s, "TH2"))
		return 0xCD;
	else if (streq(s, "RCAP2L"))
		return 0xCA;
	else if (streq(s, "RCAP2H"))
		return 0xCB;
	return 0;
}

// parse special bit names
static WORD specialbit(char *s)
{
// SCON flags
	if (streq(s, "RI"))
		return 0x98 + 0;
	else if (streq(s, "TI"))
		return 0x98 + 1;
	else if (streq(s, "RB8"))
		return 0x98 + 2;
	else if (streq(s, "TB8"))
		return 0x98 + 3;
	else if (streq(s, "REN"))
		return 0x98 + 4;
	else if (streq(s, "SM2"))
		return 0x98 + 5;
	else if (streq(s, "SM1"))
		return 0x98 + 6;
	else if (streq(s, "SM0"))
		return 0x98 + 7;
// TCON flags
	else if (streq(s, "IT0"))
		return 0x88 + 0;
	else if (streq(s, "IE0"))
		return 0x88 + 1;
	else if (streq(s, "IT1"))
		return 0x88 + 2;
	else if (streq(s, "IE1"))
		return 0x88 + 3;
	else if (streq(s, "TR0"))
		return 0x88 + 4;
	else if (streq(s, "TF0"))
		return 0x88 + 5;
	else if (streq(s, "TR1"))
		return 0x88 + 6;
	else if (streq(s, "TF1"))
		return 0x88 + 7;
// IE flags
	else if (streq(s, "EX0"))
		return 0xA8 + 0;
	else if (streq(s, "ET0"))
		return 0xA8 + 1;
	else if (streq(s, "EX1"))
		return 0xA8 + 2;
	else if (streq(s, "ET1"))
		return 0xA8 + 3;
	else if (streq(s, "ES"))
		return 0xA8 + 4;
	else if (streq(s, "ET2"))
		return 0xA8 + 5;
	else if (streq(s, "EA"))
		return 0xA8 + 7;
// PSW flags
	else if (streq(s, "P"))
		return 0xD0 + 0;
	else if (streq(s, "OV"))
		return 0xD0 + 2;
	else if (streq(s, "RS0"))
		return 0xD0 + 3;
	else if (streq(s, "RS1"))
		return 0xD0 + 4;
	else if (streq(s, "F0"))
		return 0xD0 + 5;
	else if (streq(s, "AC"))
		return 0xD0 + 6;
	else if (streq(s, "CY"))
		return 0xD0 + 7;
// port 3 flags 		
	else if (streq(s, "RXD"))
		return 0xB0 + 0;
	else if (streq(s, "TXD"))
		return 0xB0 + 1;
	else if (streq(s, "INT0"))
		return 0xB0 + 2;
	else if (streq(s, "INT1"))
		return 0xB0 + 3;
	else if (streq(s, "T0"))
		return 0xB0 + 4;
	else if (streq(s, "T1"))
		return 0xB0 + 5;
	else if (streq(s, "WR"))
		return 0xB0 + 6;
	else if (streq(s, "RD"))
		return 0xB0 + 7;
// port 1 flags 		
	else if (streq(s, "T2"))
		return 0x90 + 0;
	else if (streq(s, "T2EX"))
		return 0x90 + 1;
// T2CON flags
	else if (streq(s, "CPRL2"))
		return 0xC8 + 0;
	else if (streq(s, "CT2"))
		return 0xC8 + 1;
	else if (streq(s, "TR2"))
		return 0xC8 + 2;
	else if (streq(s, "EXEN2"))
		return 0xC8 + 3;
	else if (streq(s, "TCLK"))
		return 0xC8 + 4;
	else if (streq(s, "RCLK"))
		return 0xC8 + 5;
	else if (streq(s, "EXF2"))
		return 0xC8 + 6;
	else if (streq(s, "TF2"))
		return 0xC8 + 7;
// IP flags
	else if (streq(s, "PX0"))
		return 0xB8 + 0;
	else if (streq(s, "PT0"))
		return 0xB8 + 1;
	else if (streq(s, "PX1"))
		return 0xB8 + 2;
	else if (streq(s, "PT1"))
		return 0xB8 + 3;
	else if (streq(s, "PS"))
		return 0xB8 + 4;
	else if (streq(s, "PT2"))
		return 0xB8 + 5;
	return 0;
}

static void getnword(char *s, WORD *dat)
{
	getword(s, dat);
	if (*dat & 0x8000)	// handle negative numbers
		*dat &= 0xFF;
}

static void getsword(char *s, WORD *dat)
{
	WORD spec = specialloc(s, 0);

	if (spec)
		*dat = spec;
	else
		getnword(s, dat);
}

// 2-byte lookup
static void gen2xx(short op)
{
	WORD dat;

	getsword(arg, &dat);
	gen2b(op, dat);
}

// 3-byte lookup
static void gen3xx(short op, char *p1, char *p2)
{
	WORD adr1, adr2;

	getsword(p1, &adr1);
	getnword(p2, &adr2);
	gen3sb(op, adr1, adr2);
}

// 3-byte lookup
static void gen3sx(short op, char *p1, char *p2)
{
	WORD adr1, adr2;

	getsword(p1, &adr1);
	getsword(p2, &adr2);
	gen3sb(op, adr1, adr2);
}

// 3-byte rel lookup
static void gen3rb(short op, char *p1, char *p2)
{
	WORD adr1;
	ADDRESS adr2;

	getsword(p1, &adr1);
	getlbl(p2, &adr2);
	adr2 = make_rel(adr2, 3);
	gen3sb(op, adr1, (WORD)adr2);
}

static WORD getbits(char *bs)
{
	char *p = strchr(bs, '.');
	WORD spec;
	short bit;

	if (p == NULL)
	{
		spec = specialbit(bs);
		if (spec)
			return spec;
		else
		{
			argerr();
			return 0;
		}
	}
	bit = p[1] - '0';
	test_oor(&bit, 0, 7);
	*p = 0;
	spec = specialloc(bs, 1);
	if (spec)
		bit += spec;
	else
	{
		WORD dat;

		getword(bs, &dat);
		test_oor((short *)&dat, 0x20, 0x2F);
		bit += ((dat & 0x0F) << 3);
	}
	return bit;
}

static void genbitf(short op)
{
	WORD dat;
	ADDRESS adr;
	char *dp;

	dp = splitarg(arg);
	dat = getbits(arg);
	getlbl(dp, &adr);
	adr = make_rel(adr, 3);
	gen3sb(op, dat, (WORD)adr);
}

static void gencarr(short op)
{
	WORD dat;

	dat = getbits(arg);
	gen2b(op, dat);
}

static void genCJNE(short op)
{
	short reg = 0;
	char *dp;

	if (arg[1] == ',')	// A,xxx,xx
	{
		if (arg[2] == '#')
			reg = 0, arg += 3;
		else
			reg = 1, arg += 2;
	}
	else if (arg[2] == ',')	// Rn,xxx,xx
	{
		if (arg[0] != 'R')
			argerr();
		else
		{
			reg = arg[1] - '0';
			test_oor(&reg, 0, 7);
		}
		reg += 4;
		if (arg[3] != '#')
			argerr();
		arg += 4;
	}
	else if (arg[3] == ',')	// @Rn,xxx,xx
	{
		if (arg[0] != '@' || arg[1] != 'R')
			argerr();
		else
		{
			reg = arg[2] - '0';
			test_oor(&reg, 0, 1);
		}
		reg += 2;
		if (arg[4] != '#')
			argerr();
		arg += 5;
	}
	dp = splitarg(arg);
	gen3rb(op + reg, arg, dp);
}

static void genDJNZ(short op)
{
	if (arg[0] == 'R' && arg[1] >= '0' && arg[1] <= '7' && arg[2]== ',')
	{
		short reg = arg[1] - '0';
		ADDRESS adr;

		arg += 3;
		getlbl(arg, &adr);
		genrb(op + reg, adr);
	}
	else
	{
		char *dp;

		dp = splitarg(arg);
		gen3rb(op - 3, arg, dp);
	}
}

// build 12-bit jump (flag=0) or 16-bit jump (flag=1)
static void genjmp(short op, short flag)
{
	WORD dat;
	
	getword(arg, &dat);
	if (flag)
		gen3b(op, dat);
	else
	{
		WORD hib = (dat >> 3) & 0xE0;

		test_pag(&dat, 0x7FF);
		gen2b(op + hib, dat & 0xFF);
	}
}

static void genr07(short op)
{
	short reg = arg[1] - '0';

	test_oor(&reg, 0, 7);
	if (arg[2] == ',' && arg[3] == '#')
	{
		arg += 4;
		gen2xx(op + reg);
	}
	else if (arg[2] == 0 || (arg[2] == ',' && arg[3] == 'A' && arg[4] == 0))
		gen1b(op + reg);
	else
	{
		arg += 3;
		gen2xx(op + reg);
	}
}

static void genr01(short op)
{
	short reg = arg[1] - '0';

	if (isdptr(arg))
	{
		if (op == 0xE2 || op == 0xF2)	// "MOVX"
			arg += 2, reg = -2;
		else
			argerr(), reg = 0;
	}
	test_oor(&reg, -2, 1);
	if (arg[2] == ',' && arg[3] == '#')
		arg += 4, gen2xx(op + reg);
	else if (arg[2] == 0 || (arg[2] == ',' && arg[3] == 'A' && arg[4] == 0))
		gen1b(op + reg);
	else
		arg += 3, gen2xx(op + reg);
}

// call proper code generator
void prs8051(void)
{
	enum mod8051 mod = NONE;
	char *p;

	if (!*op)
	{
		prblank();
		return;
	}
	if ((info8051 = find8051(op)) == NULL)
	{
		if (MACACT)
			MACACT = FALSE;
		else
			opcerr();
		return;
	}
	p = strchr(arg, ',');
	if (arg[0] == 'A')
	{
		if (arg[1] == ',')
		{
			if (info8051->opcodes[NONE] == N_CJNE || info8051->opcodes[NONE] == N_INDB)
				mod = NONE;
			else if (isreg(arg + 2))
				mod = AREG,	arg += 2;
			else if (arg[2] == '@' && (isreg(arg + 3) || isdptr(arg + 3)))
				mod = AIND,	arg += 3;
			else if (arg[2] == '#')
				mod = AIMM, arg += 2;
			else
				mod = ADIR, arg += 2;
		}
		else if (arg[1] == 0)
			mod = ACC;
	}
	else if (arg[0] == 'C' && arg[1] == ',')
	{
		if (arg[2] == '!' || arg[2] == '/')
			mod = CINV, arg += 3;
		else
			mod = CARR,	arg += 2;
	}
	else if (arg[0] == '@' && (isreg(arg + 1) || isdptr(arg + 1)))
	{
		if (p == 0)
			mod = IREG,	arg++;
		else if (info8051->opcodes[NONE])
			mod = NONE;
		else
			mod = IREG,	arg++;
	}
	else if (streq(arg, "DPTR"))
	{
		if (info8051->opcodes[NONE])
			mod = NONE;
		else
			mod = REG;
	}
	else if (isreg(arg) && arg[2]==0)
	{
		mod = REG;
	}
	else if (p[1] == 'A' && p[2] == 0)
	{
		if (info8051->opcodes[NONE])
			mod = NONE;
		else
			mod = MDIR, *p = 0;
	}
	else if (p[1] == '#')
	{
		if (info8051->opcodes[NONE])
			mod = NONE;
		else
			mod = MIMM, *p = 0, p += 2;
	}
	else
	{
		if (info8051->opcodes[NONE])
			mod = NONE;
		else
			mod = JMP;
	}
	switch (mod)
	{
		case AREG:
		case REG:
			genr07(okop8051(mod));
			break;
		case AIND:
		case IREG:
			genr01(okop8051(mod));
			break;
		case ADIR:
		case MDIR:
			gen2xx(okop8051(mod));
			break;
		case CARR:
		case CINV:
			gencarr(okop8051(mod));
			break;
		case AIMM:
			arg++;
			gen2xx(okop8051(AIMM));
			break;
		case ACC:
			gen1b(okop8051(ACC));
			break;
		case JMP:
		{
			ADDRESS dat;

			getlbl(arg,&dat);
			genrb(okop8051(JMP), dat);
			break;
		}
		case MIMM:
			gen3xx(okop8051(MIMM), arg, p);
			break;
		case NONE:
			switch (info8051->opcodes[NONE])
			{
				case N_NONE:
					gen1b(okop8051(JMP));
					break;
				case N_MATH:
					gen1b(okop8051(ACC));
					break;
				case N_JMP:
					genjmp(okop8051(JMP), 0);
					break;
				case N_BJMP:
					genjmp(okop8051(JMP), 1);
					break;
				case N_DJNZ:
					genDJNZ(okop8051(JMP));
					break;
				case N_CJNE:
					genCJNE(okop8051(JMP));
					break;
				case N_BITF:
					genbitf(okop8051(JMP));
					break;
				case N_MDIR:
					if (streq(arg, "DPTR"))
						gen1b(okop8051(AREG));
					else
						gen2xx(okop8051(JMP));
					break;
				case N_MOV:
					if (arg[0] == '@')
					{
						arg++;
						if (arg[3] == '#')
							genr01(0x76);
						else if (arg[3] == 'A' && arg[4] == 0)
							genr01(0xF6);
						else
							genr01(0xA6);
					}
					else if (isreg(arg))
					{
						if (arg[3] == '#')
							genr07(0x78);
						else if (arg[3] == 'A' && arg[4] == 0)
							genr07(0xF8);
						else
							genr07(0xA8);
					}
					else if (isdptr(arg) && arg[4]==',')
					{
						WORD adr;

						getword(arg+6, &adr);
						gen3b(0x90, adr);
					}
					else if (p)
					{
						*p++ = 0;
						if (p[0]=='@' && isreg(p + 1))
							gen2xx(0x86 + (p[2] - '0'));
						else if (isreg(p) && p[2]==0)
							gen2xx(0x88 + (p[1] - '0'));
						else if (p[0] == '#')
							gen3xx(0x75, arg, p+1);
						else if (streq(p, "C"))
							gencarr(0x92);
						else if (streq(p, "A"))
							gen2xx(0xF5);
						else
							gen3sx(0x85, p, arg);
					}
					else
						argerr();
					break;
				case N_INDA:
					if (streq(arg, "@A+DPTR"))
						gen1b(okop8051(AIND));
					else
						genjmp(okop8051(JMP), 1);	// assume a long jump
					break;
				case N_INDB:
					if (streq(arg, "A,@A+PC"))
						gen1b(okop8051(AREG));
					else if (streq(arg, "A,@A+DPTR"))
						gen1b(okop8051(AIND));
					else
						argerr();
					break;
				case N_FLAG:
					if (streq(arg, "A"))
						gen1b(okop8051(ACC));
					else if (streq(arg, "C"))
						gen1b(okop8051(AREG));
					else
						gencarr(okop8051(AIND));
					break;
			}
			break;
	}
}

