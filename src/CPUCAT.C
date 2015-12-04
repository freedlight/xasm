// cpucat.c - cpu type catalog for xasm.c

#include "xasm.h"

void memval(unsigned long memsize)
{
	printf("maxmem=");
	if (memsize > 65536L)
		printf("%4dM\n",(memsize / 1048576L) + 1L);
	else
		printf("%4dK\n",(memsize / 1024L) + 1L);
}

void main(void)
{
	FILE *f;
	char *p,fsp[64];
	scpu x;

	if ((p = getenv("XASM")) == NULL)
		strcpy(fsp,".");
	else
		strcpy(fsp,p);
	strcat(fsp,"\\xasm.cpu");
	if ((f = fopen(fsp,"rb")) == NULL)
	{
		fprintf(stderr, "Can't open file %s\n", fsp);
		exit(3);
	}
	while (fread(&x,sizeof(scpu),1,f) != NULL)
	{
		putchar('\t');
		printf("%-10s ", x.name);
		if (x.active)
			putchar('X');
		else
			putchar(' ');
		if (x.dactive)
			putchar('D');
		else
			putchar(' ');
		printf(" %2d class=%2d-%02d ", x.bits, x.sclass, x.subclass);
		memval(x.maxmem);
	}
	fclose(f);
}

