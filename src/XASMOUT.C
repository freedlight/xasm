// xasmout.c - output routines for xasm.c

#include "xasm.h"
#include <dir.h>

// open output file
void outopen(char *name)
{
	char filename[NAMELEN],fmod[3];

	strcpy(filename,name);
	rmext(filename);
	if (OBJFLAG)
	{
		defext(filename,".obj");
		strcpy(fmod,"wb");
	}
	else
	{
		defext(filename,".hex");
		strcpy(fmod,"wt");
	}
	if ((outfile = fopen(filename,fmod)) == NULL)
		opnerr(filename);
	FIRSTOUT = TRUE;
}

// output a list line, with no object output
void prblank(void)
{
	prdata(UNUSED,UNUSED,UNUSED);
}

// output main part of line to list file
static void lststat(void)
{
	char *p;

	if (LSTON > 0)
	{
		if (MACACT)
		{
			arg = mptr->mtxt;
			getmparm(arg);
			fprintf(lstfile,"\t>>%s\n",arg);
		}
		else
		{
			if ((p = strchr(orig,'\n')) != 0)
				*p = 0;
			if ((p = strchr(orig,'\r')) != 0)
				*p = 0;
			if (strlen(orig) > 45)
				orig[45] = 0;
			fprintf(lstfile,"\t%s\n",orig);
		}
	}
}

// output initial part of line to list file
static void lstline(ADDRESS n)
{
	if (LSTON > 0)
	{
		if (INCLFLAG)
			fprintf(lstfile,"I%04d",iline);
		else if (APPEFLAG)
			fprintf(lstfile,"A%04d",iline);
		else if (MACACT)
			fprintf(lstfile," >>  ");
		else
			fprintf(lstfile," %04d",fline);
		fprintf(lstfile," %06X ",n);
	}
}

// output an object byte
static void prbyte(short n)
{
	char s[3];
	short val;

	val = n;
	if (n == UNUSED)
	{
		s[0] = ' ';
		s[1] = ' ';
		s[2] = 0;
	}
	else
	{
		outbyte(val);
		sprintf(s,"%02X",val&0xFF);
	}
	if (LSTON > 0)
	{
		if (n == UNUSED)
			fprintf(lstfile,"   ");
		else
			fprintf(lstfile,"%s ",s);
	}
}

// output up to 3 object bytes, with list line
void prdata(WORD i, WORD j, WORD k)
{
	if (apass == 1)
		return;
	lstline(pc);
	prbyte(i);
	prbyte(j);
	prbyte(k);
	lststat();
}

// output equate line to list file
void prequ(short i)
{
	if (apass == 1)
		return;
	if (LSTON > 0)
	{
		if (INCLFLAG)
			fprintf(lstfile,"I%04d",iline);
		else if (APPEFLAG)
			fprintf(lstfile,"A%04d",iline);
		else
			fprintf(lstfile," %04d",fline);
		fprintf(lstfile,"    %04X ",i);
	}
	prbyte(UNUSED);
	prbyte(UNUSED);
	prbyte(UNUSED);
	lststat();
}

// output checksum to object file
static void outchks(void)
{
	if (OBJFLAG)
		return;
	chks = 256-(chks&0xFF);
	fprintf(outfile,"%02X\n",chks&0xFF);
}

// finish last line of object file
void outend(void)
{
	if (apass == 1)
		return;
	if (OBJFLAG)
		return;
	while (outcnt<=20)
	{
		fprintf(outfile,"00");
		outcnt++;
	}
	outchks();
//	fprintf(outfile,":00%04X01\n\n",oline);
	fprintf(outfile,":00000001FF\n\n");
}

// start an object file data line
static void outinit(void)
{
	outcnt = 1;
	chks = 0;
	FIRSTOUT = FALSE;
	if (OBJFLAG)
		return;
	fprintf(outfile,":");
	outbyte(0x10);
	outbyte(oline/256);
	outbyte(oline);
	oline+=0x10;
	outbyte(0);
}

// check for and handle end of line for object file
void outpage(void)
{
	if (outcnt > 20)
	{
		outchks();
		outinit();
	}
	outcnt++;
}

// output a byte to object file
void outbyte(short dat)
{
	if (FIRSTOUT)
		outinit();
	if (FIRSTORG)
		norerr();
	else
	{
		dat &= 0xFF;
		outpage();
		if (OBJFLAG)
			putc(dat,outfile);
		else
			fprintf(outfile,"%02X",dat);
		chks += dat;
	}
}

// output page number for multi-page object files
void outhpag(short pg)
{
	if (OBJFLAG)
		return;
	fprintf(outfile,":02000002%d000%02X\n",pg,
		((252 + (pg * 16)) & 0xFF));
}

// output help when xasm only is typed
void help(void)
{
	puts("usage : xasm [-dep] asmfile");
	puts("\tdefault input suffix  = .asm");
	puts("\tdefault output suffix = .hex");
	puts("\t-d = show debugs");
	puts("\t-e = error output to LIST.ERR");
	puts("\t-m = error output for Turbo environ");
	puts("\t-p = pause after each error");
	printf("version %s %s\n\n",VERSION,__DATE__);
	exit(1);
}

// open listing file
void lstopen(char *name)
{
	char filename[NAMELEN];

	if (apass == 1)
		return;
	strcpy(filename,name);
	defext(filename,".lst");
	if ((lstfile = fopen(filename,"wt")) == NULL)
		opnerr(filename);
	LSTF = TRUE;
}

// open error file
void erropen(char *name)
{
	char filename[NAMELEN];

	strcpy(filename,name);
	defext(filename,".err");
	if ((errfile = fopen(filename,"wt")) == NULL)
		opnerr(filename);
	ERRF = TRUE;
}

// open symbol file
void symopen(char *name)
{
	char filename[NAMELEN];

	strcpy(filename,name);
	defext(filename,".sym");
	if ((symfile = fopen(filename,"wt")) == NULL)
		opnerr(filename);
	SYMF = TRUE;
}

static void prsym(char *sym, void *sp)
{
	if (*sym != '.')
		fprintf(symfile,"%-19s\t%04X\n",sym,((STAB *)sp)->syma);
}

void symlist(void)
{
	if (!SYMON)
		return;
	fprintf(symfile,"\n\nSYMBOL TABLE - %d items\n\n",symcnt);
	hash_print(htab,prsym);
	fprintf(symfile,"\n");
}

