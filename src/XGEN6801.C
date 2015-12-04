// XGEN6801.C - 6801 source parser for XASM.C

#include "xasm.h"

// addressing modes
enum mod6801 {
	IMP,	// implied
	REL,	// relative
	ACA,	// accumulator A
	ACB,	// accumulator B
	XRG,	// X register
	IMM,	// immediate
	DIR,	// direct (zero page)
	EXT,	// extended (absolute)
	IDX,	// indexed
	IMX,	// immediate bit, indexed
	IMD,	// immediate bit, direct
	IM2		// immediate double
};

struct entry6801 {
	char *opname;
	char opcodes[12];
};

static struct entry6801 ops6801[] = {
//				IMP  REL  ACA  ACB  XRG  IMM  DIR  EXT  IDX  IMX  IMD  IM2
{ "ABA",  { 0x1B,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "ABX",  { 0x3A,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "ADCA", { 0,   0,   0,   0,   0,   0x89,0x99,0xB9,0xA9,0,   0,   0		}},
{ "ADCB", { 0,   0,   0,   0,   0,   0xC9,0xD9,0xF9,0xE9,0,   0,   0		}},
{ "ADDA", { 0,   0,   0,   0,   0,   0x8B,0x9B,0xBB,0xAB,0,   0,   0		}},
{ "ADDB", { 0,   0,   0,   0,   0,   0xCB,0xDB,0xFB,0xEB,0,   0,   0		}},
{ "ADDD", { 0,   0,   0,   0,   0,   0,   0xD3,0xF3,0xE3,0,   0,   0xC3	}},
{ "AIM",  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0x61,0x71,0		}},
{ "ANDA", { 0,   0,   0,   0,   0,   0x84,0x94,0xB4,0xA4,0,   0,   0		}},
{ "ANDB", { 0,   0,   0,   0,   0,   0xC4,0xD4,0xF4,0xE4,0,   0,   0		}},
{ "ASL",  { 0,   0,   0x48,0x58,0,   0,   0,   0x78,0x68,0,   0,   0		}},
{ "ASLD", { 0x05,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "ASR",  { 0,   0,   0x47,0x57,0,   0,   0,   0x77,0x67,0,   0,   0		}},
{ "BCC",  { 0,   0x24,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "BCS",  { 0,   0x25,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "BEQ",  { 0,   0x27,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "BGE",  { 0,   0x2C,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "BGEU", { 0,   0x24,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "BGT",  { 0,   0x2E,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "BGTU", { 0,   0x22,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "BHI",  { 0,   0x22,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "BHS",  { 0,   0x24,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "BITA", { 0,   0,   0,   0,   0,   0x85,0x95,0xB5,0xA5,0,   0,   0		}},
{ "BITB", { 0,   0,   0,   0,   0,   0xC5,0xD5,0xF5,0xE5,0,   0,   0		}},
{ "BLE",  { 0,   0x2F,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "BLEU", { 0,   0x23,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "BLO",  { 0,   0x25,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "BLS",  { 0,   0x23,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "BLT",  { 0,   0x2D,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "BLTU", { 0,   0x25,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "BMI",  { 0,   0x2B,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "BNE",  { 0,   0x26,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "BPL",  { 0,   0x2A,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "BRA",  { 0,   0x20,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "BRN",  { 0,   0x21,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "BSR",  { 0,   0x8D,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "BVC",  { 0,   0x28,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "BVS",  { 0,   0x29,0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "CBA",  { 0x11,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "CLC",  { 0x0C,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "CLI",  { 0x0E,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "CLR",  { 0,   0,   0x4F,0x5F,0,   0,   0,   0x7F,0x6F,0,   0,   0		}},
{ "CLV",  { 0x0A,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "CMPA", { 0,   0,   0,   0,   0,   0x81,0x91,0xB1,0xA1,0,   0,   0		}},
{ "CMPB", { 0,   0,   0,   0,   0,   0xC1,0xD1,0xF1,0xE1,0,   0,   0		}},
{ "CMPX", { 0,   0,   0,   0,   0,   0,   0x9C,0xBC,0xAC,0,   0,   0x8C	}},
{ "COM",  { 0,   0,   0x43,0x53,0,   0,   0,   0x73,0x63,0,   0,   0		}},
{ "CPA",  { 0,   0,   0,   0,   0,   0x81,0x91,0xB1,0xA1,0,   0,   0		}},
{ "CPB",  { 0,   0,   0,   0,   0,   0xC1,0xD1,0xF1,0xE1,0,   0,   0		}},
{ "CPX",  { 0,   0,   0,   0,   0,   0,   0x9C,0xBC,0xAC,0,   0,   0x8C	}},
{ "DAA",  { 0x19,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "DEC",  { 0,   0,   0x4A,0x5A,0x09,0,   0,   0x7A,0x6A,0,   0,   0		}},
{ "DECA", { 0x4A,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "DECB", { 0x5A,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "DECX", { 0x09,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "DES",  { 0x34,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "DEX",  { 0x09,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "EIM",  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0x65,0x75,0		}},
{ "EORA", { 0,   0,   0,   0,   0,   0x88,0x98,0xB8,0xA8,0,   0,   0		}},
{ "EORB", { 0,   0,   0,   0,   0,   0xC8,0xD8,0xF8,0xE8,0,   0,   0		}},
{ "INC",  { 0,   0,   0x4C,0x5C,0x08,0,   0,   0x7C,0x6C,0,   0,   0		}},
{ "INCA", { 0x4C,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "INCB", { 0x5C,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "INCX", { 0x08,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "INS",  { 0x31,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "INX",  { 0x08,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "JMP",  { 0,   0,   0,   0,   0,   0,   0,   0x7E,0x6E,0,   0,   0		}},
{ "JSR",  { 0,   0,   0,   0,   0,   0,   0x9D,0xBD,0xAD,0,   0,   0		}},
{ "LDAA", { 0,   0,   0,   0,   0,   0x86,0x96,0xB6,0xA6,0,   0,   0		}},
{ "LDAB", { 0,   0,   0,   0,   0,   0xC6,0xD6,0xF6,0xE6,0,   0,   0		}},
{ "LDD",  { 0,   0,   0,   0,   0,   0,   0xDC,0xFC,0xEC,0,   0,   0xCC	}},
{ "LDS",  { 0,   0,   0,   0,   0,   0,   0x9E,0xBE,0xAE,0,   0,   0x8E	}},
{ "LDX",  { 0,   0,   0,   0,   0,   0,   0xDE,0xFE,0xEE,0,   0,   0xCE	}},
{ "LSL",  { 0,   0,   0x48,0x58,0,   0,   0,   0x78,0x68,0,   0,   0		}},
{ "LSR",  { 0,   0,   0x44,0x54,0,   0,   0,   0x74,0x64,0,   0,   0		}},
{ "LSRD", { 0x04,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "MUL",  { 0x3D,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "NEG",  { 0,   0,   0x40,0x50,0,   0,   0,   0x70,0x60,0,   0,   0		}},
{ "NOP",  { 0x01,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "OIM",  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0x62,0x72,0		}},
{ "ORAA", { 0,   0,   0,   0,   0,   0x8A,0x9A,0xBA,0xAA,0,   0,   0		}},
{ "ORAB", { 0,   0,   0,   0,   0,   0xCA,0xDA,0xFA,0xEA,0,   0,   0		}},
{ "PSH",  { 0,   0,   0x36,0x37,0x3C,0,   0,   0,   0,   0,   0,   0		}},
{ "PSHA", { 0x36,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "PSHB", { 0x37,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "PSHX", { 0x3C,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "PUL",  { 0,   0,   0x32,0x33,0x38,0,   0,   0,   0,   0,   0,   0		}},
{ "PULA", { 0x32,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "PULB", { 0x33,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "PULX", { 0x38,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "ROL",  { 0,   0,   0x49,0x59,0,   0,   0,   0x79,0x69,0,   0,   0		}},
{ "ROR",  { 0,   0,   0x46,0x56,0,   0,   0,   0x76,0x66,0,   0,   0		}},
{ "RTI",  { 0x3B,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "RTS",  { 0x39,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "SBA",  { 0x10,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "SBCA", { 0,   0,   0,   0,   0,   0x82,0x92,0xB2,0xA2,0,   0,   0		}},
{ "SBCB", { 0,   0,   0,   0,   0,   0xC2,0xD2,0xF2,0xE2,0,   0,   0		}},
{ "SEC",  { 0x0D,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "SEI",  { 0x0F,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "SEV",  { 0x0B,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "SLEEP",{ 0x1A,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "SLP",  { 0x1A,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "STAA", { 0,   0,   0,   0,   0,   0,   0x97,0xB7,0xA7,0,   0,   0		}},
{ "STAB", { 0,   0,   0,   0,   0,   0,   0xD7,0xF7,0xE7,0,   0,   0		}},
{ "STD",  { 0,   0,   0,   0,   0,   0,   0xDD,0xFD,0xED,0,   0,   0		}},
{ "STS",  { 0,   0,   0,   0,   0,   0,   0x9F,0xBF,0xAF,0,   0,   0		}},
{ "STX",  { 0,   0,   0,   0,   0,   0,   0xDF,0xFF,0xEF,0,   0,   0		}},
{ "SUBA", { 0,   0,   0,   0,   0,   0x80,0x90,0xB0,0xA0,0,   0,   0		}},
{ "SUBB", { 0,   0,   0,   0,   0,   0xC0,0xD0,0xF0,0xE0,0,   0,   0		}},
{ "SUBD", { 0,   0,   0,   0,   0,   0,   0x93,0xB3,0xA3,0,   0,   0x83	}},
{ "SWI",  { 0x3F,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "TAB",  { 0x16,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "TAP",  { 0x06,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "TBA",  { 0x17,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "TIM",  { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0x6B,0x7B,0		}},
{ "TPA",  { 0x07,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "TST",  { 0,   0,   0x4D,0x5D,0,   0,   0,   0x7D,0x6D,0,   0,   0		}},
{ "TSX",  { 0x30,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "TXS",  { 0x35,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "WAI",  { 0x3E,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "WAIT", { 0x3E,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}},
{ "XGDX", { 0x18,0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0		}}
};

static struct entry6801 *info6801;

// test for valid opcode
static short okop6801(short m)
{
	register short c;

	if ((c = info6801->opcodes[m]) == 0)
		moderr();
	return(c & 0xFF);
}

// binary search for opcode mnemonic
static struct entry6801 *find6801(char *searchfor)
{
	struct entry6801 *i;
	register short lo, hi, x, nfind;

	lo = 0;
	hi = dim(ops6801)-1;
	do
	{
		i=ops6801 + (x = (lo+hi) >> 1);
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

// immediate memory object generator
static void geni6801(WORD vopc)
{
	WORD vbyt, vadr;

	scanptr = arg;
	getword(scancom(),&vbyt);
	test_byte(&vbyt);
	if (*scanptr == '#')
		scanptr++;
	getword(scancom(),&vadr);
	test_byte(&vadr);
	prdata(vopc,vbyt,vadr);
	pc += 3L;
}

// call proper code generator
void prs6801(void)
{
	ADDRESS dat;
	enum mod6801 mod;

	if (*op == 0)
	{
		prblank();
		return;
	}
	info6801 = find6801(op);
	if (info6801 == NULL)
	{
		if (MACACT)
			MACACT = FALSE;
		else
			opcerr();
		return;
	}
	if (!*arg)
		mod = IMP;
	else if (streq(arg,"A"))
		mod = ACA;
	else if (streq(arg,"B"))
		mod = ACB;
	else if (streq(arg,"X"))
		mod = XRG;
	else if (streq(argend-2,",X"))
	{
		*(argend-2) = 0;
		if (!info6801->opcodes[IDX])		// indexed?
			mod = IMX;
		else
			mod = IDX;
		if (*arg=='#')
			arg++;
	}
	else if (*arg=='#')
	{
		arg++;
		if (strchr(arg,','))
			mod = IMD;
		else
			mod = IMM; // or IM2
	}
	else
	{
		if (info6801->opcodes[REL])		// relative?
			mod = REL;
		else
			mod = EXT; // or DIR
	}
	if ((mod == REL) || (mod == IMM) || (mod == EXT) || (mod == IDX))
	{
		getlbl(arg,&dat);
		if (mod == IMM)
		{
			if (info6801->opcodes[IM2])
				mod = IM2;
		}
		else if (mod == EXT && dat < 0x0100L)
		{
			if (info6801->opcodes[DIR])
				mod = DIR;
		}
	}
	switch (mod)
	{
		case IMP:
			gen1b(okop6801(IMP));
			break;
		case REL:
			genrb(okop6801(REL),dat);
			break;
		case ACA:
			gen1b(okop6801(ACA));
			break;
		case ACB:
			gen1b(okop6801(ACB));
			break;
		case XRG:
			gen1b(okop6801(XRG));
			break;
		case IMM:
			gen2b(okop6801(IMM),(WORD)dat);
			break;
		case DIR:
			gen2b(okop6801(DIR),(WORD)dat);
			break;
		case EXT:
			gen3b(okop6801(EXT),(WORD)dat);
			break;
		case IDX:
			gen2b(okop6801(IDX),(WORD)dat);
			break;
		case IMX:
			geni6801(okop6801(IMX));
			break;
		case IMD:
			geni6801(okop6801(IMD));
			break;
		case IM2:
			gen3b(okop6801(IM2),(WORD)dat);
			break;
	}
}

