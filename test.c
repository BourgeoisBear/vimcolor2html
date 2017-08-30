/*
    Copyright 2004-2008 Daniele Depetrini
    Author: 	Daniele Depetrini (depetrini@libero.it)
    Version: 1.5.3
    Date:  06/07/08

    This file is part of CIDRMerge.
*/

#include "cidrmerge.h"

static char *version="1.5.3";
static char *author="Daniele Depetrini (depetrini@libero.it)";

void print_address(STREAM file,unsigned int net, unsigned char pref);

/* static array to speedup prefix print */
static char *dotted[]={
								"0","1","2","3","4","5","6","7","8","9",
							   "10","11","12","13","14","15","16","17","18","19",
								"20","21","22","23","24","25","26","27","28","29",
								};

#ifdef INLINE
__inline
#endif
static unsigned char collect_octet(char *line,int current_position,int *end_position)
{
		unsigned int number=0;

		*end_position=current_position;
		while (line[current_position] >='0' && line[current_position] <='9')
		{
			number*=10;
			number+=line[current_position]-48;
			current_position++;
		}

		/*update return pointer only if it's a valid octect*/
		if (number<256)
		{
			*end_position=current_position;
		}

		return number;
}

/*
	Parse a text string in CIDR format
*/
#ifdef INLINE
__inline
#endif
static int parse_line(char* line,struct entry *res)
{
	int i,n;
	int end=-1,stop=0;

	i=0;
	n=24;
	res->network=0;
	res->prefix=INVALID_PREFIX;

	while ((n>=0) && !stop)
	{
		#ifdef INPUT_DEBUG
		printf("Before collect i:%d end:%d line[i]:%c\n",i,end,line[i]);
		#endif

		res->network|=collect_octet(line,i,&end)<<n;

		#ifdef INPUT_DEBUG
		printf("After collect i:%d end:%d line[end]:%c\n",i,end,line[end]);
		#endif

		if ((end==i) && (end>0))
		{
			if (line[end-1]=='.')
			{
				n+=8;
			}
			else
			{
				break;
			}
		}
		switch (line[end])
		{
			case '.':
				if (n>0)
				{
					i=end+1;
				}
				else
				{
					stop=1;
				}
				break;
			case '/':
				if (n==0)
				{
					i=end+1;
					#ifdef INPUT_DEBUG
					printf("Before collect prefix i:%d end:%d line[i]:%c\n",i,end,line[i]);
					#endif
					res->prefix=collect_octet(line,i,&end);
					if ((line[end] != '\0') && (line[end] != ' '))
					{
						res->prefix=INVALID_PREFIX;
					}
					#ifdef INPUT_DEBUG
					printf("After collect prefix i:%d end:%d prefix:%d\n",i,end,res->prefix);
					#endif
				}

				stop=1;
				break;
			case ' ':
				/*no break here*/
				fprintf(stderr,"WARNING: not considering characters after space in line %s",line);
			case '*':
				if ((line[end]=='*') && (line[end+1] != '\0'))
				{
					res->prefix=INVALID_PREFIX;
					n=32;
				}
			case '\0':
/*				line[end]='\n';
				line[end+1]='\0';
			case '\n':
*/				if (n<24)
				{
					res->prefix=32-n;
				}
				stop=1;
				break;
			default:
				stop=1;
				break;
		}
		n-=8;
	}
	#ifdef INPUT_DEBUG
	printf("Final prefix: %d\n",res->prefix);
	#endif
	if (res->prefix>32)
	{
		return 0;
	}

	/* final sanity check, very important: library functions will not work if this costraint is not enforced */
	if ((res->network&TONETMASK(res->prefix))!=res->network)
	{
		return 0;
	}

	return 1;
}

#ifdef INLINE
__inline
#endif
static int STRCPY(char *dst,char *src)
{
	int i=0;

	for (;src[i]!='\0';i++)
		dst[i]=src[i];

	return i;
}

#ifdef OUTPUT_VERBOSITY
static unsigned int counter=0;
#endif
void print_address(STREAM file,unsigned int net, unsigned char pref)
{
	char buf[MAXCIDR+2];
	int pos=0,len;
#ifdef OUTPUT_VERBOSITY
	fprintf(stdout,"N: %u\tVNet: %u\tMask: %d.%d.%d.%d\t",counter++,net,TONETMASK(pref)>>24,(TONETMASK(pref)&MASK2)>>16,(TONETMASK(pref)&MASK3)>>8,TONETMASK(pref)&MASK4);

/*	fprintf(file,"N: %u\tMask: %d.%d.%d.%d\t ",counter++,TONETMASK(pref)>>24,(TONETMASK(pref)&MASK2)>>16,(TONETMASK(pref)&MASK3)>>8,TONETMASK(pref)&MASK4);
*/
#else
	if (pref!=INVALID_PREFIX)
	{
#endif
		pos+=STRCPY(buf+pos,dotted[net>>24]);
		buf[pos++]='.';
		pos+=STRCPY(buf+pos,dotted[(net&MASK2)>>16]);
		buf[pos++]='.';
		pos+=STRCPY(buf+pos,dotted[(net&MASK3)>>8]);
		buf[pos++]='.';
		pos+=STRCPY(buf+pos,dotted[net&MASK4]);
		buf[pos++]='/';
		pos+=STRCPY(buf+pos,dotted[pref]);
		buf[pos++]='\n';

		len=WRITE(file,buf,pos);
		if (len<pos)
		{
			fprintf(stderr,"Written only %d bytes instead of %d, aborting\n",len,pos);
			exit(1);
		}


#ifndef OUTPUT_VERBOSITY
	}
#endif
}
