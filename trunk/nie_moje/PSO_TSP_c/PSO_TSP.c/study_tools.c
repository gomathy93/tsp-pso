/*------------------------------------------------------------------- 	ALL_CYCLES */
void all_cycles(struct graph G)
{
int	i,j;
struct particle p;

struct particle solution;

solution.f=(G.N+1)*G.l_max;
solution.rank=-1;

p.x.s[0]=0;

for (i=1;i<G.N;i++)
	{
	
	for (j=1;j<G.N-1;j++)
		{
		p.x.s[i]=j;
	
	
	
		p.f=f(p,-1,-1);
		if (p.f<solution.f)
			{
			solution=p;
			solution.rank=0;
			}
		}
	}

display_position(solution,1);
}



/*------------------------------------------------------------------- 	DETERM_SEQU */
struct swarm determ_sequ(int size,int length)
{
/* Deterministic list of sequences of integer numbers: "size" sequences of length "length"+1
all beginning and ending by 1. Except the last number, in each sequence all numbers are different.
*/
 
int				i,j,k,l;
struct swarm	ls,ls2;
int				ls_size;
struct particle	s;



/*
WARNING 1: recursive subroutine 
WARNING 2: NOT AT ALL efficient for  (length-1)! sequences are generated and only _after_ that, it keeps "size" of them.
			Can't work as soon as length is too big.
*/

/*printf("\n Size: %i, Length: %i",size,length);*/


s.x.s[0]=1;
s.x.s[length]=1;

if (length==2) /* In this case, there is just one possible sequence (size=1) */ 
	{
	s.x.s[1]=2;
	ls.size=1;
	ls.p[0]=s;
	return ls;
	}

l=fact(length-2);

/*printf("\n l = %i",l);*/

ls=determ_sequ(l,length-1); /* Generate (recursively) all possible sub-sequences... */


ls_size=0; /* ... and, from them, generate all sequences */

for (i=0;i<ls.size;i++) /* For each sub-sequence  ... */
	{
	s.x.s[1]=length;          /* ... insert a new node _before_ the sub-sequence ... */
	for (j=2;j<length;j++)
		s.x.s[j]=ls.p[i].x.s[j-1];
	
	ls2.p[ls_size]=s;  /*... and generate a new sequence */
	ls_size=ls_size+1;
	if(ls_size>=size) goto end;

	for (j=1;j<length-1;j++) /* ... insert a new node _after_ the sub-sequence */
		s.x.s[j]=ls.p[i].x.s[j];
	s.x.s[length-1]=length;	
	
	ls2.p[ls_size]=s; /*... and generate a new sequence */
	ls_size=ls_size+1;
	if(ls_size>=size) goto end;

	if (length>3)     /* ... insert  a new node _inside_ the sub-sequence */
		{
		for (j=1;j<=length-3;j++) /* For each possible position ... */
			{
			for (k=1;k<=j;k++) /* ... copy the first part of the sub-sequence... */
				s.x.s[k]=ls.p[i].x.s[k];
				
				s.x.s[j+1]=length;/* ...insert the node... */
				
			for (k=j+2;k<=length-1;k++) /* ...copy the last part of the sub-sequence... */
				s.x.s[k]=ls.p[i].x.s[k-1];
		
			ls2.p[ls_size]=s; /*... and generate a new sequence */	
			ls_size=ls_size+1;
			if(ls_size>=size) goto end; 
			}
			
		}
	
	}
end:
ls2.size=ls_size;
return ls2;


}



/*------------------------------------------------------------------- DISPLAY_TOPO */
void display_topo(struct swarm sw)
 /* - Look for the best particle
	- Display, for each particle, its cost function value and its distance to the best particle
	Note: uses the global variable swd (see sw_distance() )
	Note: Modify the global variable topo (just for visualization)
*/
{
float	f_max;
int		i;

i_best=0;
f_max=0;

for (i=1;i<sw.size;i++)
	if (sw.p[i].f<sw.p[i_best].f)
		i_best=i;
		
for (i=0;i<sw.size;i++)
	{
	if (trace>0)
		{
		printf("\n");
		display_position(sw.p[i],1);
		printf(" distance to the best: %f",swd.d[i_best][i]);
		}
	f_max=MAX(f_max,sw.p[i].f);
	}
for (i=0;i<sw.size;i++)
	{
	topo[i][0]=swd.d[i_best][i]; /* X-coordinate for the curve (on [0,1]) */
	topo[i][1]=sw.p[i].f/f_max; /* Y-coordinate for the curve (on [0,1]) */
	}	
	
	
}


/*------------------------------------------------------------------- LANDSCAPE */
void landscape(FILE *file,int swarm_size,struct particle center)
{
/* Plot a "section" of the landscape of the state space
between  particles of a swarm and a "center" particle
or between all particles of a swarm
*/
struct particle centert;
int				i,k;
struct particle pt;
struct swarm 	swt;
struct velocity v,v1;

float			delta_max;
float 			dist[Nmax][Max_size];
int 			dsize[Nmax];

float			dtheta;
float			norm;
float 			theta;

float X[Nmax], Y[Nmax];

printf("\n Landscape");


swt.size=swarm_size;

for (i=0;i<swt.size;i++)/* For each particle ...
						Note: it may arrive, particularly for small graphs
						that two particles are identical */
	{
	
	swt.p[i]=init;
	swt.p[i].v=alea_velocity(G.N-2);
	swt.p[i]=pos_plus_vel(swt.p[i],swt.p[i].v,0,0,levelling);
	swt.p[i].rank=i;
	swt.p[i].f=f(swt.p[i],-1,-1); /* Compute the cost function value */
	}

display_swarm(swt);

centert=swt.p[0];

for (i=0;i<swt.size;i++)
	{
	if (swt.p[i].f<centert.f)
		centert=swt.p[i];
	}

printf("\n Center particle:");
display_position(centert,1);

v1.size=1;

for (i=0;i<swt.size;i++)
	{
	if (i!=centert.rank)
		{
		pt=swt.p[i];
		v=coeff_pos_minus_pos(centert,pt,1,0,1,trace);
		printf("\n Distance (number of transpositions) %i",v.size);
		printf("\n %f",pt.f);
		//fprintf(file,"\n%f\n",pt.f);
		dsize[i]=v.size;
		dist[v.size][i]=pt.f;
		for (k=0;k<v.size;k++)
			{
			v1.comp[0][0]=v.comp[k][0];
			v1.comp[0][1]=v.comp[k][1];
			pt=pos_plus_vel(pt,v1,0,0,levelling);
			printf("\n %f",pt.f);
			//fprintf(file,"%f\n",pt.f);
			dist[v.size-k-1][i]=pt.f;
			}
			
		}
	}
	
// Save for future use	
for (i=0;i<swt.size;i++)
	{
	if (i!=centert.rank)
		{
		for (k=0;k<=dsize[i];k++)
			{
			fprintf(file,"%f ",dist[k][i]);
			}
		fprintf(file,"\n");
		}
	}


// Transform for graphical use

printf("\n Write landscape around the best particle\n");
fprintf(file,"\n Landscape around the best particle\n");

// Normalization

delta_max=0;

for (i=0;i<swt.size;i++)
	{
	if (i!=centert.rank)
		{
		if (abs(dist[k][i]-centert.f)>delta_max)
			delta_max=dist[k][i]-centert.f;
		}	
	}

norm=(G.N-1)/delta_max;

dtheta=4*acos(0)/(swt.size-1);
theta=0;

for (i=0;i<swt.size;i++)
	{
	if (i!=centert.rank)
		{
		fprintf(file,"\n Theta %f. %i values\n",theta,dsize[i]+1);
		for (k=0;k<=dsize[i];k++)
			{
			X[k]=k*cos(theta)-(dist[k][i]-centert.f)*sin(theta)*norm;
			Y[k]=k*sin(theta)+(dist[k][i]-centert.f)*cos(theta)*norm;
			}
		theta=theta+dtheta;
		
		for (k=0;k<=dsize[i];k++)
			{
			fprintf(file,"%f %f\n",X[k],Y[k]);
			}
		}
	}
	
printf("\n Landscape written (file trace.txt)");
}

/*------------------------------------------------------------------- 	MAP */
void map(FILE *file,struct particle pbest,int selection)
{
float		d;
int			i;
struct particle p;

for (i=0;i<selection;i++)
	{
	
	p=init;
	p.v=alea_velocity(G.N-2);
	p=pos_plus_vel(p,p.v,0,0,levelling);
	p.rank=i;
	p.f=f(p,-1,-1); /* Compute the cost function value */
	d=distance(pbest,p);
	//printf("\n %f %f",d,p.f);
	fprintf(file,"%f %f\n",d,p.f);
	}


}

/*------------------------------------------------------------------- 	NB_LOCAL_MIN */
int	nb_local_min(struct graph G)
{
int				i;
float			n_move,n_move_up;
int				nb_min;
struct particle	pt;
float			proba_up;
struct velocity	v;
struct swarm 	swt;
float z1,z2,z3;

printf("\n Number of local minimums");

// Initialization
swt.size=MAX(5,G.N);

for (i=0;i<swt.size;i++)/* For each particle ...
						Note: it may arrive, particularly for small graphs
						that two particles are identical */
	{
	
	swt.p[i]=init;
	swt.p[i].v=alea_velocity(G.N-2);
	swt.p[i]=pos_plus_vel(swt.p[i],swt.p[i].v,0,0,levelling);
	swt.p[i].rank=i;
	swt.p[i].f=f(swt.p[i],-1,-1); /* Compute the cost function value */
	}
	
/* Estimation of the proba that, for an unique move, the new position
 (immediate physical neighbour) is higher than the previous position
*/
n_move=0;
n_move_up=0;

	for (i=0;i<swt.size;i++) // For each particle
		{
		n_move=n_move+1;
		v=alea_velocity(1); // move at random
		pt=pos_plus_vel(swt.p[i],v,monotony,1,levelling);
		
printf("\nInitial f %f, final f %f",swt.p[i].f,pt.f);

		if(pt.f>swt.p[i].f) // check if the result is greater
			{
			n_move_up=n_move_up+1;
			}
		}

proba_up=n_move_up/n_move;
printf("\n Total moves %f, up moves %f, proba %f",n_move,n_move_up,proba_up);

z1=log (fact(G.N));
z2=log (proba_up)*G.N*(G.N-1);
z3=z1+z2;
nb_min=exp(z3);
printf("\n z1, z2, z3 nb_min: %f, %f, %f, %f", z1,z2,z3,exp(z3));

return nb_min;
}
/*------------------------------------------------------------------- 	PARTICLE_VISU */
struct graph particle_visu(struct particle p) /* Just for visualization */
{
float			alpha=1.1;
struct graph 	pt;
int				i,j;
int				n,n1;


for (i=0;i<G.N;i++)
	for (j=0;j<G.N;j++)
			pt.v[i][j]=0;
			
for (i=0;i<G.N;i++)
	{
	n=p.x.s[i]-1;
	n1=p.x.s[i+1]-1;
	pt.v[n][n1]=G.v[n][n1];
	if (pt.v[n][n1]==0) pt.v[n][n1]=alpha*G.l_max;
	}

pt.N=G.N+1;
return pt;
}


/*------------------------------------------------------------------- SAVE_SSM */
void save_SSM(FILE *file,struct swarm sw,struct sw_dist swd)
{

int i,j;

if (trace>0) printf("\n Saving SSM, graph size %i, sawarm size %i",G.N,sw.size);

fprintf(file,"%i %i\n",G.N,sw.size); /* Save graph size and swarm size (= (N-1)! ) */

for (i=0;i<sw.size;i++)
	{
	fprintf(file,"%i ",i+1); /* Particle number */
	for (j=0;j<G.N+1;j++)  /* Particle description (sequence) */
		{
		fprintf(file,"%i ",sw.p[i].x.s[j]);
		}
	for (j=0;j<sw.size;j++) /* f value */
		{
		fprintf(file,"%f ",swd.d[i][j]);
		}
	fprintf(file,"\n");

	}
if (trace>0) printf("\n End of saving SSM");

}

/*------------------------------------------------------------------- SAVE_TOPO */
void save_topo(FILE *file,struct swarm sw)
/* Save distances to the best particle */
{
int i,j;

for (i=0;i<sw.size;i++)
	{
	fprintf(file,"%i %f %f ",i+1,swd.d[i_best][i],sw.p[i].f);  /* Particle number, distance to the best, f value */
	for (j=0;j<G.N+1;j++) /* Particle description */
		{
		fprintf(file,"%i ", sw.p[i].x.s[j]);
		}
	fprintf(file,"\n");
	}		
}



/*------------------------------------------------------------------- SW_F */
struct swarm sw_f(struct swarm sw) /* All cost function values for the particles 
							  Modify the global variable sw1 */
{
int 			i;
struct	swarm	swt;	
float			x;

if (trace>0) printf("\n Compute all cost function values of all particles of the swarm (size: %i)",sw.size);

swt=sw;

for (i=0;i<sw.size;i++)
	{
	x=f(sw.p[i],-1,-1);
	
	/*printf("\n particle %i, f value %f",i+1,x);*/
	
	swt.p[i].f=x; 
	}
return swt;
}



/*------------------------------------------------------------------- SW_DISTANCES */
void sw_distances(struct swarm sw)
/* Note: we suppose here d(i,j)=d(j,i) */
{
int	i,j;
int sw_size;
if (trace>0) printf("\n Compute all distances between all particles of the swarm (size: %i)",sw.size);

sw_size=sw.size;

for (i=0;i<sw_size-1;i++) /* Pre-compute all distances, in order to speed up the algorithm */
	{
	if (trace>0) printf("\n  distances for particle %i, begin",i+1);
	for (j=i+1;j<sw_size;j++)
		{
		//swd.d[i][j]=particle_dist(sw.p[i],sw.p[j]); // Distance function of common paths
		swd.d[i][j]=distance(sw.p[i],sw.p[j]);
		swd.d[j][i]=swd.d[i][j];
		}
	swd.d[i][i]=0;
	if (trace>0) printf(" / end ");
	}
swd.d[sw_size-1][sw_size-1]=0;
if (trace>0) printf("\n End of distance computing");
}
