#include "stdinc.h"

/***************************************************************************
 main.c  -  description
 -------------------
 begin                : mar fév  4 21:06:16 UTC 2003
 copyright            : (C) 2003 by 
 email                : Maurice.Clerc@WriteMe.com
 last update      : 2003-03-11
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/* Looking for a minimum Hamiltonian cycle in a graph (Traveling Salesman Problem)
 using Particle Swarm Optimization
 
 Description
 -----------
 Yet_another_TSP_algorithm. In order to solve the (badly*) so called Traveling Salesman Problem,
 this program uses Particle Swarm Optimization.
 If there is _no_ Hamilton cycle, it usually finds at least a Hamilton _path_.
 If there is even no Hamilton path, it usually finds the longest path (in terms of node number).
 Due to the nature of PSO, you can't be _sure_ to obtain the best solution,
 but quite often you have at least a good result quite rapidly.
 
 *For a real Traveling Salesman (TS), the problem is to find the shortest way to see all towns.
 No matter if he has to go twice through a given town. Example:
 three towns A, B, C.  AB=1 BC=1 CA= 3.
 The only Hamilton cycle is ABCA=5, as for a real TS, a better solution is in fact ABCBA=4.
 
 
 Download
 --------
 Well, you have it, havn't you ?
 For more information and last "public" version see
 http://www.mauriceclerc.net, Math stuff for PSO, Discrete PSO
 
 EXPLANATION
 ===========
 In classical PSO, a  quite general system of equations is,
 for each particle
 
 v(t+1) = alpha*v(t) +(beta*phi)*(pig-x)					Equ. 1
 x(t+1) = pig + gamma*v(t) + (delta-eta*phi)*(pig-x)		Equ. 2
 
 pi:  previous best position of the particle
 pg:  globel best position (best particle in the neighbourhoud)
 pig: (pi+pg)/2
 
 v(t) is the "velocity" at time step t
 x(t) is the "position" at time step t
 
 alpha and phi are coefficients defining how each particle "trusts" itself and its neighbours.
 Note: in the most general case, you have phi1 for pi and phi2 for pg.
 
 This version contains also some "spherical" options, for the aim is to merge it in TRIBES,
 which is a parameter free PSO.
 
 
 Graph-particles
 --------------------
 G is a valuated graph with N nodes n_i
 
 (for visualization, it is better to have integer values. No arc <=> negative value )
 
 The position of a particle is defined as a sequence of N+1 nodes:
 x=(x_1,..., x_N+1),
 with x_1=x_N+1. It means a particle is a cycle, valid or not (some arcs x_i=>x_i+1 may be not in the graph G)
 
 What could be the "velocity" ?  If we note that "applying" a velocity to a position we find another position,
 we can define a velocity as a list of consecutive transpositions (node i <=> node j)
 
 v=( (n_1,n_j),... (n_i,n_k),... (n_N,n_l))
 
 In particular, it means that the "difference" between two particle is a velocity.
 
 Now, we have to define some operations :
 
 p + v				  	position + velocity => position	 		(subroutine pos_plus_vel)
 coeff*v					coeff*velocity => velocity				(subroutine coeff_times_vel)
 v + v'					velocity + velocity => velocity			(subroutine vel_plus_vel)
 
 
 There is also an underlying distance function, but we usually do not have to compute it,
 d(x-x'): (particle,particle) => real number
 
 Concerning the distance between two particles p1 and p2, all what we really need
 is to be sure that when p1 is moving "towards" p2, it is decreasing.
 
 
 Objective function   (subroutine f)
 -------------------
 
 We define a real function f on the set of particles so that its minimums
 are possible solutions (Hamiltonian cycles)
 
 Intuitively, if we consider a particle, the easiest way is to add the arc values of (x_i,x_i+1).
 Now, there is  a "tax" to add when this arc doesn't exist  					(subroutine tax_no_arc)
 
 The main difficulty is to find  an objective function f and a distance d
 so that they are correlated.
 That means if you move a particle p1 towards a particle p2 (decreasing d(p1,p2)),
 so, at least when we are "near" of p2, whe should have abs(f(p1)-f(p2)) also decreasing.
 
 --------
 In practice, the point is the way particles are moving (cf. move_towards())
 
 The "classical" PSO described above is just _one_ of the possible strategies used here 
 but a lot of other are also defined
 See in particular strategy 6 (hyperspherical distribution around pg),
 which is very similar to a pivot method
 
 Also, several other tools have been added, in particular:
 - a very simple greedy algorithm for initialization
 - auto_move  for local search
 - splice_around, to improve a position, asking the other particle
 - BB_use, to improve a position, using a blackboard method
 
 Compiling the program
 -------------------
 It is written in ANSI C, you shouldn't have any problem
 
 
 Performance
 --------------------
 Still quite bad, particularly for it is difficult to find a god parameter set for a given problem.
 However, again, this should be better after merged in TRIBES.
 Nevertheless, it certainly can't compete with very specific algorithms like LKH.
 */

/*
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <syscall.h>

// Define and structures
#include "def_and_struc.h"

/* Global variables */
#include "global_var.c"

/* Subroutines definitions */
#include "display.h"
#include "graph_tools.h"
#include "math_add.h"
#include "particle_tools.h"
#include "swarm_tools.h"

/* =============================== Subroutines ================================== */
#include "display.c"
#include "display_position_TSP.c"
#include "graph_tools.c"
#include "math_add.c"
#include "particle_tools.c"
#include "swarm_tools.c"

struct coeff convergence_case(float kappa, float phi);
struct particle arc_around(struct swarm sw, struct particle p);
double f(struct particle p,int rank1,int rank2);
#include "f_TSP.c"
/* =============================================================================== */

int main()
{
 FILE *f_trace;
 FILE *f_graph; // File containing the graph
 FILE *f_run; // To save swarm positions, for each iteration 

 int               auto_move_type;
 struct particle	best_hood; // Best particle in the neighbourhood 
 double         best_best_f;
 struct coeff	coeff;
 double         eps_max,eps_mean,eps_min;
 int               hamilt;
 int				i;
 int                  improv,improv_best;
 //int				iter,iter_min;
 //struct graph   G2;
 char			graph[80]; // Graph file name 
 int                  j;
 int                  k;
 // int                 m;
 //float             level;
 //int               loop_auto_move;
 float			max_eval;
 //int				max_iter;
 double         mean_eval;
 float			min_tour;
 int               move4;
 struct   particle new_p;
 int               norm_v;
 int               n_success;
 //int				parallel;
 int               run,run_max;
 int scanR;
 int				swarm_size;
 double            zzz,zzz2;
 
 /*----------------- SOME PARAMETERS -------------- */
 
#include "param.c"
f_trace=fopen("trace.txt","w");
 
 if (save==0) goto graph;
 /*-------------------------- */
 printf("\nChoose the file to save the run, please");
 scanR=scanf("%s",graph);
 
 f_run=fopen(graph,"w");
 
 /*-------------------------- */
 
 graph:
	
	/* Read the valuated graph  (Note: value 0 <=> no arc) */
	
	printf("\nFile name for the graph, please: ");
	scanR=scanf("%s",graph);
	
	//printf("\nMultiply all arc values by: ");
	//scanf("%f",&integer_coeff)  ;
	integer_coeff=1;
	
	f_graph=fopen(graph,"r");
	
	G=read_graph(f_graph,trace);
	
	//display_graph(G);
	/*  // Save the graph
	 for (i=0;i<G.N;i++)
	{
	 fprintf(f_trace,"\n");
	 for (j=0;j<G.N;j++) fprintf(f_trace,"%.0f ",G.v[i][j]);
	}
	 return EXIT_SUCCESS;
	 */
	
	printf("\n Just looking for Hamilton cycles  ? (y/n): ");
	scanR=scanf("%s",answer);
	
	if (answer[0]=='y')
 {
	G=TSP_to_Hamilton(G);
	return EXIT_SUCCESS;
 }
	
	G=graph_min_max(G); /* Compute the max and the min arc values, and the number of edges */
	min_tour=G.N*G.l_min;
	
	printf("\n Target  ? (suggestion %.0f) ",min_tour);
	scanR=scanf("%f",&target);
	
	/*------------------------------ */
 //graph_study:
	
	hamilt=check_Hamilton_cycle(G);
	if (hamilt==0) printf("\n\n WARNING. There is NO Hamiltonian cycle. I look for a Hamiltonian path\n");
	if(hamilt==1 || hamilt==2)printf("\n\n LUCKY YOU. There IS at least one Hamiltonian cycle. I look for the best one\n");
	if (hamilt==3) printf("\n I don't know whether there is a Hamiltonian cycle or not. Let's try");
	
 //parameters:
	
	printf("\n\n Default parameters");
	//swarm_size=G.N;  // Just a rule of thumb
	//zzz=0; for (i=2;i<G.N;i++) zzz=zzz+log(i);
	//swarm_size=2.46*zzz-55;   // Another rule of thumb (valid only if the result is >0
	 swarm_size=2*G.N+1;    // Another rule of thumb
	
	//swarm_size=400;
	
	printf("\nSwarm size ......... %i",swarm_size);
	swarm_size=MAX(1,MIN(swarm_size,Max_size));
	sw1.size=swarm_size;
	
	// see param.c
	printf("\n Init option................ %i",init_option);
	printf("\n Neighbourhood size.. %i ",hood);
	printf("\nHood_type .............%i",hood_type);
	printf("\nMove_type ............. %i",move[0]);
	if(move[0]<=5)
 {
	printf("\nKappa value ...... %.2f",kappa) ;
	printf("\nPhi value ............%.2f",phi);
 }
	printf("\nAuto_move_type ....%i",move[1]);
	printf("\nSplice_around the best %i",move[2]);
	printf("\nSplice_around more.... %i",move[3]);
	printf("\nRebuild method .......... %i",move[4]);
	
	//same_best_thresh=G.N/(2*hood); //Just a rule of thumb
	
	size_max=G.N;   // Max size for splice_around. Just a rule of thumb
	
	printf("\n Do you want to modify them? (y/n): ") ;
	scanR=scanf("%s",answer);
	if (answer[0]=='n')   goto end_param;
	
	
	// -----------------------------------Ask for parameters
	
	printf("\n Swarm size? (max = %i) ",Max_size);
	scanR=scanf("%i",&swarm_size);
	swarm_size=MAX(1,MIN(swarm_size,Max_size));
	sw1.size=swarm_size;
	
	printf("\n Init option? (suggested 1 or 2): ");
	scanR=scanf("%i",&init_option);
	
	printf("\n Neighbourhood size? (max = %i) ",sw1.size);
	scanR=scanf("%i",&hood);
	hood=MAX(1,MIN(hood,sw1.size));
	
	printf("\n Hood type? (0 = social (quick) 1 = physical (long)\n (suggestion: %i):  ",hood_type);
				 scanR=scanf(" %i",&hood_type);
				 
				 printf("\n Move type? (1 to 10) (suggestion: %i):  ",move[0]);
				 scanR=scanf("%i",&move[0]);
				 
				 if(move[0]<=5)
				 {
					printf("\n  kappa value? (suggestion: %.2f): ",kappa); // Constriction coefficients. See move_towards
					scanR=scanf("%f",&kappa);
					printf("\n  phi value? (suggestion: %.2f):  ",phi);
					scanR=scanf("%f",&phi);
				 }
				 
				 
				 printf("\n Auto-move type? (0 to 6) (suggestion: %i):  ",move[1]);
				 scanR=scanf("%i",&move[1]);
				 
				 printf("\nSplice_around the best?(suggestion  %i)",move[2]);
				 scanR=scanf("%i",&move[2]);
				 printf("\nSplice_around more? (suggestion %i)",move[3]);
				 scanR=scanf("%i",&move[3]);
				 printf("\nRebuild method? (suggestion %i)",move[4]);
				 scanR=scanf("%i",&move[4]);
				 
				 end_param:
					//-------------------------------- Iterations
					printf("\n Max tour evaluations? (0 => end)  ");
					scanR=scanf("%f",&max_eval);
					if (max_eval==0)   goto the_end;
					
					printf("\n Trace level  ? (0,1,2,3,4)  ");
					scanR=scanf("%i",&trace);
					
					/* ======================================================== HERE IS THE ALGORITHM */
					
					printf("\n How many times? ");
					scanR=scanf("%i",&run_max);
					if (run_max==0 ) goto the_end;
					run=1;
					n_success=0;
					eps_min=10000*target;
					eps_max=0;
					eps_mean=0;
					mean_eval=0;
					
					if (move[4]>=3) // Initialize the blackboard
				 {
					for (i=0;i<G.N;i++) for (j=0;j<G.N;j++) for (k=0;k<2;k++) BB[k].v[i][j]=0;
				 }
					
				 loop_run:
					printf("\n ___________________________ RUN %i",run);
					eval_f=0;
					time=0; // Current time step
					splice_time=0; // Last time splice_around has been used
					// Initialize the swarm 
					
					sw1=init_swarm(swarm_size,G,trace);
					
					printf("\nAfter init: eval %.0f  value %f for particle %i",eval_f, best_best.best_f,sw1.rank_best);
					if (best_best.best_f<=target) goto success;// Success
					
					if (save!=0) save_swarm(f_run,sw1); /* Save the run as text file */
					
					
					if(move[0]<=5 && move[0]!=0)
					coeff=convergence_case(kappa,phi);  // Coeffs for non spherical methods
					
				 moves:
					time=time+1;
					printf("\n total velocity %.0f",tot_vel(sw1));
					
					if (move[0]>0)
				 {
					//----------------------------------------------------------------------------- Normal move
					printf("\nEval %.0f. Normal move %i",eval_f,move[0]);
					if (eval_f>=max_eval)  goto end_max_eval;
					best_best_f=best_best.best_f;     // Before the move
					
					for (i=0;i<sw1.size;i++)
					{
					 // find the best in the neighbourhood (or the local queen) 
					 best_hood=best_neighbour(sw1,sw1.p[i],hood,hood_type,monotony,equiv_v,trace);
					 
					 // M O V E
					 sw1.p[i]=move_towards(sw1.p[i],best_hood,coeff,move[0],explicit_vel,conv_case,equiv_v,trace);
					 
					 if (sw1.p[i].best_f<best_best.best_f)// Check best of the best after the move 
					 {
						best_best=sw1.p[i];
						sw1.rank_best=i;
						printf("\neval %.0f  value %f",eval_f, best_best.best_f);
						display_position(best_best,1);
					 }
					 
					 if (best_best.best_f<=target) goto success;// Success
					 
					 if (best_best.best_f<best_best_f) goto moves; // Loop as soon as there is a global improvement
					 
					} // next i for normal move
				 }
					
					if (move[1]>0)
				 {
					//------------------------------------------------------------- If no improvement, auto_move
					printf("  /auto_move %i",move[1]);
					auto_move_type=move[1];
					//   auto_move:
					for (i=0;i<sw1.size;i++)
					{
					 sw1.p[i]= auto_move(sw1.p[i],auto_move_type,0,0,trace);
					 if (sw1.p[i].best_f<best_best.best_f)
					 {
						best_best=sw1.p[i];
						sw1.rank_best=i;
						if (best_best.best_f<=target)  goto success;// Success
						printf("\neval %.0f  value %f",eval_f, best_best.best_f);
						display_position(best_best,1);
						goto moves;
					 }
					 if (eval_f>=max_eval)  goto end_max_eval;
					} // next i for auto_move
					
				 }
					
					if(move[2]>0)
				 {
					// ---------------------------------------------------  If  still no improvement, splice_around best particle
					printf("  /splice_around, best particle");
					i= sw1.rank_best;
					sw1.p[i]= splice_around( sw1, sw1.p[i], hood_type,hood);
					
					if (sw1.p[i].best_f<best_best.best_f)// Best of the best after the move
					{
					 best_best=sw1.p[i];
					 if (best_best.best_f<=target)  goto success;// Success
					 printf("\neval %.0f  value %f",eval_f, best_best.best_f);
					 goto moves;
					}
				 }
					
					if(move[3]>0)
				 {            
					// ---------------------------------------------------  If  still no improvement, splice_around more particles
					printf(" /splice_around, more, option %i",splice_around_option);
					improv=0;
					improv_best=0;
					splice_time=time;
					
					for (i=0;i<sw1.size;i++)  // For each particle
					{
					 zzz= sw1.p[i].f;
					 zzz2= sw1.p[i].best_f;
					 sw1.p[i]= splice_around( sw1, sw1.p[i], hood_type,hood);
					 
					 if (sw1.p[i].f<zzz)    improv=improv+1;
					 if (sw1.p[i].best_f<zzz2)    improv_best=improv_best+1;
					 
					 if (sw1.p[i].best_f<best_best.best_f)// Best of the best after the move
					 {
						best_best=sw1.p[i];
						sw1.rank_best=i;
						if (best_best.best_f<=target)  goto success;// Success
						printf("\neval %.0f  value %f",eval_f, best_best.best_f);
						goto moves;
					 }
					 
					 if (eval_f>=max_eval)  goto end_max_eval;
					} // next i for splice_around
					
					// if (improv_best>sw1.size/2) goto moves;
				 }
					
					if (move[4]==9) move4=alea(1,3); else move4=move[4];
					if(move4>0)
				 {
					//--------------------------------- If still no improvement, rebuild  tours
					if (move4==1) printf("  /rebuild, random");
					if (move4==2) printf("  /rebuild, arc_around");
					if (move4==3) printf("  /rebuild, blackboard");
					
					for (i=0;i<sw1.size;i++)
					{
					 switch( move4)
					 {
						case 1:
						 norm_v=alea(1,G.N);
						 sw1.p[i].v=alea_velocity(norm_v);    // random velocity
						 sw1.p[i]=pos_plus_vel(sw1.p[i],sw1.p[i].v,0,0,0); //  => random new position
						 break;
						 
						case 2:
						 new_p=arc_around(sw1,sw1.p[i]);
						 sw1.p[i].v=coeff_pos_minus_pos(new_p,sw1.p[i],monotony,0,equiv_v,trace);
						 sw1.p[i]=new_p;
						 break;
						 
						case 3:   
						 new_p=BB_use(sw1.p[i]);
						 sw1.p[i].v=coeff_pos_minus_pos(new_p,sw1.p[i],monotony,0,equiv_v,trace);
						 sw1.p[i]=new_p;
						 break;
					 }
					 if (sw1.p[i].best_f<best_best.best_f)/* Best of the best after the move */
					 {
						best_best=sw1.p[i];
						if (best_best.best_f<=target)  goto success;// Success
						printf("\neval %.0f  value %f",eval_f, best_best.best_f);
						goto moves;
					 }
					 if (eval_f>=max_eval)  goto end_max_eval;
					}
				 }
					
					// Primitive rehope
					printf("\n Primitive rehope");
					for (i=0;i<sw1.size;i++)
				 {
					sw1.p[i].v=alea_velocity(G.N);
					sw1.p[i]= pos_plus_vel(sw1.p[i],sw1.p[i].v,0,0,0);
				 }
					
					goto moves;
					
					
					//-----------------------------------
				 // end_move:
					
					if (save!=0) save_swarm(f_run,sw1);
					if (trace>1) display_swarm(sw1);
					printf("\n Eval %.0f",eval_f);
					if (trace>0) display_position(best_best,2);
					
					goto moves;
					
				 end_max_eval:
					printf("\n\n Stop at max eval %.0f",eval_f);
					goto end;
					
				 success:
					n_success=n_success+1;
					printf("\n SUCCESS");
					
				 end:
					// Give the best position (sequence) found 
					
					printf("\n Best solution found after %.0f evaluations",eval_f);
					if (best_best.best_x.s[0]!=0) best_best.best_x=rotate(best_best.best_x,0);
					display_position(best_best,2);
					
					// Check if no error
					zzz=f(best_best,-1,-1);
					if (zzz!=best_best.best_f)
				 {
					printf("\n ERROR. The true f value is in fact %f ",zzz);
				 }
					zzz=best_best.best_f;
					if (zzz<eps_min) eps_min= zzz;
					if (zzz>eps_max) eps_max= zzz;
					eps_mean=eps_mean+zzz;
					mean_eval=mean_eval+eval_f;
					
					if (save!=0) save_swarm(f_run,sw1);
					save_swarm(f_trace,sw1);
					
					if (trace>1) display_swarm(sw1);
					
					run=run+1;
					if (run<=run_max) goto loop_run;
					else
				 {
					
					printf("\n With  swarm size=%i, max eval=%.0f => %i successes/%i. Rate=%.3f ",swarm_size,max_eval,n_success,run_max,(float)n_success/run_max);
					
					printf("\n Mean  %f [%f, %f]",eps_mean/run_max,eps_min,eps_max);
					mean_eval=mean_eval/run_max;
					printf("\n Mean eval %f",mean_eval);
					
					goto the_end;
				 }
					
				 //error_end:
					printf("\n Sorry, I give up");
					
				 the_end:
					return EXIT_SUCCESS;
}
/* ============================= Subroutines ================== */

/*------------------------------------------------------------------- 	CONVERGENCE_CASE */
// Useful only for "classical" non adaptive PSO. Will be removed when merged in TRIBES

struct coeff convergence_case(float kappa, float phi)
//*****************************General case
// Coefficients of the generalized iterative representation
// The matrix of the system is M
//       [alpha       beta*phi   ]
//       [-gamma  (delta-eta*phi)]       with phi=phi1+phi2
// NOTE: useful only for "classical" (non spherical) methods (see move_towards())
//********************************************
{
 float			Aclass2,Bclass2;
 struct coeff	coefft;
 float			disc;
 int				i;
 float			khi;
 float			maxne,maxneprim;
 float			maxneclass2;
 float			ne1class2,ne2class2;
 int scanR;
 
 
 casechoice:
	printf("\nWhich convergence case ? (0,1,2 3,4,5) (suggestion: 2): ");
	scanR=scanf("%i",&conv_case);
	
	if (conv_case<0 || conv_case>5)
 {
	printf("\n Wrong value. Le'ts try again");
	goto casechoice;
 }
	
	coefft.c[5]=phi;
	
	switch(conv_case)
 {
	case 0:  // So that x(t+1) = x(t) + v(t+1)
	 maxne=max_eig_basic(phi);
	khi=kappa/maxne;
	coefft.c[0] = khi;
	coefft.c[1] = khi;
	coefft.c[2] = khi;
	coefft.c[3] = (khi+1)*phi-1;
	coefft.c[4] = 1;
	break;
	
	
	
	case 1:  //(proved case)
	 maxne=max_eig_basic(phi);
	khi=kappa/maxne; //********* to change according to the syggestion given
	//by the program
	// khi=kappa/maxne for sure convergence
	coefft.c[0] = khi;
	coefft.c[1] = khi;
	coefft.c[2] = khi;
	coefft.c[3] = khi;
	coefft.c[4] = khi;
	break;
	
	case 2: //(just for phi>=4) //(proved case)
	 
	 if (phi>=4)
	{
	 disc=sqrt(phi*phi-4*phi);
	 Aclass2=3/2-(3/4)*phi+(3/4)*disc-(1/2)*(phi*phi)+(1/4)*(phi*phi*phi)-(3/4)*(phi*phi)*disc;
	 Bclass2=(1/4)*fabs(2-phi-2*(phi*phi)+phi*phi*phi+disc-3*phi*phi*disc);
	 ne1class2=fabs(Aclass2+Bclass2);
	 ne2class2=fabs(Aclass2-Bclass2);
	 maxneclass2=ne2class2;
	 if(ne1class2>ne2class2)
		maxneclass2=ne1class2;
	 
	 khi=kappa/maxneclass2;     //khi=kappa/maxneclass2 for sure convergence
	 
	 
	 coefft.c[3]=khi*(2-phi+disc)/2;
	 coefft.c[2]=khi*(2-phi+3*disc)/(4*phi);
	 coefft.c[0]=2*coefft.c[3];
	 coefft.c[1]=coefft.c[0];
	 coefft.c[4]=2*coefft.c[2];
	}
	else
	{
	 printf("\n For this case, you must have phi>=4. But here you have phi = %f",phi);
	 goto casechoice;
	}
	break;
	
	case 3: //(experimental case)
	 maxne=max_eig_basic(phi);
	khi=kappa/maxne;   //to begin, but not sure convergence
	printf("\n Initial khi: %f",khi);
	withkhi:
	 if(khi>almostzero)
	{
	 coefft.c[0]=1;
	 coefft.c[1]=1;
	 coefft.c[2]=khi;
	 coefft.c[3]=khi*khi;
	 coefft.c[4]=khi*khi;
	 maxneprim=max_eig(coefft,phi);
	 if (maxneprim>1)
	 {
		khi=0.9*khi;
		goto withkhi;
	 }
	}
	 else
	{
	 printf("\n No possible convergence");
	 printf("\n You should change some parameters (phi1, phi2)");
	 goto casechoice;
	}
	 printf("\n    Final khi (which is used): %f",khi);
	 break;
	 
	 case 4: //Called type 1' in the theoretical paper
		maxne=max_eig_basic(phi);
	 khi=kappa/maxne;
	 coefft.c[0] = khi;
	 coefft.c[1] = khi;
	 coefft.c[2] = 1;
	 coefft.c[3] = 1;
	 coefft.c[4] =1;
	 break;
	 
	 case 5:  // So that x(t+1) = p + v(t+1)
		maxne=max_eig_basic(phi);
	 khi=kappa/maxne;
	 coefft.c[0] = khi;
	 coefft.c[1] = khi;
	 coefft.c[2] = khi;
	 coefft.c[3] = (1-khi)*phi;
	 //coefft.c[3] = (1-khi)*phi/2;  When phi is random ?
	 coefft.c[4] = 1;
	 break;
	 
	default:
	 break;
 }
	
	
	printf("\n Coeffs:");
	for (i=0;i<5;i++)
	 printf(" %f", coefft.c[i]);
	
	return coefft;
}

//============================================================== ARC_AROUND
struct particle arc_around(struct swarm sw, struct particle p) 
{ // Build a tour, asking opinion of other particles for each arc
 double   big_value;
 int   candidate[Nmax];
 double   candidate_penalty[Nmax];
 int         candidate_nb;
 double   c_max;
 int      i;
 int      j;
 int      k;
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
 
 next_node:
	candidate_nb=0;
	for (node2=0;node2<G.N;node2++) // For each possible new node ...
 {
	if (used[node2]>0) continue;
	if(rank==G.N-1) goto set_node; // Last node => no choice, anyway
	
	penalty=0; 
	for (j=0;j<sw.size;j++) // ... ask opinions
	{
	 for (k=0;k<G.N-1;k++) // Find the arc in the current particle, if exists
		if (sw.p[k].best_x.s[k]==node1 && sw.p[k].best_x.s[k+1]==node2 )
	 {
		penalty=sw.p[k].best_f;
		goto next_opinions;
	 }
	 // Can't find it in the current particle
	 penalty=big_value;
	 
	 next_opinions:;
	}  
	// Mean value
	candidate_penalty[candidate_nb]=penalty/sw.size;
	candidate[candidate_nb]=node2;
	candidate_nb=candidate_nb+1;
 }   // next node2
	
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
	// printf("\n r %f, i %i node2 %i",r,i,node2);
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
	pt.best_time=time;
 }
	if (trace>1)
 {
	display_position(pt,1);
	display_position(pt,2);
 }
	BB_update(pt); // Update the blackboard  
	return pt;
}




