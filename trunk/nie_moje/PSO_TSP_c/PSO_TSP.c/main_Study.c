/*See Hamilton_TSP program
Just study some particular points

*/

/* Useful headers */
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <syscall.h>

/* Predefined constants and structures */
#include "def_and_struc.h"

/* More structures */
#include "study_struc.h"

/* Subroutines definitions */

#include "math_add.h"
#include "display.h"
#include "graph_tools.h"
#include "swarm_tools.h"
#include "particle_tools.h"

#include "study_tools.h"

/* Global variables */

#include "global_var.c"

struct graph		particle_matrice; /* Just for visualization */
int i_best;
struct sw_dist		swd; /* To memorize f values, to speed up the algorithm */
float				topo[Max_size][2]; /* For visualization. Curve (distance to the best,cost function value) */



/* Visualizations. */

#include "visualization.c"
#include "study_visualization.c"


/* =============================== Subroutines ================================== */

#include "math_add.c"
#include "display.c"
#include "graph_tools.c"
#include "swarm_tools.c"
#include "particle_tools.c"

#include "f_TSP.c"
#include "display_position_TSP.c"

#include "study_tools.c"

/* =============================================================================== */
void main()
{
FILE *f_graph;

FILE *f_cheats;
FILE *f_topo; 
FILE *f_SSM; /* For Search Space Maps */
FILE *f_trace;

int				choice;
int				density;
int				i;
char			graph[256]; /* Graph file name */
float			min_tour;
float			nohope;
int				parallel;
int				rehope; // For test without rehope option (value 0, 1 else)
int				selection;
int				vmax;

struct swarm swt;
int		z;

G.l_max=10;G.l_min=1; // Just for initial visualization

/*----------------- SOME PARAMETERS -------------- */


#include "param.c"



graph:
printf("\n 0 = read a file containing the graph description");
printf("\n 1 = generate a graph randomly");
printf("\n Your choice ? :");
scanf("%i",&choice);

if (choice==1)
	{
	printf("\n Number of nodes ? (<=%i): ",Nmax);
	scanf("%i",&G.N);
	printf("\n How many different arc values ?: ");
	scanf("%i",&vmax);
	printf("\n Graph density ? [1,100]: ");
	scanf("%i",&density);
	density=MIN(MAX(1,density),100);
	
	G=random_graph(G.N,vmax,density);
	goto parameters;
	}

/* Read the valuated graph  (Note: value 0 <=> no arc) */

graph_name:
printf("\nChoose the file containing the graph, please");
if (GetFileDialog(graph)==0) exit(printf("Cancelled\n"));

puts("\nChosen file:");
puts(graph);

f_graph=fopen(graph,"r");

G=read_graph(f_graph,trace);

//----------------
parameters:

G=graph_min_max(G); /* Compute the max and the min arc values, and the number of edges */
val=graph_min_tour(G); // Smaller than or equal to the best tour
min_tour=val.min;
init=init_particle(G);

f_trace=fopen("trace.txt","w");

//---------------------

printf("\n best particle ?: ");
for (i=0;i<G.N+1;i++)
	{
	scanf("%i ",&best_best.x.s[i]);
	}

best_best.x.size=G.N;
best_best.rank=-1;
best_best.v.size=0;
best_best.f=f(best_best,-1,-1);
display_position(best_best,1);

printf("\nHow many selected points?: ");
scanf("%i",&selection);

printf("\n Seed?: ");
scanf("%i",&seed);

map(f_trace,best_best,selection);
goto end;


//----------------------

printf("\n %i",nb_local_min(G));

goto end;






//-----------------------
printf("\n Swarm size ?: ");
	scanf("%i",&swt.size);


landscape(f_trace,swt.size,init);
goto end;


//------------
//cheat_type=1;

new_graph:


printf("\nChoose the file to save the topo, please");
if (PutFileDialog(graph)==0) exit(printf("    Cancelled\n"));

puts("\nChosen file:");
puts(graph);

f_topo=fopen(graph,"w");

/*--------------------------- */
printf("\nChoose the file to save the cheats, please");
if (PutFileDialog(graph)==0) exit(printf("    Cancelled\n"));

puts("\nChosen file:");
puts(graph);

f_cheats=fopen(graph,"w");
/*--------------------------- */
printf("\nChoose the file to save the Search Space Map, please");
if (PutFileDialog(graph)==0) exit(printf("    Cancelled\n"));

puts("\nChosen file:");
puts(graph);

f_SSM=fopen(graph,"w");

/*--------------------------- */

printf("\n 0 = read a file containing the graph description");
printf("\n 1 = generate a graph randomly");
printf("\n Your choice ? :");
scanf("%i",&choice);

if (choice==1)
	{
	printf("\n Number of nodes ? (<=%i): ",Nmax);
	scanf("%i",&G.N);
	
	printf("\n How many different arc values ?: ");
	scanf("%i",&vmax);
	printf("\n Graph density ? [1,100]: ");
	scanf("%i",&density);
	density=MIN(MAX(1,density),100);
	
	G=random_graph(G.N,vmax,density);
	goto parameters;
	}


/*------------------------------ */

z=fact(G.N-1);
printf("\n z= %i, G.N = %i",z,G.N);

if (z>Max_size)
	{
	printf("\n Graph too big (cf. variable Max_size)");
	goto another_graph;
	}


printf("\n Trace level  ? (0,1,2,3,4)  ",trace);
scanf("%i",&trace);

/* Initialization of particles (states) */


sw1=determ_sequ(z,G.N);




sw1=sw_f(sw1); /* All cost function values */

if (trace>0) display_swarm(sw1);

display_part:

printf("\n Want to see a particle ? (y/n): ");
scanf("%s",&answer);
if (answer[0]=='y') 
	{
	printf("\n Which one ? (1 to %i): ",z);
	scanf("%i",&i);
	particle_matrice=particle_visu(sw1.p[i-1]); 
	printf("\n f value: %f",sw1.p[i-1].f);
	goto display_part;
	}


//sw1=sw_paths(sw1); /* All paths */
sw_distances(sw1); /* All distances */

save_SSM(f_SSM,sw1,swd);

display_topo(sw1);
save_topo(f_topo,sw1);

another_graph:
printf("\n Another graph ? (y/n):");
scanf("%s",&answer);
if (answer[0]=='y') 
	{

	goto new_graph;
	}

end:;



}

