// XASM.C - generic cross assembler

#define __MAIN
#include "xasm.h"
#include <conio.h>

static void initmain(void)
{
	symcnt = maccnt = errcnt = 0;
	ipc = 0L;
	freespc = fillc = 0;
	REVFLAG = OBJFLAG = FALSE;
	cpuval = Xnone;
	htab = maketab(HTABSIZ);
	if (!ERRF)
		errfile = stderr;
}

static void initpass(void)
{
	ENDFLAG = FALSE;
	LSTON = 0;
	LSTF = FALSE;
	if (ERRMSG)
		lstfile = fopen("LST.OUT","wt");
	else
		lstfile = stdout;
	SYMON = FALSE;
	SYMF = FALSE;
	ERRON = TRUE;
	if (!ERRF)
		errfile = stderr;
	IFFON = TRUE;
	MACACT = MACON = FALSE;
	FIRSTORG = TRUE;
	pc = 0L;
	autol = fline = iline = oline = 0;
	APPEFLAG = INCLFLAG = FALSE;
	if (!ERRMSG)
		printf("\r%s : pass %d",fname,apass);
}

void cpupars(void)
{
	FILE *f;
	char *p, fsp[64];
	scpu x;

	if (apass > 1)
		return;
	if (cpuval != Xnone)	// cannot have 2 CPU statements
		cduerr();
	if ((p = getenv("XASM")) == NULL)
		strcpy(fsp, ".");
	else
		strcpy(fsp,p);
	strcat(fsp,"/xasm.cpu");
	if ((f = fopen(fsp,"rb")) == NULL)
	{
		fprintf(stderr, "Can't open file %s\n", fsp);
		exit(3);
	}
	while (fread(&x,sizeof(scpu),1,f) != NULL)
	{
		if (streq(x.name,cpu))
			break;
	}
	fclose(f);
	if (!x.active)
		cimerr(); // FATAL
	cpuval = x.sclass;
	cpusub = x.subclass;
	maxmem = x.maxmem;
	minrel = x.minrel;
	maxrel = x.maxrel;
	REVFLAG = x.byterev;
}

void genpars(void)
{
	switch (cpuval)
	{
// implemented
		case Xnone	: nocerr(); 	break;
		case X6502	: prs6502();	break;
		case X6801	: prs6801();	break;
		case X6805	: prs6805();	break;
		case X6811	: prs6811();	break;
		case X8048	: prs8048();	break;
		case X8051	: prs8051();	break;
		case X8080	: prs8080();	break;
// un-implemented
		case X1800	: // prs1800();	break;
		case X32000	: // prs32000();	break;
		case X34000	: // prs34000();	break;
		case X3800	: // prs3800();	break;
		case X56000	: // prs56000();	break;
		case X62800	: // prs62800();	break;
		case X6800	: // prs6800();	break;
		case X6804	: // prs6804();	break;
		case X6809	: // prs6809();	break;
		case X68000	: // prs68000();	break;
		case X7500	: // prs7500();	break;
		case X7700	: // prs7700();	break;
		case X7800	: // prs7800();	break;
		case X8086	: // prs8086();	break;
		case X8096	: // prs8096();	break;
		case XCLIP	: // prsCLIP();	break;
		case XCOP400: // prsCOP400();	break;
		case XNCR32	: // prsNCR32();	break;
		case XNOVA	: // prsNOVA();	break;
		case XPDP8	: // prsPDP8();	break;
		case XPDP11	: // prsPDP11();	break;
		case XZ8	: // prsZ8();	break;
		case XZ80	: // prsZ80();	break;
		case XZ8000	: // prsZ8000();	break;
		default		: cimerr();		break;
	}
}

void main(short argc, char *argv[])
{
	char sel,*cp;

	textmode(C80);
	if (argc < 2)
		help();
	DEBUGF = ERRPAUS = ERRMSG = ERRF = FALSE;
	++argv;
	--argc;
	while (*argv[0] == '-')
	{
		cp = *argv;
		while ((sel = tolower(*++cp)) != 0)
		{
			if (sel == 'd')
			{
				DEBUGF = TRUE;
				ERRPAUS = TRUE;
			}
			else if (sel == 'e')
				erropen("list");
			else if (sel == 'p')
				ERRPAUS = TRUE;
			else if (sel == 'm')
				ERRMSG = TRUE;
			else
				fprintf(stderr, "Invalid option - %s; ignored\n", *argv);
		}
		++argv;
		--argc;
	}
	initmain();
	strcpy(fname,*argv);
	for (apass = 1; apass < 3; apass++)
	{
		inpopen(fname);
		if (apass == 2)
			outopen(fname);
		initpass();
		if (DEBUGF)
			clrscr();
		while (getstmt()) 		// process a statement
		{
			serr = FALSE;
			if (DEBUGF)
			{
				gotoxy(1,2);
				printf("f:%04d i:%04d free=%08ld\n",
					fline, iline, farcoreleft());
				gotoxy(1,5);
			}
			if (UFLAG)			// not instruction
				prblank();
			else if (DFLAG)		// directive
				procdirect();
			else
			{
				if (IFFON)
				{
					if (pc > maxmem)	// memory violation ?
					{
						aorerr();
						apass = 2;
						break;
					}
					else
					{
						genlbl(pc,lbl,FALSE);
						if (MACON)
							putmac();
						else
							genpars(); 	// do processor parser
					}
				}
				else
					prblank();
			}
		}
		fclose(inpfile);
		fclose(incfile);
		if (LSTON>0 && (apass > 1))
			putc('\n',lstfile);
		if (LSTF)
			fclose(lstfile);
		if (apass > 1)
		{
			if (apass == 2)
				outend();
			fclose(outfile);
		}
		if (DEBUGF)
		{
			printf("AFTER pass %d\n",apass);
			printf("%d err, $%06lX start, $%06lX len, %ld free\n",
				errcnt,ipc,pc-ipc,freespc);
			symlist();
		}
		else
			if (errcnt > 0)
				break;
	}
	if ((errcnt == 0) || (apass == 2))
		symlist();
	if (SYMF)
		fclose(symfile);
	if (ERRF)
		fclose(errfile);
	if (!ERRMSG)
	{
		if (errcnt)
			printf("\n");
		printf("\r%s : %d err, $%06lX start, $%06lX len, %ld free\n",
			fname,errcnt,ipc,pc-ipc,freespc);
	}
	if (errcnt == 0)
		exit(0);
	else
		exit(1);
}

void defext(char *s, char *ext)
{
	if (strchr(s, '.') == NULL)
		strcat(s, ext);
}

void rmext(char *s)
{
	char *p;

	if ((p = strchr(s, '.')) != NULL)
		*p = 0;
}
