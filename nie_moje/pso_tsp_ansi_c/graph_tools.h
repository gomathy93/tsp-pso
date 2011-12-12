int						check_Hamilton_cycle(struct graph G);
void          display_graph(struct graph G);
struct graph 	graph_min_max(struct graph G);
struct val		graph_min_tour(struct graph G);
struct   seq  min_tour(struct graph G,int i);
struct   seq  min_tour_2(struct graph G,int i,int level); 
struct graph	read_graph(FILE *file, int trace);
int           sequence_similar(struct seq s1,struct seq s2) ;
struct graph	TSP_to_Hamilton(struct graph G);
