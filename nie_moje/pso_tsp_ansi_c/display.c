#include "stdinc.h"

/*------------------------------------------------------------------- DISPLAY_DISTANCES */
void display_distances(struct swarm sw)
{
/* Display all distances between particles of a swarm
*/
int i,j;
float d;

printf("\n\n Distances in the swarm");
for (i=0;i<sw.size;i++)
	{
	printf("\n");
	for (j=0;j<sw.size;j++)
		{
		d=distance(sw.p[i],sw.p[j],type_dist);
		printf("%.0f ",d);
		}
	}
printf("\n");
}


/*------------------------------------------------------------------- DISPLAY_ITER */
void display_iter(int iter,float eval_f,int swarm_size, int GN)
{
float x;
	printf("\n    Number of individual evaluations: %f",eval_f);
	x=swarm_size*(iter+1)*GN;
	printf("  = %f*%i(Swarm_size)*%i(Iter)*%i(graph_size)",eval_f/x,swarm_size,iter+1,GN);
	
}




/*------------------------------------------------------------------- DISPLAY_SOLUTION */
void display_solution(struct particle p)
{

if (p.rank<0)
	{
	printf("\n Extra particle");
	}

display_position(p,1);

}

/*------------------------------------------------------------------- DISPLAY_SWARM */
void display_swarm(struct swarm sw)
{
int i,i_best;

i_best=0;
printf("\n SWARM");
for (i=0;i<sw.size;i++) /* For each "particle"  */ 
	{
	display_position(sw.p[i],1);
	if (sw.p[i].f<sw.p[i_best].f) i_best=i;
	
	}
printf("\n Best particle: %i, f value %.1f",i_best+1,sw.p[i_best].f);
printf("\n");


}

/*------------------------------------------------------------------- DISPLAY_VELOCITY */
void display_velocity(struct velocity v)
{
int  i;

printf("\n  velocity (size=%i) :",v.size);

for (i=0;i<v.size;i++)
	{
	printf(" %i<=>%i ",v.comp[i][0]+1,v.comp[i][1]+1);
	}
}

