// XGEN6805.C - 6805 source parser for XASM.C

#include "xasm.h"

// addressing modes
enum mod6805 {
	IMP,	// implied
	REL,	// relative
	ACC,	// accumulator (A register)
	XRG,	// X register
	IMM,	// immediate
	DIR,	// direct (zero page)
	EXT,	// extended (absolute)
	IDX,	// indexed -, X
	IX1,	// indexed 1 byte - nn,X
	IX2,	// indexed 2 byte - nnnn,X
	BIT		// bit process
};

struct entry6805 {
	char *opname;
	char opcodes[11];
};

static struct entry6805 ops6805[] = {
//				IMP  REL  ACC  XRG  IMM  DIR  EXT  IDX  IX1  IX2  BIT
{ "ADC",  { 0,   0,   0,   0,   0xA9,0xB9,0xC9,0xF9,0xE9,0xD9,0	}},
{ "ADD",  { 0,   0,   0,   0,   0xAB,0xBB,0xCB,0xFB,0xEB,0xDB,0	}},
{ "AND",  { 0,   0,   0,   0,   0xA4,0xB4,0xC4,0xF4,0xE4,0xD4,0	}},
{ "ASL",  { 0,   0,   0x48,0x58,0,   0x38,0,   0x78,0x68,0,   0	}},
{ "ASLA", { 0x48,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "ASLX", { 0x58,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "ASR",  { 0,   0,   0x47,0x57,0,   0x37,0,   0x77,0x67,0,   0	}},
{ "ASRA", { 0x47,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "ASRX", { 0x57,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BCC",  { 0,   0x24,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BCLR", { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0x01}},
{ "BCS",  { 0,   0x25,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BEQ",  { 0,   0x27,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BGE",  { 0,   0x24,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BGT",  { 0,   0x22,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BHCC", { 0,   0x28,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BHCS", { 0,   0x29,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BHI",  { 0,   0x22,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BHS",  { 0,   0x24,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BIH",  { 0,   0x2F,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BIL",  { 0,   0x2E,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BIT",  { 0,   0,   0,   0,   0xA5,0xB5,0xC5,0xF5,0xE5,0xD5,0	}},
{ "BLE",  { 0,   0x23,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BLO",  { 0,   0x25,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BLS",  { 0,   0x23,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BLT",  { 0,   0x25,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BMC",  { 0,   0x2C,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BMI",  { 0,   0x2B,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BMS",  { 0,   0x2D,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BNE",  { 0,   0x26,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BPL",  { 0,   0x2A,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BRA",  { 0,   0x20,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BRCLR",{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0x03}},
{ "BRN",  { 0,   0x21,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "BRSET",{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0x04}},
{ "BSET", { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0x02}},
{ "BSR",  { 0,   0xAD,0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "CLC",  { 0x98,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "CLI",  { 0x9A,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "CLR",  { 0,   0,   0x4F,0x5F,0,   0x3F,0,   0x7F,0x6F,0,   0	}},
{ "CLRA", { 0x4F,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "CLRX", { 0x5F,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "CMP",  { 0,   0,   0,   0,   0xA1,0xB1,0xC1,0xF1,0xE1,0xD1,0	}},
{ "COM",  { 0,   0,   0x43,0x53,0,   0x33,0,   0x73,0x63,0,   0	}},
{ "COMA", { 0x43,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "COMX", { 0x53,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "CPX",  { 0,   0,   0,   0,   0xA3,0xB3,0xC3,0xF3,0xE3,0xD3,0	}},
{ "DEC",  { 0,   0,   0x4A,0x5A,0,   0x3A,0,   0x7A,0x6A,0,   0	}},
{ "DECA", { 0x4A,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "DECX", { 0x5A,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "DEX",  { 0x5A,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "EOR",  { 0,   0,   0,   0,   0xA8,0xB8,0xC8,0xF8,0xE8,0xD8,0	}},
{ "INC",  { 0,   0,   0x4C,0x5C,0,   0x3C,0,   0x7C,0x6C,0,   0	}},
{ "INCA", { 0x4C,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "INCX", { 0x5C,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "INX",  { 0x5C,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "JMP",  { 0,   0,   0,   0,   0,   0xBC,0xCC,0xFC,0xEC,0xDC,0	}},
{ "JSR",  { 0,   0,   0,   0,   0,   0xBD,0xCD,0xFD,0xED,0xDD,0	}},
{ "LDA",  { 0,   0,   0,   0,   0xA6,0xB6,0xC6,0xF6,0xE6,0xD6,0	}},
{ "LDX",  { 0,   0,   0,   0,   0xAE,0xBE,0xCE,0xFE,0xEE,0xDE,0	}},
{ "LSL",  { 0,   0,   0x48,0x58,0,   0x38,0,   0x78,0x68,0,   0	}},
{ "LSLA", { 0x48,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "LSLX", { 0x58,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "LSR",  { 0,   0,   0x44,0x54,0,   0x34,0,   0x74,0x64,0,   0	}},
{ "LSRA", { 0x44,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "LSRX", { 0x54,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "NEG",  { 0,   0,   0x40,0x50,0,   0x30,0,   0x70,0x60,0,   0	}},
{ "NEGA", { 0x40,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "NEGX", { 0x50,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "NOP",  { 0x9D,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "ORA",  { 0,   0,   0,   0,   0xAA,0xBA,0xCA,0xFA,0xEA,0xDA,0	}},
{ "ROL",  { 0,   0,   0x49,0x59,0,   0x39,0,   0x79,0x69,0,   0	}},
{ "ROLA", { 0x49,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "ROLX", { 0x59,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "ROR",  { 0,   0,   0x46,0x56,0,   0x36,0,   0x76,0x66,0,   0	}},
{ "RORA", { 0x46,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "RORX", { 0x56,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "RSP",  { 0x9C,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "RTI",  { 0x80,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "RTS",  { 0x81,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "SBC",  { 0,   0,   0,   0,   0xA2,0xB2,0xC2,0xF2,0xE2,0xD2,0	}},
{ "SEC",  { 0x99,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "SEI",  { 0x9B,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "STA",  { 0,   0,   0,   0,   0,   0xB7,0xC7,0xF7,0xE7,0xD7,0	}},
{ "STOP", { 0x8E,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "STX",  { 0,   0,   0,   0,   0,   0xBF,0xCF,0xFF,0xEF,0xDF,0	}},
{ "SUB",  { 0,   0,   0,   0,   0xA0,0xB0,0xC0,0xF0,0xE0,0xD0,0	}},
{ "SWI",  { 0x83,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "TAX",  { 0x97,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "TST",  { 0,   0,   0x4D,0x5D,0,   0x3D,0,   0x7D,0x6D,0,   0	}},
{ "TSTA", { 0x4D,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "TSTX", { 0x5D,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "TXA",  { 0x9F,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}},
{ "WAIT", { 0x8F,0,   0,   0,   0,   0,   0,   0,   0,   0,   0	}}
};

static struct entry6805 *info6805;

// test for valid opcode
static short okop6805(short m)
{
	register short c;

	if ((c = info6805->opcodes[m]) == 0)
		if (!streq(info6805->opname,"NOP"))
			moderr();
	return(c & 0xFF);
}

// binary search for opcode mnemonic
static struct entry6805 *find6805(char *searchfor)
{
	struct entry6805 *i;
	register short lo, hi, x, nfind;

	lo = 0;
	hi = dim(ops6805)-1;
	do {
		i=ops6805 + (x = (lo+hi) >> 1);
		nfind = strcmp(i->opname, searchfor);
		if (nfind < 0) lo = x+1;
			else if (nfind) hi = x-1;	// must be +1
			else break;				// got a match
	} while (lo<=hi);
	if (nfind)
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

// bit process object generator
static void genb6805(WORD i)
{
	short vopc;
	WORD vbit, vbyt;

	scanptr = arg;
	getword(scancom(),&vbit);
	test_oor((short *)&vbit,0,7);
	getword(scancom(),&vbyt);
	test_byte(&vbyt);
	if (i > 2)
	{
		short vrel;
		ADDRESS vadr;

		if (i == 3)
			vopc = 2 * (short)vbit + 1;
		else
			vopc = 2 * (short)vbit;
		getlbl(scancom(),&vadr);
		vrel = make_rel(vadr,3);
		prdata(vopc,(short)vbyt,vrel);
		pc += 3L;
	}
	else
	{
		if (i == 1)
			vopc = 2 * (short)vbit + 0x11;
		else
			vopc = 2 * (short)vbit + 0x10;
		prdata(vopc,(short)vbyt,UNUSED);
		pc += 2L;
	}
}

// call proper code generator
void prs6805(void)
{
	ADDRESS dat;
	enum mod6805 mod;

	if (!*op)
	{
		prblank();
		return;
	}
	info6805 = find6805(op);
	if (info6805 == NULL)
	{
		if (MACACT)
			MACACT = FALSE;
		else
			opcerr();
		return;
	}
	if (!*arg)
		mod = IMP;
	else if (*arg=='#')
		mod = IMM;
	else if (streq(arg,"A"))
		mod = ACC;
	else if (streq(arg,"X"))
		mod = XRG;
	else if (streq(arg,",X"))
		mod = IDX;
	else if (streq(argend-2,",X"))
	{
		*(argend-2) = 0;
		mod = IX2;
	}
	else
	{
		if (info6805->opcodes[REL])			// relative?
			mod = REL;
		else if (info6805->opcodes[BIT])	// bit process?
			mod = BIT;
		else
			mod = EXT;
	}
	if ((mod == REL) || (mod == EXT) || (mod == IX2))
	{
		getlbl(arg,&dat);
		if ((mod == EXT) && (dat < 0x0100L))
		{
			if (info6805->opcodes[DIR])
				mod = DIR;
		}
		else if ((mod == IX2) && (dat < 0x0100L))
		{
			if (info6805->opcodes[IX1])
				mod = IX1;
		}
	}
	switch (mod)
	{
		case IMP:
			gen1b(okop6805(IMP));
			break;
		case REL:
			genrb(okop6805(REL),dat);
			break;
		case ACC:
			gen1b(okop6805(ACC));
			break;
		case XRG:
			gen1b(okop6805(XRG));
			break;
		case IMM:
			gendb(okop6805(IMM));
			break;
		case DIR:
			gen2b(okop6805(DIR),(WORD)dat);
			break;
		case EXT:
			gen3b(okop6805(EXT),(WORD)dat);
			break;
		case IDX:
			gen1b(okop6805(IDX));
			break;
		case IX1:
			gen2b(okop6805(IX1),(WORD)dat);
			break;
		case IX2:
			gen3b(okop6805(IX2),(WORD)dat);
			break;
		case BIT:
			genb6805(okop6805(BIT));
			break;
	}
}

