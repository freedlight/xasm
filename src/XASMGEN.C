// XASMGEN.C - generic code generators for XASM.C

#include "xasm.h"

void test_rel(short *da)
{
	if (apass == 2)
	if (*da > maxrel || *da < minrel)
	{
		relerr(*da);
		*da = 0;
	}
}

void test_byte(WORD *da)
{
	if (*da > 0x100)
	{
		oorerr(*da, 0, 0x100);
		*da = 0xFF;
	}
}

void test_pag(WORD *da, WORD mask)
{
	short tst = *da - pc;

	if (apass != 2)
		return;
	if (tst > 0)
		tst &= (~mask);
	else
		tst = (-tst) & (~mask);
	if (tst != 0)
	{
		pagerr(tst);
		*da = 0;
	}
}

void test_oor(short *da, short lo, short hi)
{
	if (*da > hi || *da < lo)
	{
		oorerr(*da, lo, hi);
		*da = lo;
	}
}

// 1 byte object generator
void gen1b(WORD opc)
{
	prdata(opc, UNUSED, UNUSED);
	pc += 1L;
}

// 2 byte object generator
void gen2b(WORD opc, WORD da)
{
	test_byte(&da);
	prdata(opc, da, UNUSED);
	pc += 2L;
}

// 3 byte object generator
void gen3b(WORD opc, WORD da)
{
	WORD da1, da2;
	
	da1 = (da >> 8) & 0xFF;
	da2 = da & 0xFF;
	prdata(opc, da1, da2);
	pc += 3L;
}

// 3 byte object generator
void gen3sb(WORD opc, WORD da1, WORD da2)
{
	prdata(opc, da1, da2);
	pc += 3L;
}

// 4 byte object generator
void gen4b(WORD opc, WORD da1, WORD da2, WORD da3)
{
	prdata(opc, da1, da2);
	outbyte(da3);
	pc += 4L;
}

// 1 byte object generator (double byte opcode)
void gen1db(WORD opc)
{
	if (opc < 0x100)
	{
		prdata(opc, UNUSED, UNUSED);
		pc += 1L;
	}
	else
	{
		WORD opc1, opc2;

		opc1 = (opc >> 8) & 0xFF;
		opc2 = opc & 0xFF;
		prdata(opc1, opc2, UNUSED);
		pc += 2L;
	}
}

// 2 byte object generator (double byte opcode)
void gen2db(WORD opc, WORD da)
{
	test_byte(&da);
	if (opc < 0x100)
	{
		prdata(opc, da, UNUSED);
		pc += 2L;
	}
	else
	{
		WORD opc1, opc2;

		opc1 = (opc >> 8) & 0xFF;
		opc2 = opc & 0xFF;
		prdata(opc1, opc2, da);
		pc += 3L;
	}
}

// 3 byte object generator (double byte opcode)
void gen3db(WORD opc, WORD da)
{
	WORD da1, da2;
	
	da1 = (da >> 8) & 0xFF;
	da2 = da & 0xFF;
	if (opc < 0x100)
	{
		prdata(opc, da1, da2);
		pc += 3L;
	}
	else
	{
		WORD opc1, opc2;

		opc1 = (opc >> 8) & 0xFF;
		opc2 = opc & 0xFF;
		prdata(opc1, opc2, da1);
		outbyte(da2);
		pc += 4L;
	}
}

// 3 byte object generator (double byte opcode)
void gen3dsb(WORD opc, WORD da1, WORD da2)
{
	if (opc < 0x100)
	{
		prdata(opc, da1, da2);
		pc += 3L;
	}
	else
	{
		WORD opc1, opc2;

		opc1 = (opc >> 8) & 0xFF;
		opc2 = opc & 0xFF;
		prdata(opc1, opc2, da1);
		outbyte(da2);
		pc += 4L;
	}
}

// 4 byte object generator (double byte opcode)
void gen4db(WORD opc, WORD da1, WORD da2, WORD da3)
{
	if (opc < 0x100)
	{
		prdata(opc, da1, da2);
		outbyte(da3);
		pc += 4L;
	}
	else
	{
		WORD opc1, opc2;

		opc1 = (opc >> 8) & 0xFF;
		opc2 = opc & 0xFF;
		prdata(opc1, opc2, da1);
		outbyte(da2);
		outbyte(da3);
		pc += 5L;
	}
}

// 3 byte (reversed word) object generator
void gen3br(WORD opc, WORD da)
{
	WORD da1, da2;
	
	da1 = da / 256;
	da2 = da & 0xFF;
	prdata(opc, da2, da1);
	pc += 3L;
}

// constant byte data object generator
void gendb(WORD opc)
{
	WORD da;
	
	arg++;				// dump '#'
	getword(arg, &da);	// recalc data
	test_byte(&da);
	prdata(opc, da, UNUSED);
	pc += 2L;
}

// relative branch object generator
void genrb(WORD opc, ADDRESS da)
{
	short da1 = make_rel(da, 2);
	prdata(opc, da1, UNUSED);
	pc += 2L;
}

short make_rel(ADDRESS da, short off)
{
	short da1; // must be signed

	da1 = (short)(da - pc - (long) off);
	test_rel(&da1);
	return da1;
}

