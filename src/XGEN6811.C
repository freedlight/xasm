// XGEN6811.C - 68HC11 source parser for XASM.C

#include "xasm.h"

// addressing modes
enum mod6811 {
	INH,	// implied
	REL,	// relative
	ACA,	// accumulator A
	ACB,	// accumulator B
	XRG,	// X register
	YRG,	// X register
	IMM,	// immediate
	DIR,	// direct (zero page)
	EXT,	// extended (absolute)
	IDX,	// indexed, X
	IDY,	// indexed, Y
	IM2		// immediate double
};

struct entry6811 {
	char *opname;
	WORD opcodes[12];
};

static struct entry6811 ops6811[] = {
//			   INH    REL  ACA  ACB  XRG  YRG    IMM  DIR    EXT    IDX    IDY    IM2
{ "ABA",  { 0x001B,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "ABX",  { 0x003A,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "ABY",  { 0x183A,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "ADCA", { 0,     0,   0,   0,   0,   0,     0x89,0x0099,0x00B9,0x00A9,0x18A9,0			}},
{ "ADCB", { 0,     0,   0,   0,   0,   0,     0xC9,0x00D9,0x00F9,0x00E9,0x18E9,0			}},
{ "ADDA", { 0,     0,   0,   0,   0,   0,     0x8B,0x009B,0x00BB,0x00AB,0x18AB,0			}},
{ "ADDB", { 0,     0,   0,   0,   0,   0,     0xCB,0x00DB,0x00FB,0x00EB,0x18EB,0			}},
{ "ADDD", { 0,     0,   0,   0,   0,   0,     0,   0x00D3,0x00F3,0x00E3,0x18E3,0x00C3	}},
{ "ANDA", { 0,     0,   0,   0,   0,   0,     0x84,0x0094,0x00B4,0x00A4,0x18A4,0			}},
{ "ANDB", { 0,     0,   0,   0,   0,   0,     0xC4,0x00D4,0x00F4,0x00E4,0x18E4,0			}},
{ "ASL",  { 0,     0,   0x48,0x58,0,   0,     0,   0,     0x0078,0x0068,0x1868,0			}},
{ "ASLA", { 0x0048,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "ASLB", { 0x0058,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "ASLD", { 0x0005,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "ASR",  { 0,     0,   0x47,0x57,0,   0,     0,   0,     0x0077,0x0067,0x1867,0			}},
{ "ASRA", { 0x0047,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "ASRB", { 0x0057,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BCC",  { 0,     0x24,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BCLR", { 0,     0,   0,   0,   0,   0,     0,   0x0015,0,     0x001D,0x181D,0			}},
{ "BCS",  { 0,     0x25,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BEQ",  { 0,     0x27,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BGE",  { 0,     0x2C,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BGEU", { 0,     0x24,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BGT",  { 0,     0x2E,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BGTU", { 0,     0x22,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BHI",  { 0,     0x22,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BHS",  { 0,     0x24,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BITA", { 0,     0,   0,   0,   0,   0,     0x85,0x0095,0x00B5,0x00A5,0x18A5,0			}},
{ "BITB", { 0,     0,   0,   0,   0,   0,     0xC5,0x00D5,0x00F5,0x00E5,0x18E5,0			}},
{ "BLE",  { 0,     0x2F,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BLEU", { 0,     0x23,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BLO",  { 0,     0x25,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BLS",  { 0,     0x23,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BLT",  { 0,     0x2D,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BLTU", { 0,     0x25,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BMI",  { 0,     0x2B,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BNE",  { 0,     0x26,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BPL",  { 0,     0x2A,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BRA",  { 0,     0x20,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BRCLR",{ 0,     0,   0,   0,   0,   0,     0,   0x0013,0,     0x001F,0x181F,0			}},
{ "BRN",  { 0,     0x21,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BRSET",{ 0,     0,   0,   0,   0,   0,     0,   0x0012,0,     0x001E,0x181E,0			}},
{ "BSET", { 0,     0,   0,   0,   0,   0,     0,   0x0014,0,     0x001C,0x181C,0			}},
{ "BSR",  { 0,     0x8D,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BVC",  { 0,     0x28,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "BVS",  { 0,     0x29,0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "CBA",  { 0x0011,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "CLC",  { 0x000C,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "CLI",  { 0x000E,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "CLR",  { 0,     0,   0x4F,0x5F,0,   0,     0,   0,     0x007F,0x006F,0x186F,0			}},
{ "CLV",  { 0x000A,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "CMPA", { 0,     0,   0,   0,   0,   0,     0x81,0x0091,0x00B1,0x00A1,0x18A1,0			}},
{ "CMPB", { 0,     0,   0,   0,   0,   0,     0xC1,0x00D1,0x00F1,0x00E1,0x18E1,0			}},
{ "CMPD", { 0,     0,   0,   0,   0,   0,     0,   0x1A93,0x1AB3,0x1AA3,0xCDA3,0x1A83	}},
{ "CMPX", { 0,     0,   0,   0,   0,   0,     0,   0x009C,0x00BC,0x00AC,0xCDAC,0x008C	}},
{ "CMPY", { 0,     0,   0,   0,   0,   0,     0,   0x189C,0x18BC,0x1AAC,0x18AC,0x188C	}},
{ "COM",  { 0,     0,   0x43,0x53,0,   0,     0,   0,     0x0073,0x0063,0x1863,0			}},
{ "CPA",  { 0,     0,   0,   0,   0,   0,     0x81,0x0091,0x00B1,0x00A1,0x18A1,0			}},
{ "CPB",  { 0,     0,   0,   0,   0,   0,     0xC1,0x00D1,0x00F1,0x00E1,0x18E1,0			}},
{ "CPD",  { 0,     0,   0,   0,   0,   0,     0,   0x1A93,0x1AB3,0x1AA3,0xCDA3,0x1A83	}},
{ "CPX",  { 0,     0,   0,   0,   0,   0,     0,   0x009C,0x00BC,0x00AC,0xCDAC,0x008C	}},
{ "CPY",  { 0,     0,   0,   0,   0,   0,     0,   0x189C,0x18BC,0x1AAC,0x18AC,0x188C	}},
{ "DAA",  { 0x0019,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "DEC",  { 0,     0,   0x4A,0x5A,0x09,0x1809,0,   0,     0x007A,0x006A,0x186A,0			}},
{ "DECA", { 0x004A,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "DECB", { 0x005A,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "DECX", { 0x0009,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "DECY", { 0x1809,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "DES",  { 0x0034,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "DEX",  { 0x0009,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "DEY",  { 0x1809,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "EORA", { 0,     0,   0,   0,   0,   0,     0x88,0x0098,0x00B8,0x00A8,0x18A8,0			}},
{ "EORB", { 0,     0,   0,   0,   0,   0,     0xC8,0x00D8,0x00F8,0x00E8,0x18E8,0			}},
{ "FDIV", { 0x0003,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "IDIV", { 0x0002,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "INC",  { 0,     0,   0x4C,0x5C,0x08,0,     0,   0,     0x007C,0x006C,0x186C,0			}},
{ "INCA", { 0x004C,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "INCB", { 0x005C,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "INCX", { 0x0008,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "INCY", { 0x1808,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "INS",  { 0x0031,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "INX",  { 0x0008,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "INY",  { 0x1808,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "JMP",  { 0,     0,   0,   0,   0,   0,     0,   0,     0x007E,0x006E,0x186E,0			}},
{ "JSR",  { 0,     0,   0,   0,   0,   0,     0,   0x009D,0x00BD,0x00AD,0x18AD,0			}},
{ "LDAA", { 0,     0,   0,   0,   0,   0,     0x86,0x0096,0x00B6,0x00A6,0x18A6,0			}},
{ "LDAB", { 0,     0,   0,   0,   0,   0,     0xC6,0x00D6,0x00F6,0x00E6,0x18E6,0			}},
{ "LDD",  { 0,     0,   0,   0,   0,   0,     0,   0x00DC,0x00FC,0x00EC,0x18EC,0x00CC	}},
{ "LDS",  { 0,     0,   0,   0,   0,   0,     0,   0x009E,0x00BE,0x00AE,0x18AE,0x008E	}},
{ "LDX",  { 0,     0,   0,   0,   0,   0,     0,   0x00DE,0x00FE,0x00EE,0xCDEE,0x00CE	}},
{ "LDY",  { 0,     0,   0,   0,   0,   0,     0,   0x18DE,0x18FE,0x1AEE,0x18EE,0x18CE	}},
{ "LSL",  { 0,     0,   0x48,0x58,0,   0,     0,   0,     0x0078,0x0068,0x1868,0			}},
{ "LSLD", { 0x0005,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "LSR",  { 0,     0,   0x44,0x54,0,   0,     0,   0,     0x0074,0x0064,0x1864,0			}},
{ "LSRD", { 0x0004,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "MUL",  { 0x003D,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "NEG",  { 0,     0,   0x40,0x50,0,   0,     0,   0,     0x0070,0x0060,0x1860,0			}},
{ "NOP",  { 0x0001,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "ORAA", { 0,     0,   0,   0,   0,   0,     0x8A,0x009A,0x00BA,0x00AA,0x18AA,0			}},
{ "ORAB", { 0,     0,   0,   0,   0,   0,     0xCA,0x00DA,0x00FA,0x00EA,0x18EA,0			}},
{ "PSH",  { 0,     0,   0x36,0x37,0x3C,0x183C,0,   0,     0,     0,     0,     0			}},
{ "PSHA", { 0x0036,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "PSHB", { 0x0037,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "PSHX", { 0x003C,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "PSHY", { 0x183C,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "PUL",  { 0,     0,   0x32,0x33,0x38,0x1838,0,   0,     0,     0,     0,     0			}},
{ "PULA", { 0x0032,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "PULB", { 0x0033,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "PULX", { 0x0038,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "PULY", { 0x1838,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "ROL",  { 0,     0,   0x49,0x59,0,   0,     0,   0,     0x0079,0x0069,0x1869,0			}},
{ "ROR",  { 0,     0,   0x46,0x56,0,   0,     0,   0,     0x0076,0x0066,0x1866,0			}},
{ "RTI",  { 0x003B,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "RTS",  { 0x0039,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "SBA",  { 0x0010,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "SBCA", { 0,     0,   0,   0,   0,   0,     0x82,0x0092,0x00B2,0x00A2,0x18A2,0			}},
{ "SBCB", { 0,     0,   0,   0,   0,   0,     0xC2,0x00D2,0x00F2,0x00E2,0x18E2,0			}},
{ "SEC",  { 0x000D,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "SEI",  { 0x000F,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "SEV",  { 0x000B,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "STAA", { 0,     0,   0,   0,   0,   0,     0,   0x0097,0x00B7,0x00A7,0x18A7,0			}},
{ "STAB", { 0,     0,   0,   0,   0,   0,     0,   0x00D7,0x00F7,0x00E7,0x18E7,0			}},
{ "STD",  { 0,     0,   0,   0,   0,   0,     0,   0x00DD,0x00FD,0x00ED,0x18ED,0			}},
{ "STOP", { 0x00CF,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "STS",  { 0,     0,   0,   0,   0,   0,     0,   0x009F,0x00BF,0x00AF,0x18AF,0			}},
{ "STX",  { 0,     0,   0,   0,   0,   0,     0,   0x00DF,0x00FF,0x00EF,0xCDEF,0			}},
{ "STY",  { 0,     0,   0,   0,   0,   0,     0,   0x18DF,0x18FF,0x1AEF,0x18EF,0			}},
{ "SUBA", { 0,     0,   0,   0,   0,   0,     0x80,0x0090,0x00B0,0x00A0,0x18A0,0			}},
{ "SUBB", { 0,     0,   0,   0,   0,   0,     0xC0,0x00D0,0x00F0,0x00E0,0x18E0,0			}},
{ "SUBD", { 0,     0,   0,   0,   0,   0,     0,   0x0093,0x00B3,0x00A3,0x18A3,0x0083	}},
{ "SWI",  { 0x003F,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "TAB",  { 0x0016,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "TAP",  { 0x0006,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "TBA",  { 0x0017,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "TPA",  { 0x0007,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "TST",  { 0,     0,   0x4D,0x5D,0,   0,     0,   0,     0x007D,0x006D,0x186D,0			}},
{ "TSX",  { 0x0030,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "TSY",  { 0x1830,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "TXS",  { 0x0035,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "TYS",  { 0x1835,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "WAI",  { 0x003E,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "WAIT", { 0x003E,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "XGDX", { 0x008F,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}},
{ "XGDY", { 0x188F,0,   0,   0,   0,   0,     0,   0,     0,     0,     0,     0			}}
};

static struct entry6811 *info6811;

// test for valid opcode
static WORD okop6811(short mod)
{
	register WORD c;

	if ((c = info6811->opcodes[mod]) == 0)
		moderr();
	return c;
}

// binary search for opcode mnemonic
static struct entry6811 *find6811(char *searchfor)
{
	struct entry6811 *i;
	register short lo, hi, x, nfind;

	lo = 0;
	hi = dim(ops6811)-1;
	do {
		i=ops6811 + (x = (lo+hi) >> 1);
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

static void b6811(short mod, boolean brnc)
{
	WORD vmsk, vadr;

	scanptr = arg;
	getword(scancom(),&vadr);
	test_byte(&vadr);
	if (*scanptr == '#')
		scanptr++;
	getword(scancom(),&vmsk);
	test_byte(&vmsk);
	if (brnc)
	{
		ADDRESS vbr;
		short vrel;

		getlbl(scancom(),&vbr);
		vrel = make_rel(vbr,4);
		gen4db(okop6811(mod), vadr, vmsk, vrel);
	}
	else
		gen3dsb(okop6811(mod), vadr, vmsk);
}

// call proper code generator
void prs6811(void)
{
	ADDRESS dat;
	enum mod6811 mod;

	if (*op == 0)
	{
		prblank();
		return;
	}
	info6811 = find6811(op);
	if (info6811 == NULL)
	{
		if (MACACT)
			MACACT = FALSE;
		else
			opcerr();
		return;
	}
	if (!*arg)
		mod = INH;
	else if (streq(arg,"A"))
		mod = ACA;
	else if (streq(arg,"B"))
		mod = ACB;
	else if (streq(arg,"X"))
		mod = XRG;
	else if (streq(arg,"Y"))
		mod = YRG;
	else if (streq(argend-2,",X"))
	{
		*(argend-2) = 0;
		mod = IDX;
		if (*arg=='#')
			arg++;
	}
	else if (streq(argend-2,",Y"))
	{
		*(argend-2) = 0;
		mod = IDY;
		if (*arg=='#')
			arg++;
	}
	else if (*arg=='#')
	{
		arg++;
		mod = IMM; // or IM2
	}
	else
	{
		if (info6811->opcodes[REL])		// relative?
			mod = REL;
		else
			mod = EXT; // or DIR
	}
	if (streq(op,"BSET") || streq(op,"BCLR"))
	{
		if (mod == EXT)
			mod = DIR;
		b6811(mod,FALSE);
		return;
	}
	else if (streq(op,"BRSET") || streq(op,"BRCLR"))
	{
		if (mod == EXT)
			mod = DIR;
		b6811(mod,TRUE);
		return;
	}
	if (mod==REL || mod==IMM || mod==EXT || mod==IDX || mod==IDY)
	{
		getlbl(arg,&dat);
		if (mod == IMM)
		{
			if (info6811->opcodes[IM2])
				mod = IM2;
		}
		else if (mod == EXT && dat < 0x0100L)
		{
			if (info6811->opcodes[DIR])
				mod = DIR;
		}
	}
	switch (mod)
	{
		case INH:
			gen1db(okop6811(INH));
			break;
		case REL:
			genrb(okop6811(REL),dat);
			break;
		case ACA:
			gen1db(okop6811(ACA));
			break;
		case ACB:
			gen1db(okop6811(ACB));
			break;
		case XRG:
			gen1db(okop6811(XRG));
			break;
		case YRG:
			gen1db(okop6811(YRG));
			break;
		case IMM:
			gen2db(okop6811(IMM),(WORD)dat);
			break;
		case DIR:
			gen2db(okop6811(DIR),(WORD)dat);
			break;
		case EXT:
			gen3db(okop6811(EXT),(WORD)dat);
			break;
		case IDX:
			gen2db(okop6811(IDX),(WORD)dat);
			break;
		case IDY:
			gen2db(okop6811(IDY),(WORD)dat);
			break;
		case IM2:
			gen3db(okop6811(IM2),(WORD)dat);
			break;
	}
}

