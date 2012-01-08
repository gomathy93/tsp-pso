#include "stdinc.h"

/*------------------------------------------------------------------- DISPLAY_POSITION */
void display_position(struct particle p,int type)
{
 int 	i;
 float 	x;
 
 printf("\n particle %i: ",p.rank+1);
 
 for (i=0;i<p.x.size;i++)
	printf(" %i",q[p.x.s[i]]);
 printf("  %f",p.f);
 
}
