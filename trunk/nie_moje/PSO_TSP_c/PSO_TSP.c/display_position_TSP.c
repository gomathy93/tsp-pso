/*------------------------------------------------------------------- DISPLAY_POSITION */
void display_position(struct particle p,int type)
{
int 	i;
float 	x;

printf("\n particle %i: ",p.rank+1);

for (i=0;i<p.x.size+1;i++)
	printf(" %i",p.x.s[i]+1);

if (trace>1)
	{
	printf("\n arc values: ");
	for (i=0;i<p.x.size;i++)
		{
		x=G.v[p.x.s[i]][p.x.s[i+1]];
		if (x<0) printf("? "); else printf("%.0f ",x);	
		}
	}

if (type>0 || trace>1)
	{
	 printf(" f value:%.1f",p.f);
	printf(" v.size: %i,",p.v.size);
	}

if (type>1 || trace > 2)
	{
	printf("\n best previous: ");
	for (i=0;i<p.best_x.size+1;i++)
		printf(" %i",p.best_x.s[i]+1);
	}	

if (trace>2)
	display_velocity(p.v);

}
