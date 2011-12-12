

/*------------------------------------------------------------------- ALEA_VELOCITY */
struct velocity	alea_velocity(int N)
{
int				i;
struct velocity vt;

//vt.size=alea(1,N);
vt.size=N;

for (i=0;i<vt.size;i++)
	{
	vt.comp[i][0]=alea(0,G.N-1);
	vt.comp[i][1]=alea_diff(0,G.N-1,vt.comp[i][0]);
	}
//display_velocity(vt);
return vt;
}

/*------------------------------------------------------------------- AUTO_MOVE */
struct particle auto_move(struct particle p,int type, int monotony, int levelling,int trace)
/* The particle moves slowly, looking for a better position */
{
int 			i,j,k;
int				levelling0;
struct particle	pt,pt1;
struct velocity v;

if (trace>1) 
	printf("\n Auto-move for particle %i. Size %i \n",p.rank+1,p.x.size);
pt=p;


if (type==0) // Lazy Descent Method
	{
	for (i=0;i<G.N;i++) // Move at random. 
		{
		pt.v=alea_velocity(1);
		pt=pos_plus_vel(pt,pt.v,monotony,1,levelling);

		if(pt.f<p.f) // Stop as soon as a better position is found
			{
			return pt;
			}
		}
	if (trace>1) {printf("\n auto move => no move ");display_position(p,1);}
	return p;	
	}
	
if (type==1) // Energetic
	{
	next_step1:
	for (i=0;i<G.N;i++) // Move at random
		{
		pt.v=alea_velocity(1);
		pt1=pos_plus_vel(pt,pt.v,monotony,1,levelling);

		if(pt1.f<pt.f) // Move as long as a better position is found
			{
			pt=pt1;
			goto next_step1;
			}
		}
	return pt;	
	}	
	
	
if (type==2) /* Move as long as a better position is found
				Check all immediate physical neighbours. If one is better, move, and check again
			*/
	levelling0=levelling;
if (type>2)
	levelling0=1;
	
	{
	v.size=1;
	next_step2:
	for (j=0;j<G.N-1;j++)  // For each neighbour
		{
		v.comp[0][0]=j;
		for (k=j+1;k<G.N;k++) 
			{
			v.comp[0][1]=k;
			pt1=pos_plus_vel(pt,v,monotony,1,levelling);
				
			if((levelling==0 && pt1.f<pt.f) || (levelling>0 && f_levelling(G,pt,pt1)<pt.fl)) // Move towards the best neighbour
				{
				if (trace>1) 
					{printf("\n auto move => ");display_position(pt,1);}
				pt=pt1;
				goto next_step2; 
				}
			}
		}
	return pt;
	}		
}

/*------------------------------------------------------------------- BEST_NEIGHBOUR */
struct particle	best_neighbour(struct swarm sw1,struct particle p,int k, int queens, int hood_type, int monotony,int equiv_v,int trace)
{
/* Find the k nearest particles of p, including p itself, in the swarm sw1, 
and find the best one according to the function value (f) 
OR
compute the "queen" of the hood, and take her as the best neighbour

*/

struct particle bestt;
float			coeff;
float			dmin, dmax;
float			dist[Nmax];
int				i,j;
int				rank_dmin[Nmax];
float			kx;
float			weight1;

if (queens==1) goto queen;

if (hood_type==1) goto physical_hood;

// Case hood_type==0 (social hood)
/*--------------------- Neighbourhood. Super simple method: nearest by the rank */
/* Note: normally, this "social" hood should become a "physical" hood during the process */

kx=k;
if (best_hood_type==0)
	{
	bestt=p;

	for (i=1;i<kx/2;i++)
		{
		j=p.rank+i;
		if (j>sw1.size-1) {j=j-sw1.size;};
	
		if(sw1.p[j].f<bestt.f)
			{
			bestt=sw1.p[j];
			}
		}
	for (i=1;i<kx/2;i++)
		{
		j=p.rank-i;
		if (j<0) {j=sw1.size+j;};
	
		if(sw1.p[j].f<bestt.f)
			{
			bestt=sw1.p[j];
			}
		}
	}


if (best_hood_type==1)
	{
	bestt=previous_best(p,0);

	for (i=1;i<kx/2;i++)
		{
		j=p.rank+i;
		if (j>sw1.size-1) {j=j-sw1.size;};
	
		if(sw1.p[j].best_f<bestt.f)
			{
			bestt=previous_best(sw1.p[j],0);
			}
		}
	for (i=1;i<kx/2;i++)
		{
		j=p.rank-i;
		if (j<0) {j=sw1.size+j;};
	
		if(sw1.p[j].f<bestt.f)
			{
			bestt=previous_best(sw1.p[j],0);
			}
		}
	}
	
if (trace>2)
	{
	printf("\n Best neighbour of %i is %i",p.rank+1,bestt.rank+1);
	}	

return bestt;


/*--------------------- Physical hood */
physical_hood:

dmax=0;
	for (i=0;i<sw1.size;i++) // Compute distances p => particle i (including particle p itself
		{
		//if (i==p.rank) {dist[i]=1.2; continue;} // Note. distance can't be > 1
	
		if (i==p.rank) {dist[i]=0; continue;}
		dist[i]=distance(p,sw1.p[i]);
		if (dist[i]>dmax) dmax=dist[i];
		}	

	for (i=0;i<k;i++)
		{
		dmin=1.1*dmax;
		for (j=0;j<sw1.size;j++)  // Look for the k nearest (including p itself)
			{
			if (dist[j]<dmin)
				{
				dmin=dist[j];
				rank_dmin[i]=j;
				}
			
			}
		dist[rank_dmin[i]]=1.1;
		}
		
if (best_hood_type==0)
	{	
	bestt=p;	
	for (i=0;i<k;i++) // Take the best of the k nearest (including p itself)
		{
		if (sw1.p[rank_dmin[i]].f<bestt.f)
			bestt=sw1.p[rank_dmin[i]];
		}
	}
	
if (best_hood_type==1)
	{	
	bestt=previous_best(p,0);	
	for (i=0;i<k;i++) // Take the best previous best of the k nearest (including p itself)
		{
		if (sw1.p[rank_dmin[i]].best_f<bestt.f)
			bestt=previous_best(sw1.p[rank_dmin[i]],0);
		}
	}	
	
return bestt;

/*---------------------- Queen*/
queen:
bestt=p; 
weight1=1/(sw1.p[0].f+1);// We are looking for a MINIMUM

for (i=1;i<k;i++) // Compute the "queen" of the hood ...
	{
	
	j=p.rank+i;
	if (j>sw1.size-1) {j=j-sw1.size;};
	coeff=1/(sw1.p[j].f+1);
	weight1=weight1+coeff;
	coeff=coeff/weight1;

	
	/*
	weight1=f(G,bestt,-1,-1,eval_f);
	weight1=1/(weight1+1);
	weight2=1/(sw1.p[j].f+1);
	coeff=weight2/(weight1+weight2);
	*/
	
	//printf("\ncoeff %f",coeff);
	bestt.v=coeff_pos_minus_pos(sw1.p[j],bestt,coeff,monotony,equiv_v,trace);// ... as the "gravity" center
	bestt=pos_plus_vel(bestt,bestt.v,0,1,0);
	}
	
	bestt.rank=-1; // Arbitrary value, so that it can't be said it is the same particle as any other



return bestt;
}


/*------------------------------------------------------------- COEFF_POS_MINUS_POS */
struct velocity	coeff_pos_minus_pos(struct particle p1,struct particle p2,float coeff,int monotony,int equiv_v, int trace)
	/* Applying v to p2 gives p1, or, in short, p2+v=p1, or v=p1-p2 
	and, after coeff*v
	monotony = 1  => we have p1.f<=p2.f and we want, for each intermediate position p(t), 
							p(t).f<=p(t-1).f
	monotony = 2  => coeff*v is computed in order to keep a "monotony" according to the distance
						between the two particles
	
	*/
{

int 			i,j,k,n1,nt;
int				GN;
struct particle pt;
struct velocity vt;
float			d0,d;



if (trace>2) printf("\n\n coeff_pos_minus_pos, particles %i and %i",p1.rank+1,p2.rank+1);

if (p1.rank>=0 && (p1.rank==p2.rank)) {vt.size=0; return vt;}; // If it is the same particle, of course the velocity is null



vt.size=0;
pt=p2;
GN=p2.x.size;

if (trace>2)
	{
	printf("\n coeff_pos_minus_pos, GN %i",GN);
	display_position(p1,0);
	display_position(p2,0);
	}


if (pt.x.s[0]!=0) pt.x=rotate(pt.x,0); /* To be sure the particle description begins on node 1 */
if (p1.x.s[0]!=0) 
	{
	printf("\n ERROR. coeff_pos_minus_pos. The particle %i doesn't begin by 1",p1.rank);
	return vt;
	}

for (i=1;i<GN-1;i++) // For each node of pt, at rank i ...
	{
	nt=pt.x.s[i];
	n1=p1.x.s[i];
	for (j=i;j<GN;j++) // ... look for its rank in p1...
			{
			if (p1.x.s[j]==nt) // ... finds it at rank j
				{
				if (j!=i) // .. if it is not the same rank ...
					{
					pt.x.s[i]=n1; // ... change the node in pt ...
					vt.comp[vt.size][0]=n1;
					
								// ... then, looks for the node of p1 in pt
					for (k=i+1;k<GN;k++)
						{
						if (pt.x.s[k]==n1)
							{
							pt.x.s[k]=nt;
							vt.comp[vt.size][1]=nt;
							vt.size=vt.size+1;

							if (vt.size>Vmax)
								goto stop;
								
							goto next_node;
					
							}
						}
					printf("\n ERROR. Don't find %i after rank %i in particle %i",n1,i+1, pt.rank);
					return vt;
					}
				}
			}
	next_node:;
	}
	
if (trace>2) {printf ("\n vt before coeff");display_velocity(vt);printf("\n");}
vt=coeff_times_vel(vt,coeff,equiv_v,trace);

//printf("\n monotony %i",monotony);

if (trace>2) {printf ("\n vt after coeff");display_velocity(vt);printf("\n");}

if (monotony!=2) 
	return vt;

//----------------------==========================================	
	d0=distance(p2,p1); // Initial distance, with no coeff
if (d0==0) return vt;
if (coeff==1) return vt;

//printf("\n     dist. %f",d0);
if (coeff<1)
	{
	update_coeff:
	
	pt=pos_plus_vel(p2,vt,0,1,0);
	d=distance(pt,p1);
	if (d<=d0) // OK for monotony (decreasing distance)
		{
		return vt;
		}
	vt.size=vt.size+1;
//printf("\n  vt_size %i,   dist. %f",vt.size,d);	
	
	goto update_coeff;
	
	}

if (coeff>1)
	{
	
	update_coeff2:
	
	pt=pos_plus_vel(p2,vt,0,1,0);
	d=distance(pt,p1);
	if (d>=d0) // OK for monotony (increasing distance)
		{
		return vt;
		}
	vt.size=vt.size-1;
//printf("\n  vt_size %i,   dist. %f",vt.size,d);	
	
	goto update_coeff2;
	}


//---------------
stop:	
/* Too many transpositions */

printf("\n *** WARNING. More than %i components (variable Vmax) for velocity of particle %i",Vmax, pt.rank);
printf("\n I have to 'cut' it");
return vt;

}

/*------------------------------------------------------------------- COEFF_TIMES_VEL */	
struct velocity	coeff_times_vel(struct velocity v,float coeff, int equiv_v,int trace)
{
float			coefft;
int				i;
struct velocity vt,vt0;


if (v.size==0) return v;


vt=v;
if (coeff==0) {vt.size=0;return vt;};

coefft=coeff;

if (coeff<0)
	{
	// Inversion
	
	for (i=0;i<v.size;i++)
		{
		vt.comp[i][0]=v.comp[v.size-i-1][0];
		vt.comp[i][1]=v.comp[v.size-i-1][1];
		coefft=-coeff;
		}
	
	}

//printf("\n coeff %f",coefft);

 if (coefft==1) 
		return equiv_vel(v,equiv_v,trace);

if (coefft<1)
	{
	//vt.size=MAX(1,coefft*v.size);
	vt.size=coefft*v.size;
	return vt;
	}


//  coefft >1
vt0=vt;
for (i=0;i<coefft;i++)
	{
	vt=vel_plus_vel(vt,vt0,0,trace);
	}

vt.size=MIN(coefft*v.size,Vmax);
	
if (equiv_v>0) vt=equiv_vel(vt,equiv_v,trace);

//printf("\n coeff_times_vel, vt.size %i",vt.size);

return vt;
}

/*------------------------------------------------------------------- 	COMPARE_PARTICLE */
int	compare_particle(struct particle p1,struct particle p2)
{
/*  p1 = p2  => 0
    p1 # p2  => 1  */

int	i;
int GN;
GN=p1.x.size;

for (i=0;i<GN;i++)
	{
	if (p1.x.s[i]!=p2.x.s[i]) return 1;
	}
	
if (p1.v.size!=p2.v.size) return 1;	
	
for (i=0;i<p1.v.size;i++)
	{
	if (p1.v.comp[i][0] != p2.v.comp[i][0]) return 1;
	if (p1.v.comp[i][1] != p2.v.comp[i][1]) return 1;
	}	
	
return 0;

}

/*------------------------------------------------------------------- DISTANCE */
float distance(struct particle p1,struct particle p2) // Distance function of common arcs
{
int	i,j;
int GN;
int n1,n2;
float sim;
struct velocity v;
float x;

GN=p1.x.size;

if (type_dist==0)  // "Distance" depending on the number of common arcs
	{
	sim=0;

	for (i=0;i<GN;i++)
		{
		n1=p1.x.s[i]; // Select an arc in p1
		n2=p1.x.s[i+1];
		for (j=0;j<GN;j++) // Check if it is in p2
			{
			if(p2.x.s[j]!=n1) goto next_node;
			if(p2.x.s[j+1]!=n2) goto next_node;
				sim=sim+1;
			next_node:;
			}
		}
	
	x=1-sim/GN;
	}
	
if (type_dist==1) // True distance 
	{
	v=coeff_pos_minus_pos(p1,p2,1,monotony,2,trace);
	x=v.size;
	}
	
return x;

}

/*------------------------------------------------------------------- EQUIV_VEL */
struct velocity equiv_vel(struct velocity v,int equiv_v,int trace)
{
/* Compute an equivalent and hopefully smaller velocity 
*/
struct particle p0,pt;
struct velocity vt;

if (equiv_v==0) return v;

// Reference point
if (equiv_v==1) p0=init;
if (equiv_v==2) p0=best_best;
if (equiv_v==3) p0=extra_best;

p0.rank=-1;
pt=pos_plus_vel(p0,v,0,-1,0); // Move
vt=coeff_pos_minus_pos(pt,p0,1,0,0,trace); // Re-evaluate the velocity


if (trace>1)
	{
	if (vt.size!=v.size)
		{
		printf("\n equiv_vel. Initial v.size %i. Final v.size %i",v.size,vt.size);
		}
	}

if (vt.size<v.size)
	return vt;
else
	return v;
}




/*------------------------------------------------------------------- F_LEVELLING */
float	f_levelling(struct graph G, struct particle p1, struct particle p2)
{ /* Compute a temporary different function value, in order to 
  improve the convergence
p1 = particle to move
p2 = neighbour
*/

float			f_l;
int				i,j;
float			min1;
struct particle pt;
struct velocity vt;

if (p2.f<p1.f) return p2.f;

vt.size=1;
min1=p2.f;


for (i=0;i<G.N-1;i++)
	{
	for (j=i+1;j<G.N;j++)
		{
		vt.comp[0][0]=i; // Move 1 step
		vt.comp[0][1]=j;
		pt=pos_plus_vel(p2,vt,0,0,0); /* For each neighbour of p ...
								... looks for the best neighbour
								*/
		if (pt.f<min1) 
			min1=pt.f;

		}
	}


f_l=(min1+p1.f)/2;	 // Take the mean of the two bests

//printf("\n p1.f %f, p2.f %f, min1 %f, f_levelling %f",p1.f,p2.f,min1,f_l);

return f_l;
}




/*------------------------------------------------------------------- MOVE_TOWARDS */
struct particle	move_towards(struct particle p,struct particle pg,struct coeff c,int move_type,int explicit_vel,int conv_case,int equiv_v,int trace)
{
/*  

Basic equations for Particle Swarm Optimization (particular case with just one phi value)

		v(t+1) =alpha*v(t) +(beta*phi/2)*(pi-x) + (beta*phi/2)*(pg-x)			Equ. 1
		x(t+1) = x(t) + gamma*v(t) + ((1-(delta-eta*phi))/2)*(pi-x)
												((1-(delta-eta*phi))/2)*(pg-x)			Equ. 2
				
pi:  previous best position of the particle
pg:  global best position (best particle in the neighbourhoud)

or

		v(t+1) =alpha*v(t) +(beta*phi)*(pit-x)				Equ. 1
		x(t+1) = pit + gamma*v(t) + (eta*phi-delta)*(pit-x)	Equ. 2
pit=(pi+pg)/2

or

		v(t+1) =alpha*v(t) +(beta*phi)*(pit-x)						Equ. 1
		x(t+1) = x(t) + gamma*v(t) + (1-(delta-eta*phi))*(pit-x)	Equ. 2		

*/

struct particle pi;
struct particle pit,pgt;
struct particle	pt;

float			d1,d2;
float			alpha, beta, gamma, delta, eta;
float			phi;
int				tot_vel;
struct velocity	v1,v2,v3;
float			z0,z1;


if (trace>1) 
	printf("\n\n move particle %i",p.rank+1);
if (trace>2)
	display_position(p,1);

tot_vel=0;

pi=previous_best(p,0); // Generate the previous best particle

alpha=c.c[0];
//alpha=alea_float(0,c.c[0]);
beta=c.c[1];
gamma=c.c[2];
delta=c.c[3];
eta=c.c[4];
//phi=alea_float(0,c.c[5]);
phi=c.c[5];
z0=beta*phi;
z1=(1-(delta-eta*phi));

if (conv_case==5) delta=(1-alpha)*phi;
if (conv_case==6) delta=1+(1-alpha)*phi;

pt.rank=p.rank;
//printf("\n coeffs %f %f %f %f %f %f",alpha, beta,gamma,delta,eta,phi);

if (explicit_vel>0) goto explicit_velocities;

v1=coeff_pos_minus_pos(pg,p,beta*phi,monotony,equiv_v,0);
pt=pos_plus_vel(p,v1,0,1,0);
tot_vel=tot_vel+v1.size;

goto end;
//----------------------
explicit_velocities:

if (move_type==1) goto shunt1;
if (move_type==2) goto shunt2;
if (move_type==3) goto shunt3;
if (move_type==4) goto shunt4;
if (move_type==5) goto shunt5;

//------------------------------ move_type=0
pt.v=coeff_times_vel(p.v,alpha,equiv_v,trace);
if (trace>2) {display_velocity(pt.v);printf(" alpha*v");}

v1=coeff_pos_minus_pos(pi,p,z0/2,monotony,equiv_v,trace);//	(beta*phi/2)*(pi-x)
if (trace>2) {display_velocity(pt.v);printf("(beta*phi/2)*(pi-x)");}

pt.v=vel_plus_vel(pt.v,v1,equiv_v,trace);
 
 
v1=coeff_pos_minus_pos(pg,p,z0/2,monotony,equiv_v,trace);//	(beta*phi/2)*(pg-x)
if (trace>2) {display_velocity(v1);printf(" (beta*phi/2)*(pg-x)");}

pt.v=vel_plus_vel(pt.v,v1,equiv_v,trace);//  New VELOCITY


//-----------


v1=coeff_times_vel(p.v,gamma,equiv_v,trace);// gamma*v
pt=pos_plus_vel(p,v1,0,1,0);


v1=coeff_pos_minus_pos(pi,p,z1/2,monotony,equiv_v,trace);//z1*(pi-x)
pt=pos_plus_vel(pt,v1,0,1,0);


v1=coeff_pos_minus_pos(pg,p,z1/2,monotony,equiv_v,trace);//z1/2*(pg-x)
pt=pos_plus_vel(pt,v1,0,1,0); // new POSITION

goto end;

//-------------------------
shunt1:
pt.v=coeff_times_vel(p.v,alpha,equiv_v,trace);
if (trace>2) {display_velocity(pt.v);printf(" alpha*v");}
pt=pos_plus_vel(p,pt.v,0,1,0);
tot_vel=tot_vel+pt.v.size;

pit=pos_plus_vel(pi,pt.v,0,1,0);
pgt=pos_plus_vel(pg,pt.v,0,1,0);


v1=coeff_pos_minus_pos(pit,pt,z0/2,monotony,equiv_v,trace);//	(beta*phi/2)*(pi-x)
if (trace>2) {display_velocity(pt.v);printf("(beta*phi/2)*(pi-x)");}

pt.v=vel_plus_vel(pt.v,v1,equiv_v,trace);
pt=pos_plus_vel(pt,v1,0,1,0);
tot_vel=tot_vel+v1.size;

pgt=pos_plus_vel(pgt,v1,0,1,0);

pgt.rank=Max_size;
v1=coeff_pos_minus_pos(pgt,pt,z0/2,monotony,equiv_v,trace);  //	(beta*phi/2)*(pg-x)
if (trace>2) {display_velocity(v1);printf(" (beta*phi/2)*(pg-x)");}

pt.v=vel_plus_vel(pt.v,v1,equiv_v,trace);//  New VELOCITY


//-----------


v1=coeff_times_vel(p.v,gamma,equiv_v,trace);// gamma*v
pt=pos_plus_vel(p,v1,0,1,0);
tot_vel=tot_vel+v1.size;

pit=pos_plus_vel(pi,v1,0,1,0);
pgt=pos_plus_vel(pg,v1,0,1,0);

pit.rank=Max_size;
v1=coeff_pos_minus_pos(pit,pt,z1/2,monotony,equiv_v,trace); //z1/2*(pi-x)
pt=pos_plus_vel(pt,v1,0,1,0);
tot_vel=tot_vel+v1.size;

pgt=pos_plus_vel(pgt,v1,0,1,0);
pgt.rank=Max_size;
v1=coeff_pos_minus_pos(pgt,pt,z1/2,monotony,equiv_v,trace); //z1/2*(pg-x)
pt=pos_plus_vel(pt,v1,0,1,0); // new POSITION
tot_vel=tot_vel+v1.size;

goto end;

//-----------------------
shunt2:
pt.v=coeff_times_vel(p.v,alpha,equiv_v,trace);
if (trace>2) {display_velocity(pt.v);printf(" alpha*v");}

v1=coeff_pos_minus_pos(pg,pi,0.5,monotony,equiv_v,trace); // (pg-pi)/2
pit=pos_plus_vel(pi,v1,0,1,0); // (pg+pi)/2;
tot_vel=tot_vel+v1.size;

pit.rank=Max_size;
v1=coeff_pos_minus_pos(pit,p,beta*phi,monotony,equiv_v,trace);//	(beta*phi)*((pg+pi)/2 - x)
if (trace>2) {display_velocity(pt.v);printf("(beta*phi)*((pg+pi)/2 - x)");}
 
v2=vel_plus_vel(pt.v,v1,equiv_v,trace);//  New VELOCITY

//-----------


v1=coeff_times_vel(p.v,gamma,equiv_v,trace);// gamma*v
pt=pos_plus_vel(pit,v1,0,1,0); // (pg+pi)/2 + gamma*v
tot_vel=tot_vel+v1.size;

v1=coeff_pos_minus_pos(pit,p,eta*phi-delta,monotony,equiv_v,trace);//(eta*phi-delta)*((pg+pi)/2 - x)

pt=pos_plus_vel(pt,v1,0,1,0); // new POSITION
pt.v=v2;
tot_vel=tot_vel+v1.size;

goto end;

//----------------------------------------------------------------
shunt3:  // This one is the nearest one of  "theoretical" PSO
if (trace>2) printf("\n move_type %i",move_type);
/*
if (pg.rank<0) printf("\n pg.rank %i",pg.rank);
if (pi.rank<0) printf("\n pi.rank %i",pi.rank);
*/

v1=coeff_times_vel(p.v,alpha,equiv_v,trace);
if (trace>2) {display_velocity(v1);printf("%f*v",alpha);}

v3=coeff_pos_minus_pos(pg,pi,0.5,monotony,equiv_v,trace); // (pg-pi)/2

pit=pos_plus_vel(pi,v3,0,1,0); // (pg+pi)/2;
pit.rank=Max_size; // Arbitrary rank, so that coeff_pos_minus_pos doesn't give 0

v3=coeff_pos_minus_pos(pit,p,beta*phi,monotony,equiv_v,trace);//	(beta*phi)*((pg+pi)/2 - x)
if (trace>2) {display_velocity(v3);printf("(beta*phi)*((pg+pi)/2 - x)");}
 
v2=vel_plus_vel(v1,v3,equiv_v,trace);//  New velocity

if (trace>2) {display_velocity(v2);printf("v(t+1)");}
//-----------

if (conv_case==5) /* (Cf convergence_case). In this case, x(t+1)=(pg+pi)/2 + v(t+1)
					Just to speed up a bit the process */
	{
	pt=pos_plus_vel(pit,v2,0,1,0); // new POSITION
	pt.rank=p.rank;
	pt.v=v2; // New VELOCITY
	tot_vel=tot_vel+v2.size;
	goto end;
	}
	
if (conv_case==6) // x(t+1) = x(t) + v(t+1) 
	{
	pt=pos_plus_vel(p,v2,0,1,0); // new POSITION
	pt.rank=p.rank;
	pt.v=v2; // New VELOCITY
	tot_vel=tot_vel+v2.size;
	
	goto end;
	//       Some tests
	
	d1=distance(p,pg);
	d2=distance(pt,pg);
	if (d2>d1)
		{
		printf("\n Warning. Distance to best hood increased. %.0f => %.0f",d1,d2);
		
		}
	
	if (v2.size==0)
	{
	printf("\n particle doesn't move. alpha= %f, beta= %f, phi= %f",alpha,beta,phi);
	printf("\n velocity size %i",p.v); display_position(p,0);
	printf("\nprevious best"); display_position(pi,0);
	printf("\nbest hood"); display_position(pg,0);
	printf("\n(pg+pi)/2");display_position(pit,0);
	//v2=coeff_pos_minus_pos(pit,p,1,monotony,equiv_v,3);
	printf("\n((pg+pi)/2 - x)");display_velocity(v2);
	printf("\n(beta*phi)*((pg+pi)/2 - x)");display_velocity(v3);
	printf("\n");
	}
	
	
	goto end;
	}

v1=coeff_times_vel(p.v,gamma,equiv_v,trace);// gamma*v

v3=coeff_pos_minus_pos(pit,p,eta*phi-delta,monotony,equiv_v,trace);//(eta*phi-delta)*((pg+pi)/2 - x)
v1=vel_plus_vel(v1,v3,equiv_v,trace); 

pt=pos_plus_vel(pit,v1,0,1,0); // new POSITION

pt.v=v2; // New VELOCITY
tot_vel=tot_vel+v2.size;


goto end;

//----------------------------
shunt4: /* This one is the nearest one "classical" PSO
			*/
v1=coeff_times_vel(p.v,alpha,equiv_v,trace);
v3=coeff_pos_minus_pos(pg,pi,0.5,monotony,equiv_v,trace); // (pg-pi)/2
pit=pos_plus_vel(pi,v3,0,1,0); // (pg+pi)/2;
pit.rank=Max_size;
tot_vel=tot_vel+v3.size;
v3=coeff_pos_minus_pos(pit,p,beta*phi,monotony,equiv_v,trace);//	(beta*phi)*((pg+pi)/2 - x)

v2=vel_plus_vel(v1,v3,equiv_v,trace);//  New velocity


v1=coeff_times_vel(p.v,gamma,equiv_v,trace);// gamma*v
v3=coeff_pos_minus_pos(pit,p,z1,monotony,equiv_v,trace); //(1-(delta-eta*phi))*(pit-x)
v3=vel_plus_vel(v1,v3,equiv_v,trace);
pt=pos_plus_vel(p,v3,0,1,0); // x(t+1)  new POSITION
tot_vel=tot_vel+v3.size;

pt.v=v2; // New VELOCITY

goto end;

// ---------------------------------
shunt5:  // Move towards the best of pi or pg

v1=coeff_times_vel(p.v,alpha,equiv_v,trace);

if (pi.f<pg.f) pit=pi; else pit=pg;

v3=coeff_pos_minus_pos(pit,p,beta*phi,monotony,equiv_v,trace);
v2=vel_plus_vel(v1,v3,equiv_v,trace);//  New velocity

//-----------

if (conv_case==5) /* (Cf convergence_case). I this case, x(t+1)=best_of_(pi,pg) + v(t+1)
					Just to speed up a bit the process */
	{
	pt=pos_plus_vel(p,v2,0,1,0); // new POSITION
	pt.v=v2; // New VELOCITY
	tot_vel=tot_vel+v2.size;
	goto end;
	}

v1=coeff_times_vel(p.v,gamma,equiv_v,trace);// gamma*v

v3=coeff_pos_minus_pos(pit,p,eta*phi-delta,monotony,equiv_v,trace);

pt=pos_plus_vel(pit,v1,0,1,0); // new POSITION
tot_vel=tot_vel+v1.size;

pt.v=v2; // New VELOCITY

goto end;


//--------------------------
end:


if (pt.x.s[0]!=0) pt.x=rotate(pt.x,0); /* To be sure the particle description begins on node 1 */


if (pt.f<p.best_f)  /* Save best previous position */
	{
	pt.best_x=pt.x; // Change to the current position
	pt.best_f=pt.f;
	}
else
	{
	pt.best_x=p.best_x; // Don't change
	pt.best_f=p.best_f;
	}

 pt.rank=p.rank;

if (tot_vel>0) effective_move=1; else effective_move=0;
return pt;
}

/*-------------------------------------------------------------------POS_PLUS_VEL */
struct particle	pos_plus_vel (struct particle p,struct velocity v, int monotony,int type,int levelling)
{
/* Move a particle according to its velocity
	particle + velocity => new particle
	
Modify: particle p
		eventually, extra_best
*/


float			ft[Vmax+1];
int				i,i1;
int				last_trans;
int				n1,n2;
struct particle	pt,ptemp;
int				rank1,rank2;

struct velocity	vtemp;

if (v.size==0) return p;

ptemp=p;
if (type>=0)
	{
	ft[0]=p.f;
	}
	
if (trace>2)
	{
	printf("\n pos_plus_vel, particle before:");
	if (type>=0) display_position(ptemp,1); else display_position(ptemp,0);
	display_velocity(v);
	}
//printf("\n vsize %i",v.size);

for (i=0;i<v.size;i++)
	{
	n1=v.comp[i][0]; // Nodes to swap
	n2=v.comp[i][1];
	for (i1=0;i1<G.N+1;i1++)
		{
		if (ptemp.x.s[i1]==n1) 
			{
			rank1=i1;
			if (rank1==G.N) rank1=0;
			ptemp.x.s[i1]=n2;
			}
		else
			{
			if (ptemp.x.s[i1]==n2)
				{
				rank2=i1;
				if (rank2==G.N) rank2=0;
				ptemp.x.s[i1]=n1;
				}
			}
		}
		

		if (type>0)// Just modif
			{
			ptemp.f=f(ptemp,rank1,rank2);
			
			if (step==1 && ptemp.f<ptemp.best_f)
				{
				ptemp.best_x=ptemp.x;// Memorize the best previous position
				ptemp.best_f=ptemp.f; 
				}
			if (monotony>0) ft[i+1]=ptemp.f;
			if(ptemp.f<extra_best.f) 
				{
				extra_best=ptemp;
				}
			}
			
	}
		
	if (type==0)
		{
		ptemp.f=f(ptemp,-1,-1); // Complete
			
		if (step==1 && ptemp.f<ptemp.best_f)
			{
			ptemp.best_x=ptemp.x;// Memorize the best previous position
			ptemp.best_f=ptemp.f; 
			}
		if (monotony>0) ft[i+1]=ptemp.f;
		if(ptemp.f<extra_best.f)
			{ 
			extra_best=ptemp;
			}	
		}
//---------------------
if (monotony==0)
	{
	pt=ptemp;
	goto end;
	}
//----------------------	
if (monotony==1) /* Slow down to keep the monotony on f value
					Note that monotony=1 supposes f(p)>=f(p+v)
					*/
	{
	pt=p;
	last_trans=v.size;
	vtemp.size=0;
	back:
	if (ft[last_trans]>ft[0])
		{
		if (trace>3) printf("\n %f > %f => back",ft[last_trans],ft[0]);
		last_trans=last_trans-1;
		vtemp.comp[vtemp.size][0]=v.comp[last_trans][0];
		vtemp.comp[vtemp.size][1]=v.comp[last_trans][1];
		vtemp.size=vtemp.size+1;
		goto back;
		}
		if (trace>3) display_velocity(vtemp);		
		pt=pos_plus_vel(ptemp,vtemp,0,1,levelling); // Back
		goto end;
	}
	

	
printf("\n  ERROR in pos_plus_vel. Wrong value %i for monotony",monotony);
return p;	
	
//---------------
end:	
if (pt.x.s[0]!=0) pt.x=rotate(pt.x,0);
if (extra_best.x.s[0]!=0) extra_best.x=rotate(extra_best.x,0);

if (trace>2)
	{
	printf("\n            particle after:");
	display_position(pt,1);
	}
return pt;	
	

}


/*------------------------------------------------------------------- 	PREVIOUS_BEST */
struct particle previous_best(struct particle p, int type)
{
struct particle pt;

if (type==0) // Generate a particle equal to the previous best
pt=p;
pt.x=p.best_x;
pt.f=p.best_f;
if (pt.x.s[0]!=0) pt.x=rotate(pt.x,0);
return pt;

// if (type==1)
// Memorize a previous best position in a particle structure

pt=p;
pt.best_x=p.x;
pt.best_f=p.f;
return pt;
 
}

/*------------------------------------------------------------------- 	TAX_NO_ARC */
float tax_no_arc(int l,struct graph G)
	{
	/*
	Penalize the l-th unexistent arc
	*/
	
	float 	tax=1.1; /* >=1 */
	float 	x;

	//x=tax*G.l_max+(G.N-l)*(G.l_max-G.l_min);
	x=tax*G.l_max+(G.N-1)*(G.l_max-G.l_min);
	
	return x;
	
	}




/*------------------------------------------------------------------- 	VEL_PLUS_VEL */
struct velocity	vel_plus_vel(struct velocity v1,struct velocity v2, int equiv_v,int trace) /* v1 _then_ v2 */
{
int				i;
struct velocity vt;

vt=v1;

vt.size=MIN(v1.size+v2.size,Vmax);

for (i=v1.size;i<vt.size;i++)
	{
	vt.comp[i][0]=v2.comp[i-v1.size][0];
	vt.comp[i][1]=v2.comp[i-v1.size][1];
	}

if (equiv_v>0) vt=equiv_vel(vt,equiv_v,trace);
return vt;
}