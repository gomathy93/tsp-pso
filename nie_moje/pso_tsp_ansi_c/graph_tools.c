#include "stdinc.h"

/*------------------------------------------------------------------- 	CHECK_HAMILTON_CYCLE */
int check_Hamilton_cycle(struct graph G)
/* Just a few preliminary tests. Can evidently be improved ... or removed, if you want to */
{
int i,j;
int	in;
int   d;
int	out;

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
		return 0;	 // NO Halmitonian cycle
		}
	}
//------------------------------- L. Redei's theorem (tournament)
// arc i=>j, or j=>i

for (i=0;i<G.N;i++)  
	{
	for (j=0;j<G.N;j++)
		{
		if (i!=j)
			{
			if (G.v[i][j]<0 && G.v[j][i]<0) goto test2;	   // No arc between i and j
			}
		}
	}

   return 1; // Sure

 test2:
// ------------------------------ Dirac
// All degrees >= N/2
 
for (i=0;i<G.N;i++)
	{
      d=0;
	for (j=0;j<G.N;j++)
		{
		if (i!=j)
			{
			if (G.v[i][j]>=0 || G.v[j][i]>=0 ) d=d+1;	   // Total degree
			}
		}
  if (d<G.N/2) goto test3 ;
	}

return 2;

test3:  // Nothing for the moment
return 3;

}

//================================================= DISPLAY_GRAPH
void     display_graph(struct graph G)
{
  int i,j;
 for (i=0;i<G.N;i++)
 {
 printf( "\n%i /",i+1);
 for (j=0;j<G.N;j++)
    printf(" %.0f",G.v[i][j]);
 }
}

/*------------------------------------------------------------------- GRAPH_MIN_MAX */
struct graph graph_min_max(struct graph G)
{
/* FInd the min and the max arc values in a graph, and the edge number
Note that G(i,i)=0, to simplify evaluations of objective function
*/
int 		i,j;
struct graph Gt;
double		x;

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
			Gt.l_max=max(G.v[i][j],Gt.l_max);
			if (G.v[i][j]>=0)  
				{
				Gt.l_min=min(G.v[i][j],Gt.l_min); /* Remember ... -1 is a special value = no arc */
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
IMPORTANT. Arc values are supposed to be integers
Also, you must have computed l_min (cf graph_min_max)
*/
int 	i,j;
int		lmax;
int		n_diff_val;
int		nval;
int		rank;
double	x;
struct val val;

lmax=G.l_max;

if (G.l_max>MaxEdge)
	{
	printf("\n SORRY, I cannot estimate the minimum");
   printf(" \n You should increase the parameter MaxEdge up to %i",lmax);
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
printf("\n(WARNING) This is valid for _integer_ arc values");

/* For each value, evaluate a "difficulty to remove" */
x=G.N*(G.N-1);
for (i=0;i<G.l_max-G.l_min+2;i++)
	{
	if (val.v[i]>0)
		val.v[i]=val.v[i]/x;
	}

printf("\n Smallest possible tour (if exists): %f",val.min);
return val;	
}

 //============================================================ MIN_TOUR
 struct   seq   min_tour(struct graph G,int i)
{
    //    Start from node i, and systematically add the smallest arc still free
    
 double        big_value;
  int             j;
  double       min;
  int             next=0; // Just for my stupid compiler
  int             rank;
  double       val;
  struct seq x;
  int          used[Nmax]={0};
 
   big_value= G.l_max+1;
  x.size=G.N;
  rank=0;
  x.s[rank]=i;
  used[i]=1;

  loop:
  min=big_value+1;
  
  for (j =0;j<G.N;j++) // Check all neighbours of x.s[rank]
  {
     if (used[j]==1) continue;
     val= G.v[ x.s[rank]][j];
      if (val<0) val=big_value;    // For non existent arc

      if (val<min)
      {
         next=j;
         min=val;
       }
   }
   rank=rank+1;
   x.s[rank]=next;
   used[next]=1;
   if (rank<G.N-1)  goto loop;
 
  x.s[x.size]=x.s[0]; // To complete the tour
  return x;
}                                                             

 //============================================================ MIN_TOUR_2
 struct   seq   min_tour_2(struct graph G,int i,int level)
{
    //    Start from node i, and systematically add the smallest 2-arcs path still free

 double        big_value;
  int             j,k;
  double       min;
  int             next=0,next2=0;
  int             rank;
  double       val_j,val_k;
  struct seq x;
  int          used[Nmax]={0};

   big_value= G.l_max+1;
  x.size=G.N;
  rank=0;
  x.s[rank]=i;
  used[i]=1;

  loop:
 //printf("\n rank %i",rank);
  min=2*big_value+1;

  for (j =0;j<G.N;j++) // Check all neighbours of x.s[rank]
  {
 //printf("\n %i, %i, %f /",j,used[j],min);
     if (used[j]==1) continue;
     val_j= G.v[ x.s[rank]][j];
      if (val_j<0) val_j=big_value;    // For non existent arc

      if (rank==G.N-2)
      {
         x.s[G.N-1]=j;
         goto end;
      }
      
      for(k=0;k<G.N;k++)
      {
 //printf("\n %i",k);
      if (k==j) continue;
      if (used[k]==1) continue;
        val_k= G.v[ j][k];
        if (val_k<0) val_k=big_value;

         if (val_j+val_k<min)
         {
            next=j;
            next2=k;
            min=val_j+val_k;
         }
      }
  }
  //printf("\n next %i",next);
  if(level==1)
  {
   rank=rank+1;
       x.s[rank]=next;
       used[next]=1;
       if (rank<G.N-1) goto loop;
  }

  if (level==2)
  {
       rank=rank+1;
       x.s[rank]=next;
       used[next]=1;

       rank=rank+1;
       x.s[rank]=next2;
       used[next2]=1;
       if (rank<G.N-2) goto loop;    
  }

  
 end:
  x.s[x.size]=x.s[0]; // To complete the tour
  return x;
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
each value = arc value. If <0 => no arc
*/
char			bidon[50];
char			comment[100];
//double         delta;
char			edge_weight_format[30];
char			edge_weight_type[30];
struct graph	Gt;
int 			i,j;
char			name[20];
int scanR;
char			type[20];
float         zzz;


printf("\nI am reading the graph");

scanR=fscanf(file," %s %s\n",bidon,name);
scanR=fscanf(file," %s %s\n",bidon,type);
	scanR=fscanf(file," %s %s\n",bidon,comment);
	scanR=fscanf(file,"%s %i\n",bidon,&Gt.N); // dimension
	scanR=fscanf(file,"%s %s\n",bidon,edge_weight_type); 
	scanR=fscanf(file,"%s %s\n",bidon,edge_weight_format); 
	scanR=fscanf(file,"%s\n",bidon); 


	printf("\n Name: %s, type: %s, (%s)",name,type,comment);
	printf("\n Number of nodes: %i",Gt.N);
	printf("\n %s %s\n",edge_weight_type,edge_weight_format);

	if (edge_weight_type[0]=='E' && edge_weight_format[0]=='F')
		{		
		for (i=0;i<Gt.N;i++)
			{
                for (j=0;j<Gt.N;j++)
				{
                        scanR=fscanf(file,"%e ",&zzz);
                            Gt.v[i][j]=zzz;
					if (trace>2) printf(" %f",Gt.v[i][j]);
				}
			Gt.v[i][i]=0;
			}
   

    for (i=0;i<Gt.N;i++)  
			{
			for (j=0;j<Gt.N;j++)
				{
                       if (Gt.v[i][j]>0) Gt.v[i][j]=integer_coeff*Gt.v[i][j];
                       }
                    }
  if (integer_coeff!=1) printf("\nWARNING. I have multiplied all arc values by %f",integer_coeff);
                   
		return Gt;
		}
   printf("\nERROR by reading graph");   
  return Gt; 
}

//====================================================== SEQUENCE_SIMILAR
int   sequence_similar(struct seq s1,struct seq s2)
{
 /* Check if two sequences are globally similar:
 - same first value
 - same last value
 - same  set of values (not taking order into account)
 return 0 if false, 1 if true
 */
 int           i;
 int           j;
int            size;
 
   if (s1.s[0]!=s2.s[0]) return 0;

   size=s1.size;
   if (s1.s[size-1]!=s2.s[size-1]) return 0;
   if (size!=s2.size) return 0;
   
   //printf("\n\n"); for (i=0;i<s1.size;i++) printf(" %i",s1.s[i] );  printf("\n");  for (i=0;i<s1.size;i++) printf(" %i",s2.s[i] );
   
   for (i=1;i<size-1;i++)
   {
           for (j=0;j<size-1;j++)
           {
              if(s1.s[i]==s2.s[j]) goto next_i;
           }
            return 0;

    next_i:;
   }
  //printf("\n sequence_similar OK");
 return 1;
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
