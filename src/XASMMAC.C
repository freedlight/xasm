// xasmmac.c - macro processing for xasm.c

#include "xasm.h"

// enter macro in macro table
void genmac(char *xs)
{
	char *p;

	if (apass > 1)
		return;
	if ((p = strchr(xs,' ')) != 0)
		*p = 0;
	if (findmac(xs) != -1)		// test for duplicate
		dsmerr();
	maccnt++;
	if ((mac[maccnt] = (MACR *)malloc(sizeof(MACR))) == NULL)
		macerr();
	if (maccnt > MAXMAC)
		macerr();
	strcpy(mac[maccnt]->mnam,xs);
	mac[maccnt]->mnxt = NULL;
}

// put macro in table
void putmac(void)
{
	MACR *ptr,*prev;

	prblank();
	if (apass > 1)
		return;
	prev = mac[maccnt];
	ptr = mac[maccnt]->mnxt;
	while (ptr)
	{
		prev = ptr;
		ptr = ptr->mnxt;
	}
	if ((ptr = (MACR *)malloc(sizeof(MACR))) == NULL)
		macerr();
	prev->mnxt = ptr;
	strcpy(ptr->mnam,mac[maccnt]->mnam);
	strcpy(ptr->mtxt,orig);
	ptr->mnxt = NULL;
}

// fetch macro from table
void getmac(short n)
{
	short i;
	char mbf[80],abf[40];

	strcpy(abf,arg);			// save arguments
	scanptr = abf;
	for (i=0; i<=9; i++)
		margs[i] = scancom();
	mptr = mac[n];
	while (mptr->mnxt != NULL)	// while lines left in macro
	{
		mptr = mptr->mnxt;
		strcpy(mbf,mptr->mtxt);	// copy line to destroyable buffer
		scanptr = mbf;			// parse input line
		LFLAG = (mbf[0] == '.');	// look for label (always automatic)
		lbl = scan();
		op = scan();
		arg = scanarg();
		getmparm(arg);
		genlbl(pc,lbl,FALSE);
		genpars();				// generate code
	}
	autol++;
}

void getmparm(char *s)
{
	short pn;
	char *pp,*ps,*pd;

	if ((pp = strchr(s,'@')) != NULL) // if parameter
	{
		ps = s;
		pd = argb;
		while (ps < pp)			// copy up to param
			*pd++ = *ps++;
		ps++;					// skip '@'
		pn = (*ps++ - '0');		// get param number
		pp = margs[pn];
		while (*pp != 0)			// copy param
			*pd++ = *pp++;
		while (*ps && !(isspace(*ps)))	// copy rest
			*pd++ = *ps++;
		*pd = 0;
		argend = pd;
		arg = argb;
	}
}

// lookup macro in table
// returns location number for parallel pointer table
// returns -1 if not found
short findmac(char *s)
{
	short i;

	if (maccnt == 0)
		return(-1);
	for (i=1; i<=maccnt; i++)
		if (streq(s,mac[i]->mnam))
			return(i);
	return(-1);
}

// list all macros to list file
// currently not used
/*
void maclist(void)
{
	short i;

	printf("\n*** MACRO LIST ***\n");
	for (i=1; i<=maccnt; i++)
	{
		printf("macro #%d - %s\n",i,mac[i]->mnam);
		mptr = mac[i];
		while (mptr->mnxt != NULL)		// while lines left in macro
		{
			mptr = mptr->mnxt;
			printf("%s\n",mptr->mtxt);
		}
	}
	putchar('\n');
}
*/

