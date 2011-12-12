struct velocity			alea_velocity(int N);
struct particle			auto_move(struct particle p,int type, int monotony, int levelling, int trace);
struct particle			best_neighbour(struct swarm sw1,struct particle p,int k, int hood_type,int monotony,int equiv_v, int trace);
struct velocity			coeff_pos_minus_pos(struct particle p1,struct particle p2,double coeff,int monotony,int equiv_v, int trace);
struct velocity			coeff_times_vel(struct velocity v,double coeff, int equiv_v,int trace);
int						compare_particle(struct particle p1,struct particle p2);
double					distance(struct particle p1,struct particle d2,int type_dist);
struct velocity			equiv_vel(struct velocity v, int equiv_v,int trace);
double					f(struct particle p,int rank1, int rank2); 
double					f_levelling(struct graph G, struct particle p1, struct particle p2);
struct   particle       splice_around(struct swarm sw, struct particle p, int hood_type,int hood_size);
struct particle			move_towards(struct particle p1,struct particle p3,struct coeff c,int move_type,int explicit_vel,int conv_case,int equiv_v, int trace);
struct graph			particle_visu(struct particle p); /* Just for visualization */
struct particle			pos_plus_vel(struct particle p,struct velocity v, int monotony,int type,int levelling);
struct particle 		previous_best(struct particle p,int type);
struct   particle        sequence_subst(struct particle p1, struct particle p2,int size_max);
double					tax_no_arc(int times,struct graph G);
struct velocity			vel_plus_vel(struct velocity v1,struct velocity v2,int equiv_v, int trace);

void BB_update(struct particle p);    // Update the blackboard
struct particle BB_use(struct particle p); // Build a tour, using the blackboard