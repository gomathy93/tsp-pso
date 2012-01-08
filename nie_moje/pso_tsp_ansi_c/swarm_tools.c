#include "stdinc.h"

/*------------------------------------------------------------------- INIT_PARTICLE */
struct particle init_particle(struct graph G)
{
int i;
struct particle init={0};

init.rank=-1;

init.x.size=G.N;

//if (G.N-2*(G.N/2)>0)
 {
    for (i=0;i<G.N;i++)  // Define an "init" particle, for future use
	init.x.s[i]=i;
   }
//else for (i=0;i<G.N/2;i++)  { init.x.s[i]=2*i  ;     init.x.s[i+G.N/2]=2*i+1;  }

init.x.s[G.N]=0;     // Note that the _real_ size is G.N+1		
init.v.size=0;
init.f=f(init,-1,-1);
init.best_x=init.x;
init.best_f=init.f;
init.best_time=time;
//display_position(init,0);

return init;

}

/*------------------------------------------------------------------- INIT_SWARM */
struct swarm	init_swarm(int size,struct graph G,int trace)
{
/* Initialization of the swarm */
double               f2;
int 				i;
int                     i_min,i_max;
int                     level;
int					norm_v;
struct   seq      s1;
int                     size_init;
struct swarm 		swt;
float				x;// For conversion

	printf("\n Begin of swarm initialization.");
	if (size>10) printf(" Please wait");

swt.size=size;
init=init_particle(G); // Useful for future random initialization
swt.rank_best=0;

switch (init_option)
{
   case 0: // Random initialization
     i_min=0;i_max=size;
   case1_0:
for (i=i_min;i<i_max;i++)/* For each particle ...
						Note: it may arrive, particularly for small graphs
						that two particles are identical */
	{
      if (trace>1) printf("\n init particle %i",i+1);
	swt.p[i]=init;
	swt.p[i].rank=i;
   
      // Trying to find a "good" repartition

		x=G.N-2;x=i*x/(size-1)+0.5;

      norm_v=x;
 
	swt.p[i].v=alea_velocity(norm_v);    // random velocity
	if (norm_v>0) swt.p[i]=pos_plus_vel(swt.p[i],swt.p[i].v,0,0,0); //  => random position
   	swt.p[i].best_x=swt.p[i].x; // Best previous position = current position
	swt.p[i].best_f=swt.p[i].f;
      swt.p[i].best_time=time; // Memorize the current time step
 
	if (swt.p[i].f<swt.p[swt.rank_best].f) // best_best
		swt.rank_best=i;
	}
    break;

    case 1:  // Deterministic initialization, level 1
   size_init=(int)((float)size/2); // Just for, at max, half of the swarm. Other particles at random
   size_init=MIN(G.N,size_init); 
      for (i=0;i<size_init;i++)  // Define a tour beginning on i, using systematically
                                    // the smallest arc still free
      {
        if (trace>1) printf("\n init particle %i",i+1);
         swt.p[i].rank=i; // rank

        	swt.p[i].x=min_tour(G,i);    // Tour, built from i, with a simple greedy algorithm
         if (i!=0) swt.p[i].x=rotate(swt.p[i].x,0);    // Be sure it begins on 0
         swt.p[i].f=f(swt.p[i],-1,-1);   // Evaluate
         //swt.p[i].fl = swt.p[i].f;
         norm_v=alea(0,G.N);
         swt.p[i].v=alea_velocity(norm_v);    // random velocity
         swt.p[i].best_x=swt.p[i].x; // Best previous position = current position
          swt.p[i].best_f=swt.p[i].f;
         swt.p[i].best_time=time; // Memorize the current time step

         	if (swt.p[i].f<swt.p[swt.rank_best].f) // best_best
             swt.rank_best=i;       
      }

      if (size_init==size) break;
          // Complete by random initialization
         i_min=size_init;
         i_max=size;
         goto case1_0;

 case 2:  // Deterministic initialization, level 2
 printf("\n init level 2");
 level=1;
 case2:
   size_init=(int)((float)size/2); // Just for, at max, half of the swarm. Other particles at random
   size_init=MIN(G.N,size_init);
      for (i=0;i<size_init;i++)  // Define a tour beginning on i, using systematically
                                    // the smallest 2-arcs path still free
      {
         swt.p[i].rank=i; // rank

          // Tour, built from i, with a simple greedy algorithm
         s1=min_tour(G,i);
          swt.p[i].x=s1;
         if (i!=0) swt.p[i].x=rotate(swt.p[i].x,0);    // Be sure it begins on 0
         swt.p[i].f=f(swt.p[i],-1,-1);

         swt.p[i].x=min_tour_2(G,i,level);
         if (i!=0) swt.p[i].x=rotate(swt.p[i].x,0);    // Be sure it begins on 0
         f2=f(swt.p[i],-1,-1);

         if (f2>swt.p[i].f) swt.p[i].x=s1; else swt.p[i].f=f2;

         //swt.p[i].fl = swt.p[i].f;
         norm_v=alea(0,G.N);
         swt.p[i].v=alea_velocity(norm_v);    // random velocity
         swt.p[i].best_x=swt.p[i].x; // Best previous position = current position
          swt.p[i].best_f=swt.p[i].f;
         swt.p[i].best_time=time; // Memorize the current time step

         if (swt.p[i].f<swt.p[swt.rank_best].f) // best_best
             swt.rank_best=i;
      }

      if (size_init==size) break;
          // Complete by random initialization
         i_min=size_init;
         i_max=size;
         goto case1_0;

 case 3:
 printf("\n init level 2a");
 level=2;
 goto case2;
      
 }  
best_best=swt.p[swt.rank_best];

if (trace>1) 
	display_swarm(swt);
 printf("\n End of swarm initialization ");

    
return swt;
}

/*------------------------------------------------------------------- SAVE_SWARM */
void save_swarm(FILE *file,struct swarm sw)
{
int i,j;
int GN;

GN=sw.p[0].x.size;

fprintf(file,"%i %i\n",GN,sw.size); /* Save graph size and swarm size */
for (i=0;i<sw.size;i++) /* For each "particle" (which is a sequence of nodes) */ 
	{
	for (j=0;j<GN+1;j++)
		{
		fprintf(file,"%i ",sw.p[i].x.s[j]+1); /* Save the position ... */
		}
	fprintf(file," %f\n",sw.p[i].f); /* ... and the f value */
	}
}

//==========================================  TOT_VEL
double tot_vel(struct swarm sw)
// Compute the total velocity of the swarm
{
 int        i;
 double v;

 v=0;
 for (i=0;i<sw.size;i++) v=v+sw.p[i].v.size;
return v;
}
