#define	Nmax 50 // Maximum node number 
#define Vmax 70 // Maximum number of velocity components 
#define Max_size 130 // Max swarm size 
#define MaxEdge 1500  // Maximum possible arc value

// Structures

struct coeff		{float c[6];}; /* Coefficients for convergence */
struct seq			{int size;int s[Nmax+1];};
struct val			{float v[MaxEdge];float min;};
struct velocity		{int size;int comp[Vmax][2];};
struct particle		{int rank;struct seq x;struct velocity v;float f;float fl;struct seq best_x;float best_f;};
struct swarm		{int size; struct particle p[Max_size+1];int rank_best;};
struct graph 		{int N;float v[Nmax][Nmax];float l_max;float l_min;int n_edge;}; /* Arcs valuations */

