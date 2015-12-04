// cpubld.c - cpu type datafile builder for xasm.c

#include "xasm.h"

// list should be in alphabetical order
scpu cpus[] = {
//                                                       byt xa di
// name     type    subtype   memorymap    rel+ rel- bit rev sm sm
//--------- ------- --------  ------------ ---- ---- ---- -- -- ---
{ "146805",	X6805,	X_146805, 0x00001FFFL, 126, -129,  8, 0, 1, 1 },
{ "1802",	X1800,	X_1802,	  0x0000FFFFL, 127, -128,  8, 0, 0, 0 },
{ "1805",	X1800,	X_1805,	  0x0000FFFFL, 127, -128,  8, 0, 0, 0 },
{ "32008",	X32000,	X_32008,  0x00FFFFFFL, 127, -128, 32, 0, 0, 0 },
{ "32016",	X32000,	X_32016,  0x00FFFFFFL, 127, -128, 32, 0, 0, 0 },
{ "32032",	X32000,	X_32032,  0x00FFFFFFL, 127, -128, 32, 0, 0, 0 },
{ "32532",	X32000,	X_32532,  0x00FFFFFFL, 127, -128, 32, 0, 0, 0 },
{ "34010",	X34000,	X_34010,  0x0000FFFFL, 127, -128, 32, 0, 0, 0 },
{ "34020",	X34000,	X_34020,  0x0000FFFFL, 127, -128, 32, 0, 0, 0 },
{ "3870",	X3800,	X_3870,	  0x00000FFFL, 127, -128,  8, 0, 0, 0 },
{ "56000",	X56000,	X_56000,  0x0000FFFFL, 127, -128, 32, 0, 0, 0 },
{ "62801",	X62800,	X_62801,  0x0000FFFFL, 127, -128,  8, 0, 0, 0 },
{ "6301",	X6801,	X_6301,	  0x0000FFFFL, 126, -129,  8, 0, 0, 0 },
{ "6303",	X6801,	X_6303,	  0x0000FFFFL, 126, -129,  8, 0, 1, 1 },
{ "64180",	XZ80,	X_64180,  0x0000FFFFL, 127, -128,  8, 0, 0, 0 },
{ "6501",	X6502,	X_6501,	  0x0000FFFFL, 127, -128,  8, 1, 1, 1 },
{ "6502",	X6502,	X_6502,	  0x0000FFFFL, 127, -128,  8, 1, 1, 1 },
{ "6511",	X6502,	X_6511,	  0x0000FFFFL, 127, -128,  8, 1, 1, 1 },
{ "65C00",	X6502,	X_65C00,  0x0000FFFFL, 127, -128,  8, 1, 1, 1 },
{ "65C02",	X6502,	X_65C02,  0x0000FFFFL, 127, -128,  8, 1, 1, 1 },
{ "65C151",	X6502,	X_65C151, 0x0000FFFFL, 127, -128,  8, 1, 1, 1 },
{ "6800",	X6800,	X_6800,	  0x0000FFFFL, 126, -129,  8, 0, 0, 0 },
{ "68000",	X68000,	X_68000,  0x00FFFFFFL, 127, -128, 32, 0, 0, 0 },
{ "68008",	X68000,	X_68008,  0x00FFFFFFL, 127, -128, 32, 0, 0, 0 },
{ "6801",	X6801,	X_6801,	  0x0000FFFFL, 126, -129,  8, 0, 1, 1 },
{ "68010",	X68000,	X_68010,  0x00FFFFFFL, 127, -128, 32, 0, 0, 0 },
{ "6802",	X6800,	X_6802,	  0x0000FFFFL, 126, -129,  8, 0, 0, 0 },
{ "68020",	X68000,	X_68020,  0x00FFFFFFL, 127, -128, 32, 0, 0, 0 },
{ "6803",	X6801,	X_6803,	  0x0000FFFFL, 126, -129,  8, 0, 1, 1 },
{ "68030",	X68000,	X_68030,  0x00FFFFFFL, 127, -128, 32, 0, 0, 0 },
{ "6808",	X6800,	X_6808,	  0x0000FFFFL, 126, -129,  8, 0, 0, 0 },
{ "6804",	X6804,	X_6804,	  0x0000FFFFL, 126, -129,  8, 0, 0, 0 },
{ "6805",	X6805,	X_6805,	  0x00001FFFL, 126, -129,  8, 0, 1, 1 },
{ "6809",	X6809,	X_6809,	  0x0000FFFFL, 126, -129,  8, 0, 0, 1 },
{ "68200",	X68000,	X_68200,  0x00FFFFFFL, 127, -128, 16, 0, 0, 0 },
{ "68HC11",	X6811,	X_68HC11, 0x0000FFFFL, 127, -128,  8, 0, 1, 1 },
{ "7500",	X7500,	X_7500,	  0x0000FFFFL, 127, -128,  8, 0, 0, 0 },
{ "7501",	X7500,	X_7501,	  0x0000FFFFL, 127, -128,  8, 0, 0, 0 },
{ "7502",	X7500,	X_7502,	  0x0000FFFFL, 127, -128,  8, 0, 0, 0 },
{ "7520",	X7500,	X_7520,	  0x0000FFFFL, 127, -128,  8, 0, 0, 0 },
{ "7700",	X7700,	X_7720,	  0x0000FFFFL, 127, -128,  8, 0, 0, 0 },
{ "7800",	X7800,	X_7800,	  0x0000FFFFL, 127, -128,  8, 0, 0, 0 },
{ "7801",	X7800,	X_7801,	  0x0000FFFFL, 127, -128,  8, 0, 0, 0 },
{ "78C05",	X7800,	X_78C05,  0x0000FFFFL, 127, -128,  8, 0, 0, 0 },
{ "7807",	X7800,	X_7807,	  0x0000FFFFL, 127, -128,  8, 0, 0, 0 },
{ "7810",	X7800,	X_7810,	  0x0000FFFFL, 127, -128,  8, 0, 0, 0 },
{ "80186",	X8086,	X_80186,  0x00FFFFFFL, 127, -128, 16, 0, 0, 0 },
{ "8021",	X8048,	X_8048,	  0x00000FFFL, 0,   0,     8, 0, 1, 0 },
{ "80286",	X8086,	X_80286,  0x00FFFFFFL, 127, -128, 16, 0, 0, 0 },
{ "8031",	X8051,	X_8031,	  0x0000FFFFL, 127, -128,  8, 0, 1, 1 },
{ "8035",	X8048,	X_8048,	  0x00000FFFL, 0,   0,     8, 0, 1, 0 },
{ "80386",	X8086,	X_80386,  0xFFFFFFFFL, 127, -128, 32, 0, 0, 0 },
{ "8039",	X8048,	X_8048,	  0x00000FFFL, 0,   0,     8, 0, 1, 0 },
{ "8040",	X8048,	X_8048,	  0x00000FFFL, 0,   0,     8, 0, 1, 0 },
{ "8044",	X8051,	X_8044,	  0x0000FFFFL, 127, -128,  8, 0, 1, 1 },
{ "8048",	X8048,	X_8048,	  0x00000FFFL, 0,   0,     8, 0, 1, 0 },
{ "8049",	X8048,	X_8048,	  0x00000FFFL, 0,   0,     8, 0, 1, 0 },
{ "8050",	X8048,	X_8048,	  0x00000FFFL, 0,   0,     8, 0, 1, 0 },
{ "8051",	X8051,	X_8051,	  0x0000FFFFL, 127, -128,  8, 0, 1, 1 },
{ "8080",	X8080,	X_8080,	  0x0000FFFFL, 0,   0, 	   8, 0, 1, 1 },
{ "8085",	X8080,	X_8085,	  0x0000FFFFL, 0,   0, 	   8, 0, 1, 1 },
{ "8086",	X8086,	X_8086,	  0x000FFFFFL, 127, -128, 16, 0, 0, 0 },
{ "8088",	X8086,	X_8088,	  0x000FFFFFL, 127, -128, 16, 0, 0, 0 },
{ "8096",	X8096,	X_8096,	  0x0000FFFFL, 127, -128, 16, 0, 0, 0 },
{ "80C152",	X8051,	X_8051,	  0x0000FFFFL, 127, -128,  8, 0, 1, 1 },
{ "80C186",	X8086,	X_80C186, 0x00FFFFFFL, 127, -128, 16, 0, 0, 0 },
{ "80C286",	X8086,	X_80C286, 0x00FFFFFFL, 127, -128, 16, 0, 0, 0 },
{ "80C31",	X8051,	X_8051,   0x0000FFFFL, 127, -128,  8, 0, 1, 1 },
{ "80C32",	X8051,	X_8051,	  0x0000FFFFL, 127, -128,  8, 0, 1, 1 },
{ "80C40",	X8048,	X_8048,	  0x00000FFFL, 0,   0,     8, 0, 1, 0 },
{ "80C50",	X8048,	X_8048,	  0x00000FFFL, 0,   0,     8, 0, 1, 0 },
{ "80C51",	X8051,	X_8051,	  0x0000FFFFL, 127, -128,  8, 0, 1, 1 },
{ "80C52",	X8051,	X_8051,	  0x0000FFFFL, 127, -128,  8, 0, 1, 1 },
{ "80C54",	X8051,	X_8051,	  0x0000FFFFL, 127, -128,  8, 0, 1, 1 },
{ "80C58",	X8051,	X_8051,	  0x0000FFFFL, 127, -128,  8, 0, 1, 1 },
{ "80C85",	X8080,	X_80C85,  0x0000FFFFL, 0,   0, 	   8, 0, 1, 1 },
{ "80C86",	X8086,	X_80C86,  0x000FFFFFL, 127, -128, 16, 0, 0, 0 },
{ "80C88",	X8086,	X_80C88,  0x000FFFFFL, 127, -128, 16, 0, 0, 0 },
{ "83C152",	X8051,	X_8051,	  0x0000FFFFL, 127, -128,  8, 0, 1, 1 },
{ "83C51",	X8051,	X_8051,	  0x0000FFFFL, 127, -128,  8, 0, 1, 1 },
{ "84C00",	XZ80,	X_84C00,  0x0000FFFFL, 127, -128,  8, 0, 0, 0 },
{ "84C01",	XZ80,	X_84C01,  0x0000FFFFL, 127, -128,  8, 0, 0, 0 },
{ "8748",	X8048,	X_8048,	  0x00000FFFL, 0,   0,     8, 0, 1, 0 },
{ "8749",	X8048,	X_8048,	  0x00000FFFL, 0,   0,     8, 0, 1, 0 },
{ "8751",	X8051,	X_8051,	  0x0000FFFFL, 127, -128,  8, 0, 1, 1 },
{ "8752",	X8051,	X_8051,	  0x0000FFFFL, 127, -128,  8, 0, 1, 1 },
{ "87C51",	X8051,	X_8051,	  0x0000FFFFL, 127, -128,  8, 0, 1, 1 },
{ "87C52",	X8051,	X_8051,	  0x0000FFFFL, 127, -128,  8, 0, 1, 1 },
{ "87C54",	X8051,	X_8051,	  0x0000FFFFL, 127, -128,  8, 0, 1, 1 },
{ "87C58",	X8051,	X_8051,	  0x0000FFFFL, 127, -128,  8, 0, 1, 1 },
{ "COP400",	XCOP400,X_COP400, 0x0000FFFFL, 127, -128,  8, 0, 0, 0 },
{ "COPS400",XCOP400,X_COPS400,0x0000FFFFL,127, -128,   8, 0, 0, 0 },
{ "F8",		X3800,	X_F8,	  0x00000FFFL, 127, -128,  8, 0, 0, 0 },
{ "H8300",	XH8,	X_H8_300, 0x0000FFFFL, 127, -128,  8, 0, 0, 1 },
{ "H83002",	XH8,	X_H8_300H,0x00FFFFFFL, 127, -128,  8, 0, 0, 1 },
{ "NCR32",	XNCR32,	X_NCR32,  0x0000FFFFL, 127, -128, 32, 0, 0, 0 },
{ "NEC780",	XZ80,	X_NEC780, 0x0000FFFFL, 127, -128,  8, 0, 0, 0 },
{ "NOVA4",	XNOVA,	X_NOVA,	  0x0000FFFFL, 127, -128, 16, 0, 0, 0 },
{ "NSC800",	XZ80,	X_NSC800, 0x0000FFFFL, 127, -128,  8, 0, 0, 0 },
{ "PDP11",	XPDP11,	X_PDP11,  0x0000FFFFL, 127, -128, 16, 0, 0, 0 },
{ "PDP8",	XPDP8,	X_PDP8,	  0x0000FFFFL, 127, -128,  8, 0, 0, 0 },
{ "RSC264",	XRSC,	X_RSC264, 0x0000FFFFL, 127, -128,  8, 0, 0, 1 },
{ "Z8",		XZ8,	X_Z8,	  0x0000FFFFL, 127, -128,  8, 0, 0, 0 },
{ "Z80",	XZ80,	X_Z80,	  0x0000FFFFL, 127, -128,  8, 0, 0, 1 },
{ "Z8001",	XZ8000,	X_Z8001,  0x00FFFFFFL, 127, -128, 16, 0, 0, 0 },
{ "Z8002",	XZ8000,	X_Z8002,  0x00FFFFFFL, 127, -128, 16, 0, 0, 0 }
};

void main(void)
{
	int i;
	FILE *f;

	f = fopen("xasm.cpu","wb");
	for (i=0; i < dim(cpus); i++)
		fwrite(&cpus[i],sizeof(scpu),1,f);
	fclose(f);
}
