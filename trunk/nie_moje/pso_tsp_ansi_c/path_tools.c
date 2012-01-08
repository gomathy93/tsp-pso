#include "stdinc.h"

/*------------------------------------------------------------------- DISPLAY_PATH */
void display_path(struct path path) 
{
int j;

	printf("\n");
	/*printf("\n length %i",path.length);*/
	for (j=0;j<path.length+1;j++)
		{
		printf(" %i",path.path[j]);
		}


}
/*------------------------------------------------------------------- DISPLAY_PATHS */
void display_paths(struct path_list paths)
{
int 				i;

printf("\n  paths :");

for (i=0;i<paths.size;i++)
	{
	display_path(paths.paths[i]);
	}
}

/*------------------------------------------------------------------- LOOK_FOR_PATH */
int	look_for_path(struct path path,struct path_list paths) 
/* Is the given path a sub-path in the list of paths ? 1 if yes, 0 else */
{
int i,j,k;

if (trace>4)
	{ 
	printf("\nlook for common path ");
	for (j=0;j<path.length+1;j++) printf(" %i",path.path[j]);
	printf(" in path list of %i path(s)",paths.size);
	}


for (i=0;i<paths.size;i++) /* For each path of the list ... */
	{
	
	if (trace>4) 
		{
		printf("\n path number %i: ",i+1 );
		for (j=0;j<paths.paths[i].length+1;j++) printf(" %i",paths.paths[i].path[j]);
		}
	
	
	for (j=0;j<paths.paths[i].length-path.length+1;j++) /* ... for each possible sub-path ... */
		{
		if (path.path[0]==paths.paths[i].path[j])  /* ... if the first node is common ... */
			{
			
	if (trace>4) printf("\n first common node %i",path.path[0]);
	
			for (k=j+1;k<j+path.length+1;k++)/* ... check the following nodes */
				{
				
	if (trace>4) printf("\n     following nodes %i/%i", path.path[k-j],paths.paths[i].path[k]);
				if (path.path[k-j]!=paths.paths[i].path[k]) goto next_subpath;
				}
	if (trace>4) printf("\n                      OK");
			return 1;
			}
		next_subpath:;	
		}

	}
return 0;

}



/*------------------------------------------------------------------- PARTICLE_DIST */
float particle_dist(struct particle p1,struct particle p2)
{
/* Compute "distance" between two "particles" (sequences of nodes) */
float 				d;
int 				i,j;
int					is_inside;
struct path_list 	paths1,paths2;
float				similarity;
struct path			subpath;	

 if (compare_particle(p1,p2)==0) return 0;  /* If it is the same particle, nothing to compute */

/* Function of the common paths */

	paths1=p1.paths; /* Paths (acceptable sequences) of p1 */
	paths2=p2.paths; /* Paths (acceptable sequences) of p2 */
	
	if (trace>3) 
		{
		printf("\n Distance between:");
		printf("\n    particle ");display_particle(p1);
		display_paths(paths1);
		printf("\n    particle ");display_particle(p2);
		display_paths(paths2);
		}
		
	/* First, calculate a similarity */
	/* WARNING: this algorithm implies all nodes are different in a particle,
	 (except the first and the last one) */
	similarity=0;
	
	
	for (i=0;i<paths1.size;i++) /* For each path of p1 ...*/
		{
	
		/*... look for the longest sub-path which is also in p2 ... */
		/* (in practice, just look incrementally for sub-paths of length 1 ) */
		subpath.length=1;
		for (j=0;j<paths1.paths[i].length;j++)
			{
			subpath.path[0]=paths1.paths[i].path[j];
			subpath.path[1]=paths1.paths[i].path[j+1];
			
			is_inside=look_for_path(subpath,paths2);
			if (is_inside==1) 
				similarity=similarity+1;
				/*... and add (incrementally) the length of this sub-path to the similarity */
			}
		}
	/* Second, the distance is calculated from the similarity */
	d=1-similarity/(G.N-1); /* (remember, the length of each particle is G.N+1 nodes) */
	
	if (trace>3) printf("\n       distance: %f",d);
	return d;


}

/*------------------------------------------------------------------- PATHS */
struct path_list paths(struct particle p) /* List of all paths (acceptable sequences) in a particle */
{
int 				i,i1;
struct path			patht;
struct path_list 	pathst;
int					test=0;

pathst.size=0;
patht.length=0;

if (test>0) printf("\n");

for (i=0;i<G.N;i++) /* For each node of the particle but the last one ... */
	{	
	patht.path[patht.length]=p.x.s[i]; /* ... initialize a possible path */
	if (i<G.N-1) {i1=i+1;} else {i1=0;}
	
	if (test>0) {printf("\n i,i1: %i,%i",i,i1);
		printf("     arc %i=>%i/%f",p.x.s[i],p.x.s[i1],G.v[p.x.s[i]-1][p.x.s[i1]-1]);
		}
	
	if (G.v[p.x.s[i]-1][p.x.s[i1]-1]>0)
			  /* If there is an arc between the current node and the following one ... */
		{
		patht.length=patht.length+1;
		patht.path[patht.length]=p.x.s[i+1]; /* ... this second node is added to the path */
		}
	if ((i1>0 && G.v[p.x.s[i1]-1][p.x.s[i1+1]-1]==0) || (i1==0)) /* If there is _no_ arc after that or if it is the last node ... */
		{
		if (patht.length>0) /* ... and if a path exists ... */
			{
			pathst.paths[pathst.size]=patht; /* ... then the path is added to the list. */
			pathst.size=pathst.size+1;
			patht.length=0;
			}
		}
	}
if (trace>4) display_paths(pathst);
	
return pathst;
}

/*------------------------------------------------------------------- SW_PATHS */
struct swarm sw_paths(struct swarm sw)
{

int	i;
int sw_size;
struct swarm swt;

swt=sw;

if (trace>0) printf("\n Compute all paths of all particles of the swarm. Size %i",sw.size);

sw_size=sw.size;
if (a_queen==1) 
	{
	printf(", including the queen");
	sw_size=sw_size+1;
	}
for (i=0;i<sw_size;i++) /* Pre-compute all paths, in order to speed up the algorithm */
	{
	if (trace>0) printf("\n   paths for particle %i",i+1);
	swt.p[i].paths=paths(sw.p[i]);
	}
	
if (trace>0) printf("\n End of paths computing");
return swt;
}
