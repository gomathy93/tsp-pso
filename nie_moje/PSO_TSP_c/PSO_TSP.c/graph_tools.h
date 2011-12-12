struct graph			best_visu(struct particle best,struct graph G); /* Just for visualization */
void					check_Hamilton_cycle(struct graph G);
struct graph 			graph_min_max(struct graph G);
struct val				graph_min_tour(struct graph G);
struct graph			random_graph(int N,int n_values,int density);
struct graph			read_graph(FILE *file, int trace);
struct graph			TSP_to_Hamilton(struct graph G);	