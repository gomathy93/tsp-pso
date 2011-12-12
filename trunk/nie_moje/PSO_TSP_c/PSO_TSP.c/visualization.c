/**
  	Graph(Out 1); /* The graph where we are looking for a Hamiltonian cycle */
	Directed(1);
	
	Node(Out I,1) For I:InRange(I,1,G.N); /* Generate ID numbers, in sequence */

	Arc(X,Y,1) If G.v[X-1][Y-1]>=0;
	/* 
	ArcThickness(X,Y,Out S,1)  Assign S=
	(G.l_min!=G.l_max) ? (1+2*(G.v[X-1][Y-1]-G.l_max)/(G.l_min-G.l_max)) : 3;
	*/
	ArcLabel(X,Y,Out Int,Out L,1) Assign L=G.v[X-1][Y-1] If (G.l_max>G.l_min);
	ArcOffset(X,Y,Out K,1) Assign K=5 ;
	ArcColor(X,Y,Out C,1) Assign C=
	(G.v[X-1][Y-1]<G.l_max/6) ? Black :
	(G.v[X-1][Y-1]<2*G.l_max/6) ? DarkBrown :
	(G.v[X-1][Y-1]<3*G.l_max/6) ? Brown :
	(G.v[X-1][Y-1]<4*G.l_max/6) ? DarkRed :
	(G.v[X-1][Y-1]<5*G.l_max/6) ? Red :
	Orange;
/*---------------------------------------	Visualization of the current best solution */
	Graph(Out 2); 
	Directed(2);
	
	Node(Out I,2) For I:InRange(I,1,G.N); /* Generate ID numbers, in sequence */
	
	Arc(X,Y,2) If best_matrice.v[X-1][Y-1]>=0; 
	ArcThickness(X,Y,Out S,2)  Assign S=2  If (best_matrice.v[X-1][Y-1]>=0 && best_matrice.v[X-1][Y-1]<=G.l_max);
	ArcThickness(X,Y,Out S,2)  Assign S=1 If best_matrice.v[X-1][Y-1]>G.l_max;
	ArcStyle(X,Y,Out Dashed,2) If best_matrice.v[X-1][Y-1]>G.l_max;
	ArcLabel(X,Y,Out Int,Out L,2) Assign L=best_matrice.v[X-1][Y-1] If (best_matrice.v[X-1][Y-1]>=0 && best_matrice.v[X-1][Y-1]<=G.l_max)  ;
	

**/

