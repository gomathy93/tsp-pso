/**
/*-------------------------------------- Particle visualization */
Graph(Out 3); 
	Directed(3);
	
	Node(Out I,3) For I:InRange(I,1,G.N); /* Generate ID numbers, in sequence */
	
	Arc(X,Y,3) If particle_matrice.v[X-1][Y-1]!=0; 
	ArcLabel(X,Y,Out Int,Out L,3) Assign L=G.v[X-1][Y-1];
	ArcThickness(X,Y,Out S,3)  Assign S=2  If particle_matrice.v[X-1][Y-1]<=G.l_max;
	ArcThickness(X,Y,Out S,3)  Assign S=1 If particle_matrice.v[X-1][Y-1]>G.l_max;
	ArcStyle(X,Y,Out Dashed,3) If particle_matrice.v[X-1][Y-1]>G.l_max;
	ArcLabel(X,Y,Out Int,Out L,3) Assign L=G.v[X-1][Y-1] If particle_matrice.v[X-1][Y-1]<=G.l_max ;
	


/*--------------------------------------- 	Topo visualization */
/*	
	View(Out 4); 
	Point(Out P,Out X,Out Y,4)
	 For P:InRange(P,0,sw1.size)
		Assign X=20+200*topo[P][0] Y=250*topo[P][1];
	PointSize(P,Out S,4) Assign S=5;
	PointShape(P,Out Round,4);
*/

	// PointColor(P,Out C,4) Assign C=Red If (a_queen==1 && P==sw1.size); 


**/