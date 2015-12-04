// XGEN6502.C - 6502 source parser for XASM.C

#include "xasm.h"

// addressing modes
enum mod6502 {
	ACC,		// accumulator
	IMM,		// immediate
	ZER,		// zero page
	ZEX,		// zero page, indexed X
	ZEY,		// zero page, indexed Y
	ABS,		// absolute
	ABX,		// absolute, indexed X
	ABY,		// absolute, indexed Y
	IMP,		// implied
	REL,		// relative
	IDX,		// indirect X - (nn,X)
	IDY,		// indirect Y - (nn),Y
	IND			// indirect   - (nnnn)
};

struct entry6502 {
	char *opname;
	char opcodes[13];
	char flag;
		/*	subfamily flag
			0 = normal 6502
			1 = normal and 65C02 (new address modes)
			2 = 65C00 and 65C02
			3 = 65C02 only
			4 = 65C00 only
		*/
};

static struct entry6502 ops6502[] = {
//			 ACC  IMM  ZER  ZEX  ZEY  ABS  ABX  ABY  IMP  REL  IDX  IDY  IND    subfam
{ "ADC",{ 0,   0x69,0x65,0x75,0,   0x6D,0x7D,0x79,0,   0,   0x61,0x71,0x72}, 1	},
{ "AND",{ 0,   0x29,0x25,0x35,0,   0x2D,0x3D,0x39,0,   0,   0x21,0x31,0x32}, 1	},
{ "ASL",{ 0x0A,0,   0x06,0x16,0,   0x0E,0x1E,0,   0,   0,   0,   0,   0   }, 0	},
{ "ASR",{ 0x4A,0,   0x46,0x56,0,   0x4E,0x5E,0,   0,   0,   0,   0,   0   }, 0	},
{ "BBR",{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0   }, 0x0F},
{ "BBS",{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0   }, 0x8F},
{ "BCC",{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0x90,0,   0,   0   }, 0	},
{ "BCS",{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0xB0,0,   0,   0   }, 0	},
{ "BEQ",{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0xF0,0,   0,   0   }, 0	},
{ "BGE",{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0xB0,0,   0,   0   }, 0	},
{ "BIT",{ 0,   0x89,0x24,0x34,0,   0x2C,0x3C,0,   0,   0,   0,   0,   0   }, 1	},
{ "BLT",{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0x90,0,   0,   0   }, 0	},
{ "BMI",{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0x30,0,   0,   0   }, 0	},
{ "BNE",{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0xD0,0,   0,   0   }, 0	},
{ "BPL",{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0x10,0,   0,   0   }, 0	},
{ "BRA",{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0x80,0,   0,   0   }, 2	},
{ "BRK",{ 0,   0,   0,   0,   0,   0,   0,   0,   0x00,0,   0,   0,   0   }, 0	},
{ "BVC",{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0x50,0,   0,   0   }, 0	},
{ "BVS",{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0x70,0,   0,   0   }, 0	},
{ "CLC",{ 0,   0,   0,   0,   0,   0,   0,   0,   0x18,0,   0,   0,   0   }, 0	},
{ "CLD",{ 0,   0,   0,   0,   0,   0,   0,   0,   0xD8,0,   0,   0,   0   }, 0	},
{ "CLI",{ 0,   0,   0,   0,   0,   0,   0,   0,   0x58,0,   0,   0,   0   }, 0	},
{ "CLR",{ 0,   0,   0x64,0x74,0,   0x9C,0x9E,0,   0,   0,   0,   0,   0   }, 3	},
{ "CLV",{ 0,   0,   0,   0,   0,   0,   0,   0,   0xB8,0,   0,   0,   0   }, 0	},
{ "CMP",{ 0,   0xC9,0xC5,0xD5,0,   0xCD,0xDD,0xD9,0,   0,   0xC1,0xD1,0xD2}, 1	},
{ "CPX",{ 0,   0xE0,0xE4,0,   0,   0xEC,0,   0,   0,   0,   0,   0,   0   }, 0	},
{ "CPY",{ 0,   0xC0,0xC4,0,   0,   0xCC,0,   0,   0,   0,   0,   0,   0   }, 0	},
{ "DEC",{ 0x3A,0,   0xC6,0xD6,0,   0xCE,0xDE,0,   0,   0,   0,   0,   0   }, 1	},
{ "DEX",{ 0,   0,   0,   0,   0,   0,   0,   0,   0xCA,0,   0,   0,   0   }, 0	},
{ "DEY",{ 0,   0,   0,   0,   0,   0,   0,   0,   0x88,0,   0,   0,   0   }, 0	},
{ "EOR",{ 0,   0x49,0x45,0x55,0,   0x4D,0x5D,0x59,0,   0,   0x41,0x51,0x52}, 1	},
{ "INC",{ 0x1A,0,   0xE6,0xF6,0,   0xEE,0xFE,0,   0,   0,   0,   0,   0   }, 1	},
{ "INX",{ 0,   0,   0,   0,   0,   0,   0,   0,   0xE8,0,   0,   0,   0   }, 0	},
{ "INY",{ 0,   0,   0,   0,   0,   0,   0,   0,   0xC8,0,   0,   0,   0   }, 0	},
{ "JMP",{ 0,   0,   0,   0,   0,   0x4C,0,   0,   0,   0,   0x7C,0,   0x6C}, 1	},
{ "JSR",{ 0,   0,   0,   0,   0,   0x20,0,   0,   0,   0,   0,   0,   0   }, 0	},
{ "LDA",{ 0,   0xA9,0xA5,0xB5,0,   0xAD,0xBD,0xB9,0,   0,   0xA1,0xB1,0xB2}, 1	},
{ "LDX",{ 0,   0xA2,0xA6,0,   0xB6,0xAE,0,   0xBE,0,   0,   0,   0,   0   }, 0	},
{ "LDY",{ 0,   0xA0,0xA4,0xB4,0,   0xAC,0xBC,0,   0,   0,   0,   0,   0   }, 0	},
{ "LSL",{ 0x0A,0,   0x06,0x16,0,   0x0E,0x1E,0,   0,   0,   0,   0,   0   }, 0	},
{ "LSR",{ 0x4A,0,   0x46,0x56,0,   0x4E,0x5E,0,   0,   0,   0,   0,   0   }, 0	},
{ "MUL",{ 0,   0,   0,   0,   0,   0,   0,   0,   0x02,0,   0,   0,   0   }, 4	},
{ "NOP",{ 0,   0,   0,   0,   0,   0,   0,   0,   0xEA,0,   0,   0,   0   }, 0	},
{ "ORA",{ 0,   0x09,0x05,0x15,0,   0x0D,0x1D,0x19,0,   0,   0x01,0x11,0x12}, 1	},
{ "PHA",{ 0,   0,   0,   0,   0,   0,   0,   0,   0x48,0,   0,   0,   0   }, 0	},
{ "PHP",{ 0,   0,   0,   0,   0,   0,   0,   0,   0x08,0,   0,   0,   0   }, 0	},
{ "PHX",{ 0,   0,   0,   0,   0,   0,   0,   0,   0xDA,0,   0,   0,   0   }, 2	},
{ "PHY",{ 0,   0,   0,   0,   0,   0,   0,   0,   0x5A,0,   0,   0,   0   }, 2	},
{ "PLA",{ 0,   0,   0,   0,   0,   0,   0,   0,   0x68,0,   0,   0,   0   }, 0	},
{ "PLP",{ 0,   0,   0,   0,   0,   0,   0,   0,   0x28,0,   0,   0,   0   }, 0	},
{ "PLX",{ 0,   0,   0,   0,   0,   0,   0,   0,   0xFA,0,   0,   0,   0   }, 2	},
{ "PLY",{ 0,   0,   0,   0,   0,   0,   0,   0,   0x7A,0,   0,   0,   0   }, 2	},
{ "RMB",{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0   }, 0x07},
{ "ROL",{ 0x2A,0,   0x26,0x36,0,   0x2E,0x3E,0,   0,   0,   0,   0,   0   }, 0	},
{ "ROR",{ 0x6A,0,   0x66,0x76,0,   0x6E,0x7E,0,   0,   0,   0,   0,   0   }, 0	},
{ "RTI",{ 0,   0,   0,   0,   0,   0,   0,   0,   0x40,0,   0,   0,   0   }, 0	},
{ "RTS",{ 0,   0,   0,   0,   0,   0,   0,   0,   0x60,0,   0,   0,   0   }, 0	},
{ "SBC",{ 0,   0xE9,0xE5,0xF5,0,   0xED,0xFD,0xF9,0,   0,   0xE1,0xF1,0xF2}, 1	},
{ "SEC",{ 0,   0,   0,   0,   0,   0,   0,   0,   0x38,0,   0,   0,   0   }, 0	},
{ "SED",{ 0,   0,   0,   0,   0,   0,   0,   0,   0xF8,0,   0,   0,   0   }, 0	},
{ "SEI",{ 0,   0,   0,   0,   0,   0,   0,   0,   0x78,0,   0,   0,   0   }, 0	},
{ "SMB",{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0   }, 0x87},
{ "STA",{ 0,   0,   0x85,0x95,0,   0x8D,0x9D,0x99,0,   0,   0x81,0x91,0x92}, 1	},
{ "STX",{ 0,   0,   0x86,0,   0x96,0x8E,0,   0,   0,   0,   0,   0,   0   }, 0	},
{ "STY",{ 0,   0,   0x84,0x94,0,   0x8C,0,   0,   0,   0,   0,   0,   0   }, 0	},
{ "STZ",{ 0,   0,   0x64,0x74,0,   0x9C,0x9E,0,   0,   0,   0,   0,   0   }, 3	},
{ "TAX",{ 0,   0,   0,   0,   0,   0,   0,   0,   0xAA,0,   0,   0,   0   }, 0	},
{ "TAY",{ 0,   0,   0,   0,   0,   0,   0,   0,   0xA8,0,   0,   0,   0   }, 0	},
{ "TRB",{ 0,   0,   0x14,0,   0,   0x1C,0,   0,   0,   0,   0,   0,   0   }, 3	},
{ "TSB",{ 0,   0,   0x04,0,   0,   0x0C,0,   0,   0,   0,   0,   0,   0   }, 3	},
{ "TSX",{ 0,   0,   0,   0,   0,   0,   0,   0,   0xBA,0,   0,   0,   0   }, 0	},
{ "TXA",{ 0,   0,   0,   0,   0,   0,   0,   0,   0x8A,0,   0,   0,   0   }, 0	},
{ "TXS",{ 0,   0,   0,   0,   0,   0,   0,   0,   0x9A,0,   0,   0,   0   }, 0	},
{ "TYA",{ 0,   0,   0,   0,   0,   0,   0,   0,   0x98,0,   0,   0,   0   }, 0	},
};

static struct entry6502 *info6502;

static short mode_err(void)
{
	moderr();
	return 0;
}

// test for valid opcode
static short okop6502(short mode)
{
	register short c;

	if (info6502->flag == 2) // valid for C00, C02, C151
	{
		if ((cpusub != X_65C02) && (cpusub != X_65C151) && (cpusub != X_65C00))
		{
			opcerr();
			return 0;
		}
	}
	if (info6502->flag == 3) // valid for C02, C151
	{
		if ((cpusub != X_65C02) && (cpusub != X_65C151))
		{
			opcerr();
			return 0;
		}
	}
	if (info6502->flag == 4) // valid for C00
	{
		if (cpusub != X_65C00)
		{
			opcerr();
			return 0;
		}
	}
	if (info6502->flag == 1 && cpusub == X_6502) // valid for 6501
	{
		if (streq(op,"BIT"))
		{
			if (mode == IMM || mode == ABX || mode == ZEX)
				return mode_err();
		}
		if (mode == ACC)
		{
			if (streq(op,"DEC") || streq(op,"INC"))
				return mode_err();
		}
		if ((mode == ABX) && streq(op,"JMP"))
			return mode_err();
		if ((mode == IND) && !streq(op,"JMP"))
			return mode_err();
	}
	if ((c = info6502->opcodes[mode]) == 0)
		if (!streq(info6502->opname,"BRK"))
			moderr();
	return c & 0xFF;
}

// binary search for opcode mnemonic
static struct entry6502 *find6502(char *searchfor)
{
	struct entry6502 *i;
	register short lo, hi, x, nfind;

	lo = 0;
	hi = dim(ops6502)-1;
	do {
		i = ops6502 + (x = (lo + hi) >> 1);
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
			return NULL;
		prblank();
		MACACT = TRUE;
		getmac(x);
		i = NULL;
	}
	return i;
}

// bit process object generator
static void genb6502(WORD opc)
{
	WORD vbyt;
	short vbit, vopc, vrel;

	opc &= 0xFF;
	vbit = op[3] - '0';
	test_oor(&vbit,0,7);
	vopc = opc + (vbit * 0x10);
	scanptr = arg;
	getword(scancom(),&vbyt);
	test_byte(&vbyt);
// BBSx or BBRx
	if (opc == 0x8F || opc == 0x0F)
	{
		ADDRESS vadr;

		getlbl(scancom(),&vadr);
		vrel = make_rel(vadr, 3);
		prdata(vopc,vbyt,vrel);
		pc += 3L;
	}
	else
// SMBx or RMBx
	{
		prdata(vopc,(short)vbyt,UNUSED);
		pc += 2L;
	}
}

// call proper code generator
void prs6502(void)
{
	ADDRESS dat;
	enum mod6502 mod;
	char tmp[6];

	if (!*op)
	{
		prblank();
		return;
	}
// special handler for RMBx, SMBx BBRx, BBSx
	if ((strlen(op) == 4) &&
		(cpusub==X_6501 || cpusub==X_6511 || cpusub==X_65C00))
	{
		strcpy(tmp,op);
		tmp[3] = 0;
		if ((info6502 = find6502(tmp)) != 0)
			if (abs(info6502->flag) > 2)
			{
				genb6502(info6502->flag);
				return;
			}
	}
	if ((info6502 = find6502(op)) == NULL)
	{
		if (MACACT)
			MACACT = FALSE;
		else
			opcerr();
		return;
	}
	if (!*arg)
		mod = IMP;
	else if (*arg == '#')
		mod = IMM;
	else if (streq(arg,"A"))
		mod = ACC;
	else if ((*arg == '(') & (streq(argend-3,",X)")))
	{
		arg++;
		*(argend-3) =  0;
		mod = IDX;
	}
	else if ((*arg == '(') & (streq(argend-3,"),Y")))
	{
		arg++;
		*(argend-3) = 0;
		mod = IDY;
	}
	else if (streq(argend-2,",X"))
	{
		*(argend-2) = 0;
		mod = ABX;
	}
	else if (streq(argend-2,",Y"))
	{
		*(argend-2) = 0;
		mod = ABY;
	}
	else if ((*arg == '(') & (*(argend-1) == ')'))
	{
		arg++;
		*(argend-1) = 0;
		mod = IND;
	}
	else
	{
		if (info6502->opcodes[REL])
			mod = REL;
		else
			mod = ABS;
	}
	if ((mod != ACC) && (mod != IMM) && (mod != IMP))
	{
		getlbl(arg,&dat);
		if (mod == ABS && dat < 0x0100L)
		{
			if (info6502->opcodes[ZER])
				mod = ZER;
		}
		else if (mod == ABX && dat < 0x0100L)
		{
			if (info6502->opcodes[ZEX])
				mod = ZEX;
		}
		else if (mod == ABY && dat < 0x0100L)
		{
			if (info6502->opcodes[ZEY])
				mod = ZEY;
		}
	}
	switch (mod)
	{
		case ACC:
			gen1b(okop6502(ACC));
			break;
		case IMM:
			gendb(okop6502(IMM));
			break;
		case ZER:
			gen2b(okop6502(ZER),(WORD)dat);
			break;
		case ZEX:
			gen2b(okop6502(ZEX),(WORD)dat);
			break;
		case ZEY:
			gen2b(okop6502(ZEY),(WORD)dat);
			break;
		case ABS:
			gen3br(okop6502(ABS),(WORD)dat);
			break;
		case ABX:
			gen3br(okop6502(ABX),(WORD)dat);
			break;
		case ABY:
			gen3br(okop6502(ABY),(WORD)dat);
			break;
		case IMP:
			gen1b(okop6502(IMP));
			break;
		case REL:
			genrb(okop6502(REL),dat);
			break;
		case IDX:
			if (streq(op,"JMP"))
				gen3br(okop6502(IDX),(WORD)dat);
			else
				gen2b(okop6502(IDX),(WORD)dat);
			break;
		case IDY:
			gen2b(okop6502(IDY),(short)dat);
			break;
		case IND:
			if (streq(op,"JMP"))
				gen3br(okop6502(IND),(WORD)dat);
			else
				gen2b(okop6502(IND),(short)dat);
			break;
	}
}

