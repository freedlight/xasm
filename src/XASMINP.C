// xasminp.c - input routines for xasm.c

#include "xasm.h"
#include <dir.h>

// open input file
void inpopen(char *name)
{
	char filename[NAMELEN];

	strcpy(filename,name);
	defext(filename,".asm");
	if ((inpfile = fopen(filename,"rt")) == NULL)
		opnerr(filename);
#if BIGBUF
	setvbuf(inpfile,NULL,_IOFBF,BSIZE);
#endif
}

void appopen(char *name)
{
	char filename[NAMELEN];

	fclose(inpfile);
	strcpy(filename,name);
	defext(filename,".asm");
	if ((inpfile = fopen(filename,"rt")) == NULL)
		incerr(filename);
#if BIGBUF
	setvbuf(inpfile,NULL,_IOFBF,BSIZE);
#endif
	rmext(filename);
	strcpy(iname,filename);
	APPEFLAG = TRUE;
}

void inclpush(char *name)
{
	char filename[NAMELEN];

	if (INCLFLAG)
		ioverr();
	strcpy(filename,name);
	defext(filename,".asm");
	if ((incfile = fopen(filename,"rt")) == NULL)
		incerr(filename);
#if BIGBUF
	setvbuf(incfile,NULL,_IOFBF,BSIZE);
#endif
	rmext(filename);
	strcpy(iname,filename);
	iline = 0;
	INCLFLAG = TRUE;
}

static void inclpop(void)
{
	if (!INCLFLAG)
		return;
	fclose(incfile);
	INCLFLAG = FALSE;
}

// read and parse a line from input
boolean getstmt(void)
{
	char *i,*j;

	if (ENDFLAG)
	{
		if (INCLFLAG)
			inclpop();
		else
			return(FALSE);
	}
	DFLAG = UFLAG = FALSE;
	if (INCLFLAG || APPEFLAG)
		iline++;
	else
		fline++;
	if (INCLFLAG)
		j = fgets(orig,WIDTH+1,incfile);
	else
		j = fgets(orig,WIDTH+1,inpfile);
	if ((j > 0) && (orig[0] != '*') && (orig[0] != ';'))
	{
		if ((i = strchr(orig,'\r')) != NULL)
			*i = 0;
		if ((i = strchr(orig,'\n')) != NULL)
			*i = 0;
		strcpy(stmt,orig);
		if ((i = strchr(stmt,';')) != NULL)
			*i = 0;
		i = stmt + strlen(stmt) - 1;
	// remove trailing blanks
		while (isspace(*i) && (i > stmt))
			*i-- = 0;
	}
	else
		strcpy(stmt,"");
	if (j == NULL)
	{
		if (INCLFLAG)
			inclpop();
		else
			return(FALSE);
	}
	if (strlen(stmt) < 2)
	{
		UFLAG = TRUE;
		return(TRUE);
	}
	LFLAG = isalpha(stmt[0]);		// flag label
	if (stmt[0] == '.')
		LFLAG=TRUE;
	scanptr = stmt;			// find label and opcode
	lbl = scan();
	op = scan();
	arg = scanarg();
	DFLAG = (*op == '.');	// flag directive
	return(TRUE);
}

// get next token, uppercase
char *scan(void)
{
	register char *p;

	p = scanptr;		// start of token
	while (*scanptr && (!isspace(*scanptr)))
		scanptr++;
	while (*scanptr && isspace(*scanptr))
		*scanptr++ = 0;
	return(p);
}

// get arg token (special)
char *scanarg(void)
{
	register char *p;

	p = scanptr;		// start of token
	while (*scanptr && (!isspace(*scanptr)))
		scanptr++;
	argend = scanptr;
//	while (*scanptr && isspace(*scanptr))
//		*scanptr++ = 0;
	return(p);
}

// get next token, comma delimited
char *scancom(void)
{
	register char *p;

	p = scanptr;		// start of token
	while (*scanptr && (*scanptr!=',') && (!isspace(*scanptr)))
		scanptr++;
	while (*scanptr && ( *scanptr == ',' || isspace(*scanptr) ))
		*scanptr++ = 0;
	return(p);
}

