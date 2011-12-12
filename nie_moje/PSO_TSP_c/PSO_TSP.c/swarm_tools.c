

/*------------------------------------------------------------------- INIT_PARTICLE */
struct particle init_particle(struct graph G)
{
int i;
struct particle init;

init.rank=-1;

init.x.size=G.N;
for (i=0;i<G.N;i++)  // Define an "init" particle, for future use
	init.x.s[i]=i;
init.x.s[G.N]=0;
		
init.v.size=0;
init.f=f(init,-1,-1);
init.best_x=init.x;
init.best_f=init.f;
return init;
}

/*------------------------------------------------------------------- INIT_SWARM */
struct swarm	init_swarm(int size,struct graph G,int trace)
{
/* Initialization of the swarm */
int 				i;
int					norm_v;
struct swarm 		swt;
float				x,x1; // For conversion

	printf("\n Begin of swarm initialization.");
	if (size>10) printf(" Please wait");

swt.size=size;


 /* Random initialization */


init=init_particle(G);
swt.rank_best=0;

for (i=0;i<size;i++)/* For each particle ...
						Note: it may arrive, particularly for small graphs
						that two particles are identical */
	{
	
	swt.p[i]=init;
	swt.p[i].rank=i;
	swt.p[i].best_x=swt.p[i].x; // Best previous position = current position
	swt.p[i].best_f=swt.p[i].f;
	//swt.p[i].v=alea_velocity(G.N-2); // Random velocity
	//swt.p[i].v=alea_velocity(alea(1,2*(G.N-2)));
	//swt.p[i].v=alea_velocity(2*G.N);
								
	//x=G.N-2;x=i*x/(size-1)+0.5;
							// Trying to find a "good" repartition
	if (size<=G.N-1)
		{
		x1=size; x1=1-x1/(G.N-1); x1=16*x1+1;
		x=(size-1)-i;x=x/(size-1);
		x=exp(x1*log(x)); x=(G.N-2)*(1-x)+0.5;
		norm_v=x+1;
		}
	else
		{
		x=G.N-2;x=i*x/(size-1)+0.5;
		norm_v=x+1;
		}
	//printf("\n x %f, norm_v %i",x,norm_v);
	swt.p[i].v=alea_velocity(norm_v);
	
	swt.p[i]=pos_plus_vel(swt.p[i],swt.p[i].v,0,0,0); //  => random position
	
	if (swt.p[i].f<swt.p[swt.rank_best].f) // best_best
		swt.rank_best=i;
	}

best_best=swt.p[swt.rank_best];
extra_best=best_best;
same_best_val=extra_best.f;
same_best=0;

if (trace>0) 
	display_swarm(swt);
 printf("\n End of swarm initialization ");

effective_move=1;
return swt;
}

/*------------------------------------------------------------------- NO_HOPE */
int	no_hope(struct swarm sw,int swarm_size, int adaptive,float nohope,int hood)
{
/*  0 = still hope,   1 = no hope   (to reach a solution) */

int		i;
float	nohope_thresh2;
float	sensibility=8;  // The smaller it is, the more often a Rehope is done. 8 seems correct
int		v_size_tot,v_tot,v_tot_min;


if (same_best>=same_best_thresh)
	{
	printf("\n NO HOPE: still the same best result");
	return 1;
	}

v_tot=0; // Number of non null velocities
v_size_tot=0;
for (i=0;i<sw.size;i++)
	if (sw.p[i].v.size>0) {v_tot=v_tot+1;v_size_tot=v_size_tot+sw.p[i].v.size;}
	
//if (effective_move==0)
if (v_tot==0)
	{
	printf("\n ABSOLUTELY NO HOPE. Swarm doesn't move");
	//i=trace; trace=3;
	display_distances(sw);
	//trace=i;
	return 2;
	}	
	

if (adaptive==0) 
	v_tot_min=hood;
else
	v_tot_min=(swarm_size-hood)*pow(threshold(best_best,val,G)-1,sensibility) + hood;
	


nohope_thresh2=MAX(3,swarm_size*nohope);	// At least 3 particles


if (sw.size<=nohope_thresh2 && nohope>0) // If too few different classes of particles => no hope 
	{
	printf("\n  NO HOPE: swarm too reduced");
	return 1; 
	}

if (v_tot<v_tot_min) // If the swarm almost doesn't move anymore ...
	{
	printf("\n  NO HOPE: the swarm almost doesn't move anymore");
	printf("\n  v_tot %i, v_tot_min %i",v_tot,v_tot_min);
	return 1; 
	}


return 0;
}

/*------------------------------------------------------------------- REDUCE_SWARM */
struct swarm reduce_swarm(struct swarm sw)
{
struct swarm	diff;
int				i,j;

if (trace>0)
	printf("\n Reduce swarm");

diff.size=1;  /* Initialize the "reduced" swarm with the first particle */
diff.p[0]=sw.p[0];


/* Count how many different  particles there are */
for (i=1;i<sw.size;i++) /* For each other particle ... */
	{
	
	/* ... check if it is in the reduced swarm ... */
	for (j=0;j<diff.size;j++)
		{
		if (sw.p[i].f==diff.p[j].f) 
			{
			if (compare_particle(sw.p[i],diff.p[j])==0) // It is already in the reduced swarm
				goto next_check;
			}
		}
	/*... and, if  not, add it */
	diff.p[diff.size]=sw.p[i];
	diff.p[diff.size].rank=diff.size;
	diff.size=diff.size+1;
	next_check:;
	}

if (trace>1)
	{
	printf("\n Reduced swarm");
	display_swarm(diff);
	}
	
return diff;
}

/*------------------------------------------------------------------- RE_HOPE */
struct swarm re_hope(struct swarm sw,int swarm_size,int trace,int auto_move_type,int monotony,int levelling)
/* Check if there is still hope to reach a solution. If probably not, move the swarm */
/* No-hope condition:  to few different particles */

{
struct particle	current_best;
int				i;
int				rank_best;
int				type;
struct swarm 	swt;


printf("\n\n RE-HOPE process for %i particles => %i particles",sw.size,swarm_size);

levelling=LIL;


if (rehope_method==0)// -------------- Ultra-simple method
	{
	current_best=extra_best;
	swt=init_swarm(swarm_size,G,trace);
	if (current_best.f<best_best.f)
		{
		swt.p[best_best.rank]=current_best;
		best_best=current_best;
		extra_best=current_best;
		}
	goto end_rehope;
	}

// --------------- Sophisticated method


if (auto_move_type<9) type=auto_move_type;
if (auto_move_type==9)  // Adaptive
	{
	if (same_best<2) 
		{type=0; goto rehope;}
	if (same_best==2)
		{type=1; goto rehope;}
	if (same_best<=3)
		{type=2; goto rehope;}
	if (same_best>3)
		{//type=3;
		type=2;
		}
	}

rehope:	
printf("\n  auto move type: %i",type);

for (i=0;i<swarm_size;i++)
	{
	if (i>=sw.size)  // For new particles ...
		{
		swt.p[i]=init;
		swt.p[i].rank=i;
		}
	else  // If the particle is in the reduced swarm ...
		{
	 	swt.p[i]=previous_best(sw.p[i],0);// .. back to  its best previous position
		}
	
	swt.p[i]=auto_move(swt.p[i],type,monotony,levelling,trace); /*... try to find around a better position ...*/	
	swt.p[i].v=alea_velocity(G.N-2); /*... change the velocity randomly */
	
	
	if (swt.p[i].f<swt.p[i].best_f) // Redefine the previous best. Useful only if "step"=0
		{
		swt.p[i].best_f=swt.p[i].f;
		swt.p[i].best_x=swt.p[i].x;
		}

	if (swt.p[i].x.s[0]!=0) swt.p[i].x=rotate(swt.p[i].x,0);// Eventually "rotate" to have node 1 first
	
	}
	
best_best=swt.p[0];
for (i=0;i<swarm_size;i++)
	{
	if (swt.p[i].f<best_best.f) // Check best of the best
			best_best=swt.p[i];
	}
	
swt.size=swarm_size;

if (trace>0)
	printf("\n   swarm re-expanded");



end_rehope:	//---------------------------


rank_best=best_best.rank;

//printf("\n rank_best %i, f: %f",rank_best,swt.p[rank_best].f);

if (rank_best<0) rank_best=0; // If the best of the best is a queen, we keep it at rank 0

if (extra_best.f<best_best.f) // Eventually replace best of the best by extra-best
	{
	extra_best.rank=rank_best;
	if (extra_best.x.s[0]!=0) extra_best.x=rotate(extra_best.x,0);
	swt.p[rank_best]=extra_best; // Keep at least the extra-best 
	best_best=extra_best;
	if (trace>0) 
		printf("\n extra_best => swarm, at rank %i",rank_best);
	}	

swt.rank_best=rank_best;
best_best.v.size=0;
swt.p[swt.rank_best].v.size=0; // Best of the best slow down	


if (trace>1) display_swarm(swt);
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

/*------------------------------------------------------------------- 	TRESHOLD */
float threshold(struct particle best,struct val val,struct graph G)
{ /* Modify the threshold to decide Rehope or not, according to
 the best result so far, and the probability of each arc value
This threshold will be compared to the global velocity (see rehope() )
*/

int		i;
int		lmax,lmin;
float	t,tmax,tmin;
int		v;

lmax=G.l_max; lmin=G.l_min;

tmin=G.l_min*G.N/(G.N-1);
tmax=(G.l_max+1)*G.N/(G.N-1);

// Estimate how "difficult" it is to improve the best particle

t=0;

for (i=0;i<G.N;i++)
	{
	v=G.v[best.x.s[i]][best.x.s[i+1]]-G.l_min; // rank for arc value
	if (v<0) v=G.l_max+1-G.l_min;
	t=t+ (v+G.l_min)*val.v[v];  // + arc_value*proba_for_this_value
	}

	t=(t-tmin)/(tmax-tmin);

printf("\n  Easiness coeff: %f",t);
return t;
}