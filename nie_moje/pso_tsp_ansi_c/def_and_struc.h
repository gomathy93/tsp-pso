#define	Nmax 51 // Maximum node number 
#define Vmax 100 // Maximum number of velocity components 
#define Max_size 60 // Max swarm size 
#define MaxEdge 1300  // Maximum possible arc value

// Structures

struct coeff		{float c[6];}; /* Coefficients for convergence */
struct seq			{int size;int s[Nmax+1];};
struct val			{float v[MaxEdge];float min;};
struct velocity		{int size;int comp[Vmax][2];};
struct particle		{int rank;struct seq x;struct velocity v;double f;double fl;struct seq best_x;double best_f;double best_time;};
struct swarm		{int size; struct particle p[Max_size+1];int rank_best;};
struct graph 		{int N;double v[Nmax][Nmax];double l_max;double l_min;int n_edge;}; /* Arcs valuations */

