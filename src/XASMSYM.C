// xasmsym.c - symbol processing for xasm.c

#include "xasm.h"
#include <conio.h>

// converts local label to full length symbol
static char *makeloc(char *s)
{
	if (s[1] == '.') // automatic label (..x)
	{
		if (MACON)
			strcpy(lbuf,s);
		else
			sprintf(lbuf,"L%04X.%c",autol,s[2]);
	}
	else
		sprintf(lbuf,"%s%s",lastlab,s);
	return(lbuf);
}


// enter label in symbol table
void genlbl(ADDRESS lblval, char *lblstr, boolean needslbl)
{
	STAB *sp;

	if (!LFLAG)
	{
		if (needslbl) // must have label
			nolerr();
		else
			return;
	}
// local label process
	if (lblstr[0] == '.')
	{
		if (apass == 1)
			lblstr = makeloc(lblstr);
	}
	else
		strcpy(lastlab,lblstr);
	if (apass > 1)
		return;
// test for duplicate
	if ((STAB *)findsym(htab,lblstr) != 0)
	{
		if (!streq(op,".SET"))
			dsmerr();
	}
	else
	{
		sp = (STAB *)addsym(htab,lblstr,sizeof(STAB));
		sp->syma = lblval;
		symcnt++;
	}
}

boolean getword(char *lblstr, WORD *lblval)
{
	ADDRESS x = (ADDRESS)*lblval;
	boolean rtn = getlbl(lblstr,&x);
	*lblval = (WORD)x;
	return rtn;
}

// fetch symbol value from table
boolean getlbl(char *lblstr, ADDRESS *lblval)
{
	char *p,cp;
	ADDRESS dat, extr;
	boolean LOFLAG,HIFLAG,SYFLAG;
	STAB	*sp;

	if (lblstr[0] == 0)
	{
		miserr();	// label missing
		*lblval = 0x0100L;
		return(FALSE);
	}
	LOFLAG = HIFLAG = SYFLAG = FALSE;
	cp = ' ';
	while (isspace(*lblstr))
		lblstr++;
	if (lblstr[0] == '>')
	{
		HIFLAG = TRUE;
		lblstr++;
	}
	else if (lblstr[0] == '<')
	{
		LOFLAG = TRUE;
		lblstr++;
	}
	else if (lblstr[0] == '%')
	{
		*lblval = pc;
		SYFLAG = TRUE;
	}
	else if (lblstr[0] == '.')
		lblstr = makeloc(lblstr);
	else if (lblstr[0] == 0x27)	// handle '
	{
		lblstr++;
		dat = (ADDRESS) *lblstr;
		*lblval = dat;
		SYFLAG = TRUE;
	}
	else if ((*lblstr=='$') || (isdigit(*lblstr)))
	{
		dat = makenum(lblstr);
		*lblval = dat;
		SYFLAG = TRUE;
	}
	p = lblstr;
	while (*p && (*p!='+') && (*p!='-') && (*p!='*') && (*p!='/'))
		p++;
	cp = ' ';
	extr = 0;
	if (*p && (*p > ' '))
	{
		cp = *p;
		if ((cp=='+') || (cp=='-') || (cp=='*') || (cp=='/'))
		{
			*p++=0;
			getlbl(p,&extr);
		}
		if (DEBUGF)
		{
			gotoxy(1,4);
			printf("cp:%c extr=%06X",cp,extr);
			gotoxy(1,5);
		}
	}
	if (!SYFLAG)
	{
		if (HIFLAG || LOFLAG)
			*lblval = 0xFFL;		// byte default
		else
			*lblval = 0x0100L;		// word default
		if (lblstr[0] == 0)
			*lblval = 0;
		else if ((sp = (STAB *)findsym(htab,lblstr)) != 0)
		{
			if (HIFLAG)
				*lblval = (ADDRESS) ((sp->syma / 256) & 0xFF);
			else if (LOFLAG)
				*lblval = (ADDRESS) (sp->syma & 0xFF);
			else
				*lblval = sp->syma;
		}
		if (DEBUGF)
		{
			gotoxy(1,3);
			printf("sym:%-12s (len=%2d) %06X",lblstr,strlen(lblstr),*lblval);
			gotoxy(1,5);
		}
	}
	if (apass == 2)
	{
		if (cp == '+')
			(*lblval) += extr;
		else if (cp == '-')
			(*lblval) -= extr;
		else if (cp == '*')
			(*lblval) *= extr;
		else if (cp == '/')
			(*lblval) /= extr;
		if (!sp && !SYFLAG)
			fnderr(lblstr);
	}
	if (SYFLAG)
		return(TRUE);
	else
		return(sp != 0);
}

ADDRESS makenum(register char *s)
{
	char *p;

// number format - $xxxx
	if (s[0] == '$')
	{
		s++;
		return atoh(s);
	}
// number format - xxxx:xxxx (8086)
	else if ((p = strchr(s,':')) != 0)
	{
		*p++ = 0;
		return((atoh(s) * 16L) + atoh(p));
	}
// number format - xxxxh (Microsoft)
	else if (toupper(s[strlen(s)-1]) == 'H')
		return atoh(s);
// number format - decimal
	else
		return atol(s);
}

