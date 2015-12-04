// xasmerr.c - error routines for xasm.c

#include "xasm.h"
#include <conio.h>
#include <errno.h>
#include <dos.h>

//***********************
// processing procedures
//***********************

static void errl(void)
{
	char buf[256];

	strcpy(buf,orig);
	if (strlen(buf) > 40)
		buf[40] = '\0';
	if (INCLFLAG || APPEFLAG)
	{
		if (ERRMSG)
			fprintf(stdout,"\n%8s:%04d  ",iname,iline);
		else
			fprintf(errfile,"\n%8s:%04d  %s\n\t\t\t",iname,iline,buf);
	}
	else
	{
		if (ERRMSG)
			fprintf(stdout,"\n%8s:%04d  ",fname,fline);
		else
			fprintf(errfile,"\n%8s:%04d  %s\n\t\t\t",fname,fline,buf);
	}
}

static void errex(void)
{
	if (!ERRMSG)
	{
		fprintf(errfile,"\n");
		fclose(errfile);
	}
	exit(1);
}

static void errnoise(void)
{
	if (!ERRMSG)
	{
		sound(1400);
		delay(20);
		nosound();
	}
}

static void errpause(void)
{
	if (ERRPAUS && !ERRMSG)
	{
		printf("\n[ hit a key to continue ]");
		getch();
	}
}

static void warst(char *xs)
{
	if (ERRON)
	{
		errl();
		if (ERRMSG)
			fprintf(stdout,"*warn* %s ***",xs);
		else
			fprintf(errfile,"WARNING: %s ***",xs);
		errnoise();
		errpause();
	}
}

static void errst(char *xs)
{
	if (ERRON)
	{
		errl();
		if (ERRMSG)
			fprintf(stdout,"*erro* %s ***",xs);
		else
			fprintf(errfile,"%s ***",xs);
		errnoise();
		errpause();
	}
	errcnt++;
}

static void errvl(char *xs, WORD val, WORD mn, WORD mx, boolean xc)
{
	if (ERRON)
	{
		errl();
		if (mn || mx)
		{
			if (ERRMSG)
				fprintf(stdout,"*erro* %s (%04X)(%04X-%04X) ***", xs,val,mn,mx);
			else
				fprintf(errfile,"%s (%04X)(%04X-%04X) ***",xs,val,mn,mx);
		}
		else if (xc)
		{
			if (ERRMSG)
				fprintf(stdout,"*erro* %s (%c) ***",xs,val);
			else
				fprintf(errfile,"%s (%c) ***",xs,val);
		}
		else
		{
			if (ERRMSG)
				fprintf(stdout,"*erro* %s (%04X) ***",xs,val);
			else
				fprintf(errfile,"%s (%04X) ***",xs,val);
		}
		errnoise();
		errpause();
	}
	errcnt++;
}

static void errms(char *xs)
{
	errl();
	if (ERRMSG)
		fprintf(stdout,"*fatl* %s ***",xs);
	else
		fprintf(errfile,"%s ***\n",xs);
	errnoise();
	errex();
}

static void errfl(char *xs, char *xt)
{
	errl();
	if (ERRMSG)
		fprintf(stdout,"*fatl* %s - %s (code %d) ***",xt,xs,errno);
	else
		fprintf(errfile,"\t%s - %s (code %d) ***\n",xt,xs,errno);
	errnoise();
	errex();
}

//****************
// error messages
//****************

void opcerr(void)
{
	errst("invalid opcode");
}

void argerr(void)
{
	errst("invalid opcode argument");
}

void moderr(void)
{
	errst("bad address mode");
}

void relerr(WORD val)
{
	if (!serr)
		if (apass==2)
			errvl("rel branch out of range",val,minrel,maxrel,FALSE);
}

void regerr(short val)
{
	errvl("invalid register",val,0,0,TRUE);
}

void pagerr(WORD val)
{
	if (apass==2)
		errvl("jump out of page",val,0,0,FALSE);
}

void oorerr(WORD val, WORD mn, WORD mx)
{
	if (apass==2)
		errvl("symbol out of range",val,mn,mx,FALSE);
}

void aorerr(void)
{
	errst("address beyond CPU range");
}

void nolerr(void)
{
	errst("label required");
}

void miserr(void)
{
	errst("missing argument");
}

void dsmerr(void)
{
	errst("duplicate symbol");
}

void fnderr(char *s)
{
	if (ERRON)
	{
		errl();
		if (ERRMSG)
			fprintf(stdout,"*erro* <%s> - symbol not found ***",s);
		else
			fprintf(errfile,"\n\t\t<%s> - %s ***",s,"symbol not found");
		errnoise();
		errpause();
	}
	errcnt++;
	serr = TRUE;
}

void direrr(void)
{
	errst("illegal directive");
}

void orgerr(void)
{
	errst("reverse .ORG");
}

void norerr(void)
{
	errms("no .ORG directive");
}

void nocerr(void)
{
	errms("no .CPU directive");
}

void cpuerr(void)
{
	errms("unsupported .CPU specified");
}

void cimerr(void)
{
	errms(".CPU specified not implemented");
}

void cduerr(void)
{
	warst("only first .CPU statement valid");
}

void macerr(void)
{
	errms("macro table overflow");
}

void ioverr(void)
{
	errms("include nest overflow");
}

void opnerr(char *s)
{
	errfl("could not open",s);
}

void incerr(char *s)
{
	errfl("could not include",s);
}


