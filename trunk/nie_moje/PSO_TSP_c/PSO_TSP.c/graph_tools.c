#include <stdio.h>
#include "math_add.h"
#include "def_and_struc.h"

/*------------------------------------------------------------------- 	BEST_VISU */
struct graph best_visu(struct particle best, struct graph G) /* Just for visualization */
{
float			alpha=1.1;
struct graph 	bt;
int				i,j;
int				n,n1;


for (i=0;i<G.N;i++)  /* Initialization. May be useful with some compilers */
	for (j=0;j<G.N;j++)
			bt.v[i][j]=-1;
			
for (i=0;i<G.N;i++)
	{
	n=best.x.s[i];
	n1=best.x.s[i+1];
	bt.v[n][n1]=G.v[n][n1];
	if (bt.v[n][n1]<0) bt.v[n][n1]=alpha*G.l_max;
	}

bt.N=G.N;
return bt;
}

/*------------------------------------------------------------------- 	CHECK_HAMILTON_CYCLE */
void check_Hamilton_cycle(struct graph G)
/* Just a few preliminary tests. Can evidently be improved ... or removed, if you want to */
{
int i,j;
int	in;
int	out;
int	tournament;

for (i=0;i<G.N;i++) /* At least one incoming arc and one outcoming */
	{
	in=0;
	out=0;
	for (j=0;j<G.N;j++)
		{
		if (i!=j)
			{
			if (G.v[i][j]>=0) out=out+1; /* Outgoing arc */
			if (G.v[j][i]>=0) in=in+1; /* Incoming arc */
			}
		}
	if (in*out==0)
		{
		printf("\n\n WARNING. There is NO Hamiltonian cycle. I look for a Hamiltonian path\n");
		return;	
		}
	}
/*------------------------------- */
tournament=0;
for (i=0;i<G.N;i++) /* L. Redei's theorem (tournament) */
	{
	for (j=0;j<G.N;j++)
		{
		if (i!=j)
			{
			if (G.v[i][j]<0 && G.v[j][i]<0) return;	
			}
		}
	}
printf("\n\n LUCKY YOU. There IS at least one Hamiltonian cycle. I look for the best one\n");
return;	
}

/*------------------------------------------------------------------- GRAPH_MIN_MAX */
struct graph graph_min_max(struct graph G)
{
/* FInd the min and the max arc values in a graph, and the edge number
Note that G(i,i)=0, to simplify evaluations of objective function
*/
int 		i,j;
struct graph Gt;
float		x;

Gt=G;
Gt.n_edge=0;
Gt.l_max=0;
Gt.l_min=1000000;

for (i=0;i<G.N;i++)
	{
	for (j=0;j<G.N;j++)
		{
		if (j!=i)
			{
			Gt.l_max=MAX(G.v[i][j],Gt.l_max);
			if (G.v[i][j]>=0)  
				{
				Gt.l_min=MIN(G.v[i][j],Gt.l_min); /* Remember ... -1 is a special value = no arc */
				Gt.n_edge=Gt.n_edge+1;
				}
			}
		}
	}

printf("\n Graph %i vertices, %i edges, min value: %f, max value: %f",Gt.N,Gt.n_edge,Gt.l_min,Gt.l_max);
x=100*Gt.n_edge/(G.N*(G.N-1));
printf("\n Graph density %f",x); 
return Gt;
}

/*------------------------------------------------------------------- GRAPH_MIN_TOUR */
struct val graph_min_tour(struct graph G)
{ /* Compute a value surely smaller than (or equal to) the best tour
and give statistics about arc values
IMPORTANT. Arc values are supposed to b integers
Also, you must have computed l_min (cf graph_min_max
*/
int 	i,j;
int		lmax;
int		n_diff_val;
int		nval;
int		rank;
float	x;
struct val val;

lmax=G.l_max;

if (G.l_max>MaxEdge)
	{
	printf("\n SORRY, I cannot estimate the minimum. \n You should increase the parameter MaxEdge up to %i",lmax);
	}

for (i=0;i<MaxEdge;i++)
	val.v[i]=0;

for (i=0;i<G.N;i++)
	{
	for (j=0;j<G.N;j++)
		{
		if (i==j) continue;
		if(G.v[i][j]>=0)
			rank=G.v[i][j]-G.l_min;
		else
			rank=G.l_max+1-G.l_min;
		val.v[rank]=val.v[rank]+1;
		}
	}
	
val.min=0;
n_diff_val=0;
nval=0;

printf("\n Arc value,  number");

for (i=0;i<G.l_max-G.l_min+1;i++)
	if (val.v[i]>0)
		{
		n_diff_val=n_diff_val+1;
		if (trace>1) printf("\n %f    %f",G.l_min+i,val.v[i]);
		}	

for (i=0;i<G.l_max;i++)
	{
		for (j=0;j<val.v[i];j++)
			{
			val.min=val.min+(G.l_min+i);
			nval=nval+1;
			if (nval>=G.N) goto end;
			}
	}
	
end:

printf("\n Number of different arc values: %i",n_diff_val);

/* For each value, evaluate a "difficulty to remove" */
x=G.N*(G.N-1);
for (i=0;i<G.l_max-G.l_min+2;i++)
	{
	if (val.v[i]>0)
		val.v[i]=val.v[i]/x;
	}

printf("\n Smallest possible tour (if exists): %.0f",val.min);
return val;	
}

/*------------------------------------------------------------------- RANDOM_GRAPH */
struct graph random_graph(int N,int n_values,int density)
/* Generate a graph randomly */
{
int				arc;
struct graph	Gt;
int				i,j;

for (i=0;i<N;i++) 
	{
	for (j=0;j<N;j++)
		{
		arc=alea(0,100);
		if (arc<=density) 
			{
			Gt.v[i][j]=alea(1,n_values);
			}
		else
			{
			Gt.v[i][j]=-1;
			}
		}
	Gt.v[i][i]=0;
	}
Gt.N=N;
return Gt;
}

/*------------------------------------------------------------------- READ_GRAPH */
struct graph read_graph(FILE *file, int trace)
{
/* TSPLIB MATRIX format. One value/line. First value = number of nodes
each value = arc value. If zero => no arc
*/
char			bidon[50];
char			comment[100];
char			edge_weight_format[30];
char			edge_weight_type[30];
struct graph	Gt;
int 			i,j;
char			name[20];
char			type[20];


printf("\nI am reading the graph");

fscanf(file," %s %s\n",bidon,name);
fscanf(file," %s %s\n",bidon,type);


	fscanf(file," %s %s\n",bidon,comment);
	fscanf(file,"%s %i\n",bidon,&Gt.N); // dimension
	fscanf(file,"%s %s\n",bidon,edge_weight_type); 
	fscanf(file,"%s %s\n",bidon,edge_weight_format); 
	fscanf(file,"%s\n",bidon); 


	printf("\n Name: %s, type: %s, (%s)",name,type,comment);
	printf("\n Number of nodes: %i",Gt.N);
	printf("\n %s %s\n",edge_weight_type,edge_weight_format);

	if (edge_weight_type[0]=='E' && edge_weight_format[0]=='F')
		{		
		for (i=0;i<Gt.N;i++)
			{
			for (j=0;j<Gt.N-1;j++)
				{
				fscanf(file,"%f ",&Gt.v[i][j]);
					if (trace>2) printf(" %f",Gt.v[i][j]);
				}
			
			fscanf(file,"%f\n",&Gt.v[i][Gt.N-1]);
			if (trace>2) printf(" %f\n",Gt.v[i][Gt.N-1]);
			Gt.v[i][i]=0;
			}
		return Gt;
		}	
}

/*------------------------------------------------------------------- 	TSP_TO_HAMILTON */
struct graph TSP_to_Hamilton(struct graph G)
{
struct graph	Gt;
int 			i,j;

for (i=0;i<G.N;i++)
	{
	for (j=0;j<G.N;j++)
		{
		if (G.v[i][j]>=0) Gt.v[i][j]=1;
		}
	Gt.v[i][i]=0;	
	}
Gt.N=G.N;
return Gt;
}
