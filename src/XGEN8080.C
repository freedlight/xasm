// XGEN8080.C - 8080 source parser for XASM.C

#include "xasm.h"

// addressing modes
enum mod8080 {
	IMP,	// implied
	IMM8,	// immediate, 8 bit
	IMM16,	// immediate, 16 bit
	R8,		// register, 8 bit
	REG,	// register, 16 bit
	BRA,	// branch
	RIMM,	// register, 16 bit with immediate
	R8R8,	// register->register
	RST		// reset vector
};

struct entry8080 {
	char *opname;
	char opcodes[9];
	char flag;	// special flag for 8085; 1=only on 8085
};

static struct entry8080 ops8080[] = {
//			  IMP  IMM8 IMM16 R8  REG  BRA  RIMM R8R8 RST  8085
{ "ACI", { 0,   0xCE,0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "ADC", { 0,   0,   0,   0x88,0,   0,   0,   0,   0 }, 0 },
{ "ADD", { 0,   0,   0,   0x80,0,   0,   0,   0,   0 }, 0 },
{ "ADI", { 0,   0xC6,0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "ANA", { 0,   0,   0,   0xA0,0,   0,   0,   0,   0 }, 0 },
{ "ANI", { 0,   0xE6,0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "CALL",{ 0,   0,   0,   0,   0,   0xCD,0,   0,   0 }, 0 },
{ "CC",  { 0,   0,   0,   0,   0,   0xDC,0,   0,   0 }, 0 },
{ "CM",  { 0,   0,   0,   0,   0,   0xFC,0,   0,   0 }, 0 },
{ "CMA", { 0x2F,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "CMC", { 0x3F,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "CMP", { 0,   0,   0,   0xB8,0,   0,   0,   0,   0 }, 0 },
{ "CNC", { 0,   0,   0,   0,   0,   0xD4,0,   0,   0 }, 0 },
{ "CNZ", { 0,   0,   0,   0,   0,   0xC4,0,   0,   0 }, 0 },
{ "CP",  { 0,   0,   0,   0,   0,   0xF4,0,   0,   0 }, 0 },
{ "CPE", { 0,   0,   0,   0,   0,   0xEC,0,   0,   0 }, 0 },
{ "CPI", { 0,   0xFE,0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "CPO", { 0,   0,   0,   0,   0,   0xE4,0,   0,   0 }, 0 },
{ "CZ",  { 0,   0,   0,   0,   0,   0xCC,0,   0,   0 }, 0 },
{ "DAA", { 0x27,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "DAD", { 0,   0,   0,   0,   0x09,0,   0,   0,   0 }, 0 },
{ "DCR", { 0,   0,   0,   0,   0x05,0,   0,   0,   0 }, 0 },
{ "DCX", { 0,   0,   0,   0,   0x0B,0,   0,   0,   0 }, 0 },
{ "DI",  { 0xF3,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "EI",  { 0xFB,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "HLT", { 0x76,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "IN",  { 0,   0xDB,0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "INR", { 0,   0,   0,   0,   0x04,0,   0,   0,   0 }, 0 },
{ "INX", { 0,   0,   0,   0,   0x03,0,   0,   0,   0 }, 0 },
{ "JC",  { 0,   0,   0,   0,   0,   0xDA,0,   0,   0 }, 0 },
{ "JM",  { 0,   0,   0,   0,   0,   0xFA,0,   0,   0 }, 0 },
{ "JMP", { 0,   0,   0,   0,   0,   0xC3,0,   0,   0 }, 0 },
{ "JNC", { 0,   0,   0,   0,   0,   0xD2,0,   0,   0 }, 0 },
{ "JNZ", { 0,   0,   0,   0,   0,   0xC2,0,   0,   0 }, 0 },
{ "JP",  { 0,   0,   0,   0,   0,   0xF2,0,   0,   0 }, 0 },
{ "JPE", { 0,   0,   0,   0,   0,   0xEA,0,   0,   0 }, 0 },
{ "JPO", { 0,   0,   0,   0,   0,   0xE2,0,   0,   0 }, 0 },
{ "JZ",  { 0,   0,   0,   0,   0,   0xCA,0,   0,   0 }, 0 },
{ "LDA", { 0,   0,   0x3A,0,   0,   0,   0,   0,   0 }, 0 },
{ "LDAX",{ 0,   0,   0,   0,   0x0A,0,   0,   0,   0 }, 0 },
{ "LHLD",{ 0,   0,   0x2A,0,   0,   0,   0,   0,   0 }, 0 },
{ "LXI", { 0,   0,   0,   0,   0,   0,   0x01,0,   0 }, 0 },
{ "MOV", { 0,   0,   0,   0,   0,   0,   0,   0x01,0 }, 0 },
{ "MVI", { 0,   0,   0,   0,   0,   0,   0x02,0,   0 }, 0 },
{ "NOP", { 0x00,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "ORA", { 0,   0,   0,   0xB0,0,   0,   0,   0,   0 }, 0 },
{ "ORI", { 0,   0xF6,0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "OUT", { 0,   0xD3,0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "PCHL",{ 0xE9,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "POP", { 0,   0,   0,   0,   0xC1,0,   0,   0,   0 }, 0 },
{ "PUSH",{ 0,   0,   0,   0,   0xC5,0,   0,   0,   0 }, 0 },
{ "RAL", { 0x17,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "RAR", { 0x1F,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "RC",  { 0xD8,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "RET", { 0xC9,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "RIM", { 0x20,0,   0,   0,   0,   0,   0,   0,   0 }, 1 },
{ "RLC", { 0x07,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "RM",  { 0xF8,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "RNC", { 0xD0,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "RNZ", { 0xC0,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "RP",  { 0xF0,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "RPE", { 0xE8,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "RPO", { 0xE0,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "RRC", { 0x0F,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "RST", { 0,   0,   0,   0,   0,   0,   0,   0,   1 }, 0 },
{ "RZ",  { 0xC8,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "SBB", { 0,   0,   0,   0x98,0,   0,   0,   0,   0 }, 0 },
{ "SBI", { 0,   0xDE,0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "SHLD",{ 0,   0,   0x22,0,   0,   0,   0,   0,   0 }, 0 },
{ "SIM", { 0x30,0,   0,   0,   0,   0,   0,   0,   0 }, 1 },
{ "SPHL",{ 0xF9,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "STA", { 0,   0,   0x32,0,   0,   0,   0,   0,   0 }, 0 },
{ "STAX",{ 0,   0,   0,   0,   0x02,0,   0,   0,   0 }, 0 },
{ "STC", { 0x37,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "SUB", { 0,   0,   0,   0x90,0,   0,   0,   0,   0 }, 0 },
{ "SUI", { 0,   0xD6,0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "XCHG",{ 0xEB,0,   0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "XRA", { 0,   0,   0,   0xA8,0,   0,   0,   0,   0 }, 0 },
{ "XRI", { 0,   0xEE,0,   0,   0,   0,   0,   0,   0 }, 0 },
{ "XTHL",{ 0xE3,0,   0,   0,   0,   0,   0,   0,   0 }, 0 }
};

static struct entry8080 *info8080;

// test for valid opcode
static short okop8080(short mode)
{
	register short c;

	if (info8080->flag > 0)
	{
		if (!streq(cpu,"8085"))
		{
			opcerr();
			return(0);
		}
	}
	if ((c = info8080->opcodes[mode]) == 0)
		if (!streq(info8080->opname,"NOP"))
			moderr();
	return(c & 0xFF);
}

// binary search for opcode mnemonic
static struct entry8080 *find8080(char *searchfor)
{
	struct entry8080 *i;
	register short lo, hi, x, nfind;

	lo = 0;
	hi = dim(ops8080)-1;
	do {
		i = ops8080 + (x = (lo + hi) >> 1);
		nfind = strcmp(i->opname, searchfor);
		if (nfind < 0)
			lo = x + 1;
		else if (nfind > 0)
			hi = x - 1;	// must be +1
		else
			break;		// got a match
	} while (lo <= hi);
	if (nfind != 0)	// not found
	{
		if ((x = findmac(searchfor)) == -1)
			return(NULL);
		prblank();
		MACACT = TRUE;
		getmac(x);
		return(NULL);
	}
	else
		return(i);
}

// register process a
static void genra8080(WORD opc)
{
	short reg,i;

	reg = arg[0];
	switch (reg)
	{
		case 'A': i = 7; break;
		case 'B': i = 0; break;
		case 'C': i = 1; break;
		case 'D': i = 2; break;
		case 'E': i = 3; break;
		case 'H': i = 4; break;
		case 'L': i = 5; break;
		case 'M': i = 6; break;
		default:  regerr(reg);
	}
	gen1b(opc+i);
}

// register process b
static void genrb8080(WORD opc)
{
	short reg,mod;

	switch (opc)
	{
		case 0x02: mod = 1; break;
		case 0x03: mod = 2; break;
		case 0x04: mod = 4; break;
		case 0x05: mod = 4; break;
		case 0x09: mod = 2; break;
		case 0x0A: mod = 1; break;
		case 0x0B: mod = 2; break;
		case 0xC1: mod = 3; break;
		case 0xC5: mod = 3; break;
		default:   mod = 0; moderr();
	}
	reg = arg[0];
	if (mod == 1)
	{
		switch (reg)
		{
			case 'B': gen1b(opc + 0x00); break;
			case 'D': gen1b(opc + 0x10); break;
			default:  regerr(reg);
		}
	}
	else if (mod == 2)
	{
		switch (reg)
		{
			case 'B': gen1b(opc + 0x00); break;
			case 'D': gen1b(opc + 0x10); break;
			case 'H': gen1b(opc + 0x20); break;
			case 'S': gen1b(opc + 0x30); break;
			default:  regerr(reg);
		}
	}
	else if (mod == 3)
	{
		switch (reg)
		{
			case 'B': gen1b(opc + 0x00); break;
			case 'D': gen1b(opc + 0x10); break;
			case 'H': gen1b(opc + 0x20); break;
			case 'P': gen1b(opc + 0x30); break;
			default:  regerr(reg);
		}
	}
	else if (mod == 4)
	{
		switch (reg)
		{
			case 'A': gen1b(opc + 0x38); break;
			case 'B': gen1b(opc + 0x00); break;
			case 'C': gen1b(opc + 0x08); break;
			case 'D': gen1b(opc + 0x10); break;
			case 'E': gen1b(opc + 0x18); break;
			case 'H': gen1b(opc + 0x20); break;
			case 'L': gen1b(opc + 0x28); break;
			case 'M': gen1b(opc + 0x30); break;
			default:  regerr(reg);
		}
	}
}

// register with immed data process
static void genri8080(WORD opc)
{
	short r1;
	WORD dat;
	char *reg;

	scanptr = arg;
	reg = scancom();
	getword(scancom(),&dat);
	r1 = *reg;
	if (opc == 1)	// LXI
	{
		switch (r1)
		{
			case 'B': gen3b(0x01,dat); break;
			case 'D': gen3b(0x11,dat); break;
			case 'H': gen3b(0x21,dat); break;
			case 'S': gen3b(0x31,dat); break;
			default:  regerr(r1);
		}
	}
	else if (opc == 2)	// MVI
	{
		switch (r1)
		{
			case 'A': gen2b(0x3E,dat); break;
			case 'B': gen2b(0x06,dat); break;
			case 'C': gen2b(0x0E,dat); break;
			case 'D': gen2b(0x16,dat); break;
			case 'E': gen2b(0x1E,dat); break;
			case 'H': gen2b(0x26,dat); break;
			case 'L': gen2b(0x2E,dat); break;
			case 'M': gen2b(0x36,dat); break;
			default:  regerr(r1);
		}
	}
	else
		moderr();
}

// register - register move process
static void genrr8080(void)
{
	short r1,r2,i;

	r1 = arg[0];
	r2 = arg[2];
	switch (r1)
	{
		case 'A': i = 0x78; break;
		case 'B': i = 0x40; break;
		case 'C': i = 0x48; break;
		case 'D': i = 0x50; break;
		case 'E': i = 0x58; break;
		case 'H': i = 0x60; break;
		case 'L': i = 0x68; break;
		case 'M': i = 0x70; break;
		default:  regerr(r1);
	}
	switch (r2)
	{
		case 'A': i += 7; break;
		case 'B': i += 0; break;
		case 'C': i += 1; break;
		case 'D': i += 2; break;
		case 'E': i += 3; break;
		case 'H': i += 4; break;
		case 'L': i += 5; break;
		case 'M': i += 6; break;
		default:  regerr(r2);
	}
	gen1b(i);
}

// reset process
static void genrs8080(WORD dat)
{
	switch (dat)
	{
		case 0: gen1b(0xC7); break;
		case 1: gen1b(0xCF); break;
		case 2: gen1b(0xD7); break;
		case 3: gen1b(0xDF); break;
		case 4: gen1b(0xE7); break;
		case 5: gen1b(0xEF); break;
		case 6: gen1b(0xF7); break;
		case 7: gen1b(0xFF); break;
		default: oorerr(dat,0,7);
	}
}

// call proper code generator
void prs8080(void)
{
	ADDRESS dat;
	short i;
	enum mod8080 mod;

	if (!*op)
	{
		prblank();
		return;
	}
	if ((info8080 = find8080(op)) == NULL)
	{
		if (MACACT)
			MACACT = FALSE;
		else
			opcerr();
		return;
	}
	if (!*arg)
		mod = IMP;
	else
	{
		for (i=0; i<9; i++)
		{
			if (info8080->opcodes[i])
			{
				mod = i;
				break;
			}
		}
	}
	if (mod==IMM8 || mod==IMM16 || mod==BRA || mod==RST)
		getlbl(arg,&dat);
	switch (mod)
	{
		case IMP:
			gen1b(okop8080(IMP));
			break;
		case IMM8:
			gen2b(okop8080(IMM8),(WORD)dat);
			break;
		case IMM16:
			gen3b(okop8080(IMM16),(WORD)dat);
			break;
		case R8:
			genra8080(okop8080(R8));
			break;
		case REG:
			genrb8080(okop8080(REG));
			break;
		case BRA:
			gen3b(okop8080(BRA),(WORD)dat);
			break;
		case RIMM:
			genri8080(okop8080(RIMM));
			break;
		case R8R8:
			genrr8080();
			break;
		case RST:
			genrs8080((WORD)dat);
			break;
	}
}

