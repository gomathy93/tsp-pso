#include "stdinc.h"

//------------------------------------------------------------------- ALEA_VELOCITY 
struct velocity	alea_velocity(int N)
{
int				i;
struct velocity vt;

vt.size=alea(1,N);

for (i=0;i<vt.size;i++)
	{
	vt.comp[i][0]=alea(0,G.N-1);
	vt.comp[i][1]=alea_diff(0,G.N-1,vt.comp[i][0]);
	}
//display_velocity(vt);
return vt;
}

// ------------------------------------------------------------------- AUTO_MOVE 
struct particle auto_move(struct particle p,int type, int monotony, int levelling,int trace)
/* The particle moves slowly, looking for a better position */
{
int               b,d,g;
int               dd,gg;
//double         best_before;
double         delta1,delta2,delta_min;
double         eval_f0;
int 			i,j,k;
int               improv;
//int				levelling0;
int                  loop,loop_max;
int               node;
struct particle	pt,pt1,pt2;
struct velocity v;

if (trace>1) 
	printf("\n Auto-move type %i for particle %i. Size %i \n",type,p.rank+1,p.x.size);
pt=p;
 if (pt.x.s[0]!=0) pt.x=rotate(pt.x,0);
 if (pt.best_x.s[0]!=0) pt.best_x=rotate(pt.best_x,0);

 // loop_max=G.N;
 loop_max=1;
if (type==0) // Lazy Descent Method  (random)
	{
      for(loop=0;loop<loop_max;loop++)
      {
	for (i=0;i<G.N;i++) // Move at random. 
		{
		pt.v=alea_velocity(1);
		pt=pos_plus_vel(pt,pt.v,monotony,1,levelling);

		if(pt.best_f<p.best_f) // Stop as soon as a better position is found
			{
			goto return_pt;
			}
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
		pt.v=alea_velocity(1);  // Just one transposition (switch)
		pt1=pos_plus_vel(pt,pt.v,monotony,1,levelling);

		if(pt1.best_f<pt.best_f) // Move as long as a better position is found
			{
			pt=pt1;
			goto next_step1;
			}
		}
	goto return_pt;	
	}	
	


 // type=2,  3, 5, 6, 7
 if (trace>1) printf("\n   transpositions");

/* 
				Check all immediate physical neighbours.
            If one is better, choose the best, and move
            if (type=3, 5, 6, 7 ) check again
*/

	v.size=1;
 
	next_step2:
    improv=0;
    delta_min=pt.best_f-target;
     
	for (j=0;j<G.N-1;j++)  // For each neighbour
	{
		v.comp[0][0]=j;
		for (k=j+1;k<G.N;k++) 
		{
			v.comp[0][1]=k;
			pt1=pos_plus_vel(pt,v,monotony,1,levelling);
				
               delta1=pt1.best_f-target;
               if( delta1<delta_min)
               {
                  if (trace>1)                     
					{printf("\n auto move => ");display_position(pt1,1);}
                  delta_min=delta1;
                  pt=pt1;  // Memorize the best new position
                  improv=1;
                  if (delta_min<=0) goto end_type2; // By any chance ...
			}
            }
	}

      if (improv==1 &&(type==3 || type==5 || type==6 || type==7)) // Move as long as a better position is found
      {
         goto next_step2;
      }   

             if (type==6) goto three_opt;
             if (type==7) goto three_opt_weak;
             
  end_type2:

	goto return_pt;

 //----------------------------------------------------------------------------------------------------------------
 three_opt:
   if(pt. best_time<splice_time ) goto return_pt;
 if (trace>1)
    printf("\n auto_move, 3-opt");
 
 three_opt_loop:
   // b=>g => d=> b, so  (b,d,g) => (g,d,b)
   // or
 // b=> d=>g=>b, so (b,d,g) => (d,g,b)
 
  delta_min=pt.best_f-target; // Maximum possible improvement

   // for (b=0;b<G.N;b++)
   for (b=0;b<G.N-2;b++)
    {
       //for (d=b+1;d<b+G.N;d++)
       for (d=b+1;d<G.N-1;d++)
       {
         //dd=d%G.N;
         //if (dd==b) continue;
          //for (g=dd+1;g<dd+G.N;g++)
          //for (g=dd+1;g<dd+G.N;g++)
          for (g=d+1;g<G.N;g++)
          {
         //gg=g%G.N;
         // if (gg==b) continue;
          //if (gg==dd) continue;

          pt1=pt; pt1.x=pt.best_x; // Try to directly modify the best previous position
          pt2=pt1;

          node=   pt1.x.s[b];
          pt1.x.s[b]=pt1.x.s[g];
          pt1.x.s[g]=pt1.x.s[d];
           pt1.x.s[d]=node;
           eval_f0=eval_f;
           pt1.f=f(pt1,-1,-1);
          eval_f=eval_f0+6.0/G.N; // This move could be done by just two transpositions
                                              // so it wouldn't be fair to count a complete evaluation                                   
           node= pt2.x.s[b];
           pt2.x.s[b]=pt2.x.s[d];
          pt2.x.s[d]=pt2.x.s[g];
           pt2.x.s[g]=node ;
           eval_f0=eval_f;
          pt2.f=f(pt2,-1,-1);
          eval_f=eval_f0+6.0/G.N;
          
                 delta1= pt1.f-target;
                  delta2= pt2.f-target;
                   if (MIN(delta1,delta2)>=delta_min) goto next_g;    // No improvement

                  if (delta1<delta2)
                  {
                  p1:
                  pt=pt1;
                  delta_min=delta1;
                  goto end_improve;
                  }

                  if (delta2<delta1)
                  {
                  p2:
                  pt=pt2;
                  delta_min=delta2;
                  goto end_improve;
                  }

                if (delta1==delta2)
                {
                  if (alea(0,1)==0) goto p1; else goto p2;
                }

           end_improve:

    next_g:;
           } // g
       } // d
    }  // b

           if(pt.f<pt.best_f)
           {
              pt.best_x=pt.x;
             pt.best_f=pt.f;
              pt.best_time=time;
             if (move[4]>=3) BB_update(pt);
              goto  three_opt_loop;   // Try again
           }

    goto return_pt;


//-----------------------------------------------------------------------------------------------------
 three_opt_weak:    // Only three _consecutive_ nodes
  if(pt. best_time<splice_time ) goto return_pt;
  
 if (trace>1)
    printf("\n auto_move, 3-opt_weak");
    
// three_opt_weak_loop:

     delta_min=pt.best_f-target; // Maximum possible improvement
   // (b,b+1,b+2) => (b+2,b,b+1)
   // or
 // (b,b+1,b+2) => (b+1,b+2,b)
    for (b=0;b<G.N;b++)
    {
       dd=(b+1)%G.N;
       gg=(dd+1)%G.N;

       pt1=pt; pt1.x=pt.best_x; // Try to directly modify the best previous position
       pt2=pt1;
       
          node=   pt1.x.s[b];
          pt1.x.s[b]=pt1.x.s[gg];
          pt1.x.s[gg]=pt1.x.s[dd];
           pt1.x.s[dd]=node;
           eval_f0=eval_f;
           pt1.f=f(pt1,-1,-1);
          eval_f=eval_f0+6.0/G.N; // This move could be done by just two transpositions
                                              // so it wouldn't be fair to count a complete evaluation
  
          node=   pt2.x.s[b];
          pt2.x.s[b]=pt2.x.s[dd];
          pt2.x.s[dd]=pt2.x.s[gg];
           pt2.x.s[gg]=node ;
           eval_f0=eval_f;
          pt2.f=f(pt2,-1,-1);
          eval_f=eval_f0+6.0/G.N;

                  delta1= pt1.f-target;
                  delta2= pt2.f-target;
                   if (MIN(delta1,delta2)>=delta_min) goto next_b_2;    // No improvement
                   
                  if (delta1<delta2)
                  {
                  pt_1:
                  pt=pt1;
                  delta_min=delta1;
                  goto end_improve_2;
                  }
                  
                  if (delta2<delta1)
                  {
                  pt_2:
                  pt=pt2;
                  delta_min=delta2;
                  goto end_improve_2;
                  }                 

                if (delta1==delta2)
                {
                  if (alea(0,1)==0) goto pt_1; else goto pt_2;
                }
                
           end_improve_2:      
               if (trace>1)
                  printf("\n3-opt_weak =>%f",pt.f); 
  
    next_b_2:;
		}  // next b
 
  if (pt.x.s[0]!=0) pt.x=rotate(pt.x,0);
     if(pt.f<pt.best_f)
     {
             pt.best_x=pt.x;
             pt.best_f=pt.f;
              pt.best_time=time;
             if (move[4]>=3) BB_update(pt);
 //             goto  three_opt_weak_loop;  // Try again
     }

  return_pt:
  //if (pt.x.s[0]!=0)
  pt.x=rotate(pt.x,0);
 //if (pt.best_x.s[0]!=0)
 pt.best_x=rotate(pt.best_x,0);
 pt.v=coeff_pos_minus_pos(pt,p,1,monotony,equiv_v,trace);
 return pt;

}

/*------------------------------------------------------------------- BEST_NEIGHBOUR */
struct particle	best_neighbour(struct swarm sw1,struct particle p,int k, int hood_type, int monotony,int equiv_v,int trace)
{
/* Find the k nearest particles of p, including p itself, in the swarm sw1, 
and find the best one according to the function value (f) 

*/

struct particle bestt={0};
double			dmin, dmax;
double			dist[Nmax];
int				i,j;
int				rank_dmin[Nmax];
double			kx;

if (hood_type==1) goto physical_hood;

// Case hood_type==0 (social hood)
/*--------------------- Neighbourhood. Super simple method: nearest by the rank */
/* Note: normally, this "social" hood should become a "physical" hood during the process */

kx=k;
if (best_hood_type==0)  // Best neighbour  in terms of current position
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


if (best_hood_type==1)   // Best neighbour in terms of best previous position
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
 //if (bestt.rank==p.rank) printf("\n Particle %i is its own best neighbour",p.rank);
 
return bestt;


/*--------------------- Physical hood */
physical_hood:

dmax=0;
	for (i=0;i<sw1.size;i++) // Compute distances p => particle i (including particle p itself
		{
		//if (i==p.rank) {dist[i]=1.2; continue;} // Note. distance can't be > 1
	
		if (i==p.rank) {dist[i]=0; continue;}
		dist[i]=distance(p,sw1.p[i],type_dist);
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
}



/*------------------------------------------------------------- COEFF_POS_MINUS_POS */
struct velocity	coeff_pos_minus_pos(struct particle p1,struct particle p2,double coeff,int monotony,int equiv_v, int trace)
	/* Applying v to p2 gives p1, or, in short, p2+v=p1, or v=p1-p2 
	and, after coeff*v
	monotony = 1  => we have p1.f<=p2.f and we want, for each intermediate position p(t), 
							p(t).f<=p(t-1).f
	monotony = 2  => coeff*v is computed in order to keep a "monotony" according to the distance
						between the two particles

      equiv_v 	>0 = try to find a smaller equivalent velocity. 0 = do nothing. Cf equiv_vel()
	
	*/
{

int 			i,j,k,n1,nt;
int				GN;
struct particle pt;
struct velocity vt;
double			d0,d;

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
	printf("\n ERROR. coeff_pos_minus_pos. The first node of the particle %i is %i. Must be 1",p1.rank,1+p1.x.s[0]);
   return vt;
	}
   
 // Progressively update pt, in order to obtain p1
 
for (i=1;i<GN-1;i++) // For each node of pt, at rank i ...
	{
	nt=pt.x.s[i];
	n1=p1.x.s[i];
   if (n1==nt) continue; // It's OK
   
	for (j=i+1;j<GN;j++) // ... look for its rank in p1...
			{
			if (p1.x.s[j]==nt) // ... find it at rank j
				{
					pt.x.s[i]=n1; // ... change the node in pt ...
					vt.comp[vt.size][0]=n1; // Set the first part of the velocity component
					
								// ... then, look for the node of p1 in pt
					for (k=i+1;k<GN;k++)
					{
						if (pt.x.s[k]==n1)
							{
							pt.x.s[k]=nt;// Change the node
							vt.comp[vt.size][1]=nt;  // Set the second part of the velocity   component
							vt.size=vt.size+1;                                               

							if (vt.size>Vmax)
								goto stop;
								
							goto next_node;
					
                                    }
					} // next k
					printf("\n ERROR. coeff_pos_minus_pos. Don't find node %i after rank %i in particle %i",n1+1,i+1, pt.rank+1);
 
                          printf("\n particle pt"); display_position(pt,0) ;
                           printf("\n particle p2"); display_position(p2,0);
                            printf("\n particle p1"); display_position(p1,0);
                            display_velocity(vt);
                           return vt;
			} // end if (p1.x.s[j]==nt)
            }   // next j
	next_node:;
	} // next i
	
if (trace>2) {printf ("\n vt before coeff");display_velocity(vt);printf("\n");}
vt=coeff_times_vel(vt,coeff,equiv_v,trace);

//printf("\n monotony %i",monotony);

if (trace>2) {printf ("\n vt after coeff");display_velocity(vt);printf("\n");}

if (monotony!=2) 
	return vt;

//----------------------==========================================	
	d0=distance(p2,p1,type_dist); // Initial distance, with no coeff
if (d0==0) return vt;
if (coeff==1) return vt;

//printf("\n     dist. %f",d0);
if (coeff<1)
	{
	update_coeff:
	
	pt=pos_plus_vel(p2,vt,0,1,0);

	d=distance(pt,p1,type_dist);
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
	d=distance(pt,p1,type_dist);


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
struct velocity	coeff_times_vel(struct velocity v,double coeff, int equiv_v,int trace)
{
double			coefft;
int				i;
int               new_vsize;
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


 new_vsize=  coefft*v.size;
 if (new_vsize>Vmax)
 {
    vt.size=Vmax;
    printf("\nWARNING. Vmax is too small. Should be at least%i",new_vsize);
 }
	
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
double distance(struct particle p1,struct particle p2,int type_dist) // Distance function of common arcs
{
int	i,j;
int GN;
int n1,n2;
double sim;
struct velocity v;
double x=0;

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
	v=coeff_pos_minus_pos(p1,p2,1,0,2,trace);
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
//if (equiv_v==3) p0=extra_best;

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
double	f_levelling(struct graph G, struct particle p1, struct particle p2)
{ /* Compute a temporary different function value, in order to 
  improve the convergence
p1 = particle to move
p2 = neighbour
*/

double			f_l;
int				i,j;
double			min1;
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

double         c1,c2,c3;
double			d1,d2;
double			alpha, beta, gamma, delta, eta;
double			phi;
int				tot_vel;
struct velocity	v1,v2,v3;
double			z0,z1;


if (trace>1) 
	printf("\n\n move particle %i",p.rank+1);

if (trace>2)
	display_position(p,1);

tot_vel=0;

pi=previous_best(p,0); // Generate the previous best particle
if (p.x.s[0]!=0) p.x=rotate( p.x,0);

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
if (move_type==6) goto shunt6;
if (move_type==7) goto shunt7;
if (move_type==8) goto shunt8;
if (move_type==9) goto shunt9;
if (move_type==10) goto shunt10;

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


v1=coeff_pos_minus_pos(pi,p,z1/2,monotony,equiv_v,trace);//z1/2*(pi-x)
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
shunt3:  // This one is the nearest one to  "theoretical" PSO
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
	
	d1=distance(p,pg,type_dist);
	d2=distance(pt,pg,type_dist);
	if (d2>d1)
		{
		printf("\n Warning. Distance to best hood increased. %.0f => %.0f",d1,d2);
		
		}
	
	if (v2.size==0)
	{
	printf("\n particle doesn't move. alpha= %f, beta= %f, phi= %f",alpha,beta,phi);
	printf("\n velocity size %i",p.v.size); display_position(p,0);
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
shunt4: /* This one is the nearest one to "classical" PSO
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



shunt6: // "Spherical" distribution  around pg

   v2.size=distance(p,pg,type_dist);   // Radius
   v1.size=distance(pi,pg,type_dist);
   v1.size=MAX(v1.size,v2.size);
if (v1.size==0)
{
   //printf("\n Null velocity for particle %i", p.rank) ;
   effective_move=0;
   return p;
}
   v1=alea_velocity(v1.size);
   pt=pos_plus_vel(pg,v1,0,1,0);
     pt.v=coeff_pos_minus_pos(pt,p,1,0,equiv_v,trace);  // pt.v=pt-p

goto end;

 shunt7: // "Spherical" distribution  around  (pi+pg)/2

     v1=coeff_pos_minus_pos(pi,pg,1,0,equiv_v,trace);  // v=pi-pg
       v2=coeff_times_vel(v1,0.5,equiv_v,trace);
   pt=pos_plus_vel(pg,v2,0,1,0);  //Center =pg+ (pi-pg)/2
   
   v1=alea_velocity(v1.size);  
   pt=pos_plus_vel(pt,v1,0,1,0);
   pt.v=coeff_pos_minus_pos(pt,p,1,0,equiv_v,trace);  // pt.v=pt-p . New velocity (not really necessary)

goto end;

 shunt8: // "Spherical"  non uniform distribution  around  a point between pi and pg

v1.size=distance(pi,pg,type_dist);   // Radius

if (v1.size==0)
{
   //printf("\n Null velocity for particle %i", p.rank) ;
   effective_move=0;
   return p;
}

 c1=1/pi.f;
 c2=1/pg.f;
 c3=c1+c2;
 //c1=c1/c3;  // Weight for pi
 c2=c2/c3;  // Weight for pg

   v1=alea_velocity(v1.size);
   pit=pos_plus_vel(pi,v1,0,1,0);      // A point around pi
   
   v1=alea_velocity(v1.size);
   pgt=pos_plus_vel(pg,v1,0,1,0);  // A point around pg

   v1=coeff_pos_minus_pos(pgt,pit,c2,0,equiv_v,trace); // c2*(pgt-pit)

   pt=pos_plus_vel(pit,v1,0,1,0); // New position  pit+ c2*(pgt-pit)
   pt.v=coeff_pos_minus_pos(pt,p,1,0,equiv_v,trace);  // New velocity (not really necessary) 


goto end;


shunt9: // "Spherical" distribution  around p, radius distance(pg,p) or distance(pi,p)

  	v1.size=distance(p,pg,type_dist);   // Radius
   v2.size=distance(p,pi,type_dist);
   v1.size=MAX(v1.size,v2.size);

if (v1.size==0)
{
   //printf("\n Null velocity for particle %i", p.rank) ;
   effective_move=0;
   return p;
}
   v1=alea_velocity(v1.size);
   pt=pos_plus_vel(p,v1,0,1,0);
     pt.v=coeff_pos_minus_pos(pt,p,1,0,equiv_v,trace);  // pt.v=pt-p
goto end;

shunt10: // "Spherical" distribution  around pi, radius distance(pi,pg) or distance(pi,p)

  	v1.size=distance(pi,pg,type_dist);   // Radius
   v2.size=distance(pi,p,type_dist);
   v1.size=MAX(v1.size,v2.size);
      //v1.size=MIN(v1.size,v2.size);
if (v1.size==0)
{
   //printf("\n Null velocity for particle %i", p.rank) ;
   effective_move=0;
   return p;
}
   v1=alea_velocity(v1.size);
   pt=pos_plus_vel(pi,v1,0,1,0);
     pt.v=coeff_pos_minus_pos(pt,p,1,0,equiv_v,trace);  // pt.v=pt-p
goto end;

//--------------------------
end:


 pt.x=rotate(pt.x,0); /* To be sure the tour description begins on node 1 */
 pt.best_x=rotate(pt.best_x,0);


if (pt.f<p.best_f)  /* Save best previous position */
	{
	pt.best_x=pt.x; // Change to the current position
	pt.best_f=pt.f;
       pt.best_time=time;
	}
else
	{
	pt.best_x=p.best_x; // Don't change
	pt.best_f=p.best_f;
       pt.best_time=p.best_time;
	}

 pt.rank=p.rank;

if (tot_vel>0) effective_move=1; else effective_move=0;
BB_update(pt); // Update the blackboard
return pt;
}

/*-------------------------------------------------------------------POS_PLUS_VEL */
struct particle	pos_plus_vel (struct particle p,struct velocity v, int monotony,int type,int levelling)
{
/* Move a particle according to its velocity
	particle + velocity => new particle
	
Modify: particle p
		//eventually, extra_best
*/

int                  found;
double			ft[Vmax+1];
int				i,i1;

int				last_trans;
int				n1,n2;
//int               node;
struct particle	pt,ptemp;
int				rank1=0,rank2=0;

struct velocity	vtemp;
// printf("\npos_plus_vel, v.size %i",v.size);

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

for (i=0;i<v.size;i++)
	{
	n1=v.comp[i][0]; // Nodes to swap
	n2=v.comp[i][1];
      found=0;
      for (i1=0;i1<G.N;i1++)
	{

		if (ptemp.x.s[i1]==n1)
         {
           rank1=i1;
           found=found+1;
           if (found==2) goto re_eval;
         }
 		if (ptemp.x.s[i1]==n2)
         {
           rank2=i1;
           found=found+1;
           if (found==2) goto re_eval;
         }                                                    
    }
  printf("\n ERROR. pos_plus_vel. Can't find node %i or node %i",n1, n2);        

  	
  re_eval:
  ptemp.x.s[rank1]=n2;   // was n1, becomes n2.
                                         // node no at rank1  _was_ at rank2
  ptemp.x.s[rank2]=n1;  // was n2, becomes n1
                                             // node no at rank2  _was_ at rank1
  
		if (type>0)// Just update the evaluation
			{
			ptemp.f=f(ptemp,rank1,rank2);  // nodes in ranks rank1 and rank2 have been switched
			
			if (step==1 && ptemp.f<ptemp.best_f)
				{
				ptemp.best_x=ptemp.x;// Memorize the best previous position
				ptemp.best_f=ptemp.f;
                      ptemp.best_time=time;
                      if (move[4]>=3) BB_update(ptemp);
				}
			if (monotony>0) ft[i+1]=ptemp.f;
			}
			
	}   // next i (velocity component)
		
	if (type==0)

		{
		ptemp.f=f(ptemp,-1,-1); // Complete  re evaluation

			
		if (step==1 && ptemp.f<ptemp.best_f)
			{
			ptemp.best_x=ptemp.x;// Memorize the best previous position
			ptemp.best_f=ptemp.f;
                ptemp.best_time=time;
                if (move[4]>=3) BB_update(ptemp);
			}
		if (monotony>0) ft[i+1]=ptemp.f;
	
		}
//---------------------
//printf("\npos_plus_vel,%f %f",p.best_f,ptemp.best_f);

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
pt.x=rotate(pt.x,0);
pt.best_x=rotate(pt.best_x,0);

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
 pt.best_time=time;
return pt;
 
}

//====================================================== SEQUENCE_SUBST
struct   particle    sequence_subst(struct particle p1, struct particle p2,int size_max)
{/*
 s1= position (or best position) of p1, s2=best position of p2

 In s2, look for a sub sequence ss2 (size>=4) that is globally similar to one (ss1) in s1
 and so that s1/ss1=>ss2 is a better sequence (smaller total  length regarding graph G).
  f1 is the total length of the sequence s1.

 return: the new better sequence, after substitution

 G is a global variable.
 subs_option is a globl variable
 WARNING: each sequence is supposed to be a permutation of the G nodes, beginning in 0
 plus again the node 0 at the end (cycle).
 */
  double f1,f2;
  float  GN2;
 int        i;
 int        ij;

 int        j;
 int        k;

 int        kl;
 int        l;

  struct  particle p1t;
 double  p1f;
 struct particle ptemp;
 struct  seq   s1,s2, ss1,ss2;
 int        sim;
 int        size;

 GN2=G.N/2;
 if (subst_option==0)
 {
    s1=p1.x;
    p1f=p1.f;
    }
  else
  {
     s1=p1. best_x;
     p1f=p1.best_f;
     if (p1.best_x.s[0]!=0) printf("\n WARNING. sequence_subst. Not node 0 at first place in best_x");
     if (p1.best_x.s[G.N]!=0) printf("\n WARNING. sequence_subst. Not node 0 at last place in best_x");
  }
  
 s2=p2.best_x;
   p1t=p1;
  size=4;

  loop:
    ss1.size=size;  ss2.size=size;

   for (i=0;i<G.N;i++)    // Try all subsequences of size "size" and G.N-size
   {
      for (j=0;j<size;j++) // Build a subsequence in s1, for particle p1
                                    // It begins on i in particle p1  
      {
           ij=(i+j)%G.N;
         ss1.s[j]=s1.s[ij];    
      }                                                                          

       for (k=0;k<G.N;k++)   // For the second particle ...
       {          
                  for (l=0;l<size;l++)   // ... build a subsequence in s2
                                                 // It begins on k in particle p2
                  {
                      kl=(k+l)%G.N;
                     ss2.s[l]=s2.s[kl];    
                  }
                  
                  sim=sequence_similar(ss1,ss2); // Compare the subsequences

                  if (sim==1)  // Globally the same
                  {    
                     // Check if it is better
                     ptemp.x=ss1;                                                        
                      f1=  f(ptemp,-1,-1);  // Warning. It  also adds the arc value ss1.s[size-1] => ss1.s[0]
                                                       // but it is not important, for it does exactly the same for f2.                  
                      ptemp.x=ss2;                 
                      f2=  f(ptemp,-1,-1);
   
                      if (f2<f1)  // Replace ss1 by ss2
                      {
                           for (j=0; j<size;j++)
                           {
                              ij=(i+j)%G.N;
                             if (subst_option==0) p1t.x.s[ij]=ss2.s[j];
                             else p1t.best_x.s[ij]=ss2.s[j];
                           }

                           if (subst_option==0) p1t.f=p1f-f1+f2;  // Update the f-value
                           else  p1t.best_f=p1f-f1+f2; 

                           goto end_subs; // It is not allowed to replace both the sequence
                                                   // _and_ its complementary: it would mean just replace
                                                   // p1.x (or p1.best_x) by p2.best_x. Not interesting.
                        }
      
              
                        if (p2.best_f-f2<p1f-f1) // The complementary susbsequence is better
                        {
                           if (subst_option==0) p1t.x=s2;
                           else   p1t.best_x=s2;
                           
                           for (l=0; l<size;l++)
                           {
                              kl=(k+l)%G.N;
                             if (subst_option==0) p1t.x.s[kl]=ss1.s[l]; // Keep ss1
                             else p1t.best_x.s[kl]=ss1.s[l]; 
                           }
                             if (subst_option==0) p1t.f=p2.best_f-f2+f1;   // Update the f-value
                             else  p1t.best_f=p2.best_f-f2+f1;
                           
                           goto end_subs;
                        }                 
                        goto end_sim;
                        
                    end_subs:
  
  
                    if (subst_option==0) p1t.x=rotate(p1t.x,0);
                     else p1t.best_x=rotate(p1t.best_x,0);
                     

                    // Stop as soon as there has been improvement
                      if (subst_option==0)
                      {
                         if (p1t.f<p1t.best_f)  // Memory
                         {
                               p1t.best_x=p1t.x;
                              p1t.best_f=p1t.f;
                               p1t.best_time=time;
                         }
                       }
                       
                         if(subst_option>0 && p1t.best_f<p1f)
                         {
                               p1t.best_time=time;
                         }
   
                         BB_update(p1t); // Update the blackboard
  
                            return p1t; // It has been improved
                       
                    end_sim:;

                    
                     }   // end sim==1
       } // next k
                     
   }    // next i

  size=size+1;
   if (size<=GN2) goto loop;  // Try a longer subsequence
   else
   {
     if (trace>1)
         printf("\n       sequence_subst. size [4...%i] ==> no improvement", size-1);
      return p1; // No possible improvement
   }

}

  //===================================================================== SPLICE_AROUND
struct   particle   splice_around(struct swarm sw, struct particle p, int hood_type,int hood_size)
 {
   /* The particle looks at all its neighbours (or the whole swarm),
   and tries to improve itself, by substituying a sequence in its position.

   "hood" is a global variable (neighbourhood size))
   "size_max" is global variable
    */

struct seq hood_rank;
int   i;
int   improv;
float  h2;
int   j;
int   k;
struct   particle pt={0};
int         size;

if (trace>1) printf("\n splice particle %i",p.rank);
  improv=0; 
if (splice_around_option==1) goto hood;

 // The whole swarm
 // Look, compare and eventually move
 if (splice_around_option==0 )
     size=sw.size;
 else  size=(G.N*splice_around_option)/100;  // Just a given rate of the swarm is checked
 
      for(k=0;k<size;k++)  // For each neighbour
      {
         if (k==p.rank) continue;
         //  If  comparing best_positions
         // and if none of the two best positions has changed since last splice_around call
         // => no need to check again.
         if(subst_option==1 && p. best_time<splice_time && sw.p[k].best_time<splice_time ) continue;
         
               pt=sequence_subst(p,sw.p[k],size_max);
               if (pt.f<p.f) improv=1;
               if(improv>0)
               {
                if (trace>1)
                   printf("  %f => %f (best= %f)",p.f,pt.f,pt.best_f);   
               return pt; // Stop asa there is some improvement
               }
          //if (pt.best_f<best_best.best_f) return pt; // Stop asa enough improvement
         //if (pt.best_f<p.best_f) return pt; // Stop asa there is improvement of the previous best
      }
if (trace>1 && improv==0) printf("=> NO improvement");
 return pt;
 
 //----------------------------------------------------------------------------------
  hood:
 // Case hood_type==0 (social hood)
//--------------------- Neighbourhood. Super simple method: nearest by the rank 
     if (hood_type!=0)
           printf("\nWARNING. splice_around: physical neighbourhood not yet written. I use social one");
    
// Build the neighbourhood
 k=0;
 h2=(float)hood_size/2;
	for (i=1;i<h2;i++)
		{
		j=p.rank+i;
		if (j>sw.size-1) {j=j-sw.size;}
         hood_rank.s[k]=j;
         k=k+1;
		}
	for (i=1;i<h2;i++)
		{
		j=p.rank-i;
		if (j<0) {j=sw.size+j;}
         hood_rank.s[k]=j;
         k=k+1;
		}

      hood_rank.size=k;
 // printf("\n hood_size %i, hood_rank.size %i",hood_size,k);
           //for (k=0;k<hood_rank.size;k++) printf("\n  hood_rank.s[%i]  %i",k,hood_rank.s[k]);

 // Look, compare and eventually move
      for(k=0;k<hood_rank.size;k++)
      {
        pt=sequence_subst(p,sw.p[hood_rank.s[k]],size_max);
               if (pt.f<p.f) improv=1;
               if(improv>0)
               {
                if (trace>01)
                   printf(" \n %f => %f (best= %f)",p.f,pt.f,pt.best_f);
               return pt; // Stop asa there is some improvement
               }
      }
if (trace>1 && improv==0) printf("\n => NO improvement");
  return pt;

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

//============================================================== BB_UPDATE
void BB_update(struct particle p)
{
 /*
	Update the blackboard. Must be used each time a particle modify its  position
	BB is a global variable.
	G is a global variable.
	BB_option is a global variable
	------------------------
	BB_option=0:
	For each arc (n1,n2) of x, the value f is added to BB[n1][n2][0]
	and 1 is added to BB[n1][n2][1].
	BB[n1][n2][0]/BB[n1][n2][1] is seen as the probable "cost" (penalty) of
	using the arc (n1,n2) in a tour.
	----------------------  
	BB_option=1:
	For each arc (n1,n2) of x, the value f replace the previous one if it is smaller,
	and 1 is added to BB[n1][n2][1].
	*/
 if (move[4]<3) return; // The blackboard won't be used. No need to update it!
 if (trace>1)
	printf("\n BB_update");
 int  i;
 int n1,n2;
 
 for (i=0;i<G.N;i++) // Warning. This suppose the last node is equal to the first one
 {
	n1=p.x.s[i];
	n2=p.x.s[i+1];
	if (BB_option==0)
	{
	 BB[0].v[n1][n2]=BB[0].v[n1][n2]+p.f;
	 BB[1].v[n1][n2]=BB[1].v[n1][n2]+1;
	}
	else
	{
	 if ( BB[1].v[n1][n2]>0 && p.f<BB[0].v[n1][n2])
	 {
		BB[1].v[n1][n2]=BB[1].v[n1][n2]+1;
		BB[0].v[n1][n2]=p.f;
	 }
	}
 }
 
}

//============================================================== BB_USE
struct particle BB_use(struct particle p)
{  /*
	Build a tour, using the blackboard
	For each arc (n,*) to add, make a probabilistic choice,
	according to the line n of BB
	*/
 
 double   big_value;
 int   candidate[Nmax];
 double   candidate_penalty[Nmax];
 int         candidate_nb;
 double   c_max;
 int      i;
 //int      j;
 //int      k;
 //int      m;
 int      node1,node2;
 double   penalty;
 struct particle pt;
 double  r;
 int  rank;
 int     used[Nmax]={0};
 
 big_value=G.N*G.l_max;
 
 pt=p;
 node1=0; // The new tour begins in 0
 rank=1;
 used[node1]=1;
 pt.x.s[node1]=0;
 
 //printf("\n");
 next_node:
	candidate_nb=0;
	for (node2=0;node2<G.N;node2++) // For each possible new node ...
 {
	if (used[node2]>0) continue;
	if(rank==G.N-1) goto set_node; // Last node => no choice, anyway
	
	// Ask the blackboard
	if (BB[1].v[node1][node2]==0)  penalty=big_value;
	else
	{
	 if (BB_option==0)
		penalty= BB[0].v[node1][node2]/BB[1].v[node1][node2] ;
	 else
		penalty= BB[0].v[node1][node2];
	}
	
	candidate_penalty[candidate_nb]=penalty;
	candidate[candidate_nb]=node2;
	candidate_nb=candidate_nb+1;
 }   // next node2
	
	if (candidate_nb==1) goto set_node;  // Fi there is just one possible node, add it
	
	// Choose the new node at random, according to penalties
	c_max=0;
	for (i=0;i<candidate_nb;i++) c_max=c_max+ candidate_penalty[i];
	for (i=0;i<candidate_nb;i++) candidate_penalty[i]=1-candidate_penalty[i]/c_max;//Proba
	for (i=1;i<candidate_nb;i++) candidate_penalty[i]=candidate_penalty[i-1]+candidate_penalty[i];  // Cumulate
	r=alea_float(0,candidate_nb-1); // random choice
	
	//printf("\n %i: ",candidate_nb);for (i=0;i<candidate_nb;i++) printf(" %f",  candidate_penalty[i]);
	
	for (i=0;i<candidate_nb; i++)
 {
	if(r<candidate_penalty[i])
	{
	 node2=candidate[i];
	 goto set_node;
	}
 }
 set_node:
	//printf("\n r %f, i %i node2 %i",r,i,node2);
	//printf(" %i",node2);
	pt.x.s[rank]=node2;  // Add the node
	used[node2]=1;   //  remember it is already used
	rank=rank+1;
	if (rank<G.N) goto next_node;
	
	pt.x.s[G.N]=pt.x.s[0]; // Complete the tour
	pt.f=f(pt,-1,-1); // evaluate
	if (pt.f<pt.best_f)  // eventually update the best previous position
 {
	pt.best_x=pt.x;
	pt.best_f=pt.f;
	BB_update(pt); // Update the blackboard
 }
	if (trace>1)
 {
	display_position(pt,1);
	display_position(pt,2);
 }
	BB_update(pt); // Update the blackboard
	return pt;
}
