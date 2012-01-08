#include "stdinc.h"

// Global variables

int					adaptive; // 0 = constant criterium for Rehope, 1 = depending on current best result
float				almostzero=0.0000000001;
char				answer[1];
int					auto_move_type;
int					batch;
struct particle 	best_best;
struct graph		best_matrice; /* Just for visualization */
int					best_hood_type;// 0 = use best neighbour; 1 = use best neighbour's previous best
//struct 				particle best_solution;
double               time; // Current time step
// struct   graph    BB[2]={0}; // Blackboard for long term swarm memory
struct   graph    BB[2];
int                     BB_option;
int					conv_case; // Cf. convergence_case()
int                     effective_move;
int					equiv_v; // >0 = try to find as smaller equivalent velocity. 0 = do nothing. Cf equiv_vel()
int					explicit_vel; // 0 = no explicit velocity in move_towards
double			eval_f; // Total number of individual evaluations
//struct particle 	extra_best;
struct graph 		G;
int					hood; // Neighbourhood size
int					hood_type; // 0 = social (ranks) 1= physical (distances)
struct particle 	init;
int                     init_option;   // 0 => random; 1 => at least partly deterministic
float         integer_coeff;
int					iter,iter_min,iter_max;
//int					levelling; // 0 = no Local Iterative Levelling; >0 = LIL
//int					LIL;// initial value for "levelling", each time Rehope is performed
float              kappa;
int					monotony; // Cf : 0=none. 1= on f value. 2= on distance
int move[10];
float               phi;
int					reduce; // 0 = no reduced swarm ; 1 = try to reduce the swarm
int					same_best; // How many times the extra_best is still the same
int					same_best_thresh; // Threshold for NoHope for this criterion
float				same_best_val; // Last extra_best value
int					save;
int					seed=1; /* For reinitialize rand() function */
int                     size_max;
int   splice_around_option;  // 0 = whole swarm, 1 = just neighbourhood
int            splice_time;
int					step; // if =1, check previous_best at each substep
int        subst_option; // 0 = try to improve the current position, 1=try to improve  the best previous position

struct swarm		sw1,sw2;
float			target;
int					trace; // Just to display more or less information during the process 
int					type_dist; // 0 = "distance" depending on common arcs; 1 = true distance
struct val			val; // Cf graph_min_tour


FILE *f_trace;
FILE *f_batch_param;
FILE *f_batch_run;
FILE *f_graph; // File containing the graph 
FILE *f_run; // To save swarm positions, for each iteration
FILE *f_trace;
