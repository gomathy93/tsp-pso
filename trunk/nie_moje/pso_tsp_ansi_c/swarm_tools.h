struct particle 		init_particle(struct graph G);
struct swarm			init_swarm(int size, struct graph G,int trace);
//int						no_hope(struct swarm sw,int swarm_size, int adaptive,float nohope,int hood);
struct swarm 			reduce_swarm(struct swarm sw);
//struct swarm			re_hope(struct swarm sw,int swarm_size,int trace,int auto_move_type,int monotony,int levelling);
void					save_swarm(FILE *file,struct swarm sw);
struct swarm			sw_arcs(struct swarm sw);
double                        tot_vel(struct swarm sw);
//float					threshold(struct particle best,struct val val,struct graph G);
