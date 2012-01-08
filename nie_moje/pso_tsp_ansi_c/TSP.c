#include "stdinc.h"

/***************************************************************************
                          TSP.c  -  description
                             -------------------
    begin                : mer mar 5 2003
    copyright            : (C) 2003 by 
    email                : Maurice.Clerc@WriteMe.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 //================================================= DISPLAY_GRAPH
void     display_graph(struct graph G)
{
  int i,j;
 for (i=0;i<G.N;i++)
 {
 printf( "\n%i /",i+1);
 for (j=0;j<G.N;j++)
    printf(" %.0f",G.v[i][j]);
 }
}

/*------------------------------------------------------------------- READ_GRAPH */
struct graph read_graph(FILE *file, int trace)
{
/* TSPLIB MATRIX format. One value/line. First value = number of nodes
each value = arc value. If <0 => no arc
*/
char			bidon[50];
char			comment[100];
double         delta;
char			edge_weight_format[30];
char			edge_weight_type[30];
struct graph	Gt;
int 			i,j;
char			name[20];
char			type[20];
float         zzz;


printf("\nI am reading the graph");

fscanf(file," %s %s\n",bidon,name);
fscanf(file," %s %s\n",bidon,type);
	fscanf(file," %s %s\n",bidon,comment);
	fscanf(file,"%s %i\n",bidon,&Gt.N); // dimension
	fscanf(file,"%s %s\n",bidon,edge_weight_type);
	fscanf(file,"%s %s\n",bidon,edge_weight_format);
	fscanf(file,"%s\n",bidon);


	printf("\n Name: %s, type: %s, (%s)",name,type,comment);
	printf("\n Number of nodes: %i",Gt.N);
	printf("\n %s %s\n",edge_weight_type,edge_weight_format);

	if (edge_weight_type[0]=='E' && edge_weight_format[0]=='F')
		{
		for (i=0;i<Gt.N;i++)
			{
                for (j=0;j<Gt.N;j++)
				{
                        fscanf(file,"%e ",&zzz);
                            Gt.v[i][j]=zzz;
					if (trace>2) printf(" %f",Gt.v[i][j]);
				}
			Gt.v[i][i]=0;
			}


    for (i=0;i<Gt.N;i++)
			{
			for (j=0;j<Gt.N;j++)
				{
                       if (Gt.v[i][j]>0) Gt.v[i][j]=integer_coeff*Gt.v[i][j];
                       }
                    }
  if (integer_coeff!=1) printf("\nWARNING. I have multiplied all arc values by %f",integer_coeff);

		return Gt;
		}
   printf("\nERROR by reading graph");
  return Gt;
}

//====================================================== SEQUENCE_SIMILAR
int   sequence_similar(struct seq s1,struct seq s2)
{
 /* Check if two sequences are globally similar:
 - same first value
 - same last value
 - same  set of values (not taking order into account)
 return 0 if false, 1 if true
 */
 int           i;
 int           j;
int            size;

   if (s1.s[0]!=s2.s[0]) return 0;

   size=s1.size;
   if (s1.s[size-1]!=s2.s[size-1]) return 0;
   if (size!=s2.size) return 0;

   //printf("\n\n"); for (i=0;i<s1.size;i++) printf(" %i",s1.s[i] );  printf("\n");  for (i=0;i<s1.size;i++) printf(" %i",s2.s[i] );

   for (i=1;i<size-1;i++)
   {
           for (j=0;j<size-1;j++)
           {
              if(s1.s[i]==s2.s[j]) goto next_i;
           }
            return 0;

    next_i:;
   }
  //printf("\n sequence_similar OK");
 return 1;
}

//====================================================== SEQUENCE_SUBST
struct   particle    sequence_subst(struct particle p1, struct particle p2,int size_max)
{/*
 s1= position of p1, s2=position of p2

 In s2, look for a sub sequence ss2 (size>=4) that is globally similar to one (ss1) in s1
 and so that s1/ss1=>ss2 is a better sequence (smaller total  length regarding graph G).
  f1 is the total length of the sequence s1.

 return: the new better sequence, after substitution

 G is a global variable.
 WARNING: each sequence is supposed to be a permutation of the G nodes, beginning in 0
 plus again the node 0 at the end (cycle).
 */
  double f1,f2;
  float  GN2;
 int        i;
 int        ij;
 int        im;
 int        j;
 int        k;
 int        km;
 int        kl;
 int        l;
 int        m;
 struct  particle p1t;
 struct particle ptemp;
 struct  seq   s1,s2, ss1,ss2;
 int        sim;
 int        size;
 double  zzz;

 GN2=G.N/2;
 s1=p1.x;
 s2=p2.best_x;
   p1t=p1;
  size=4;

  loop:
    ss1.size=size;  ss2.size=size;

   for (i=0;i<G.N;i++)    // Try all subsequences of size "size" and G.N-size
   {
      for (j=0;j<size;j++)
      {
           ij=i+j;
         if (ij>=G.N) ij=ij-G.N;
         ss1.s[j]=s1.s[ij];    // Build a subsequence in s1, for particle p1
                                    // It begins on i in particle p1
      }

       for (k=0;k<G.N;k++)  // For the second particle ...
       {
                  for (l=0;l<size;l++)
                  {
                      kl=k+l;
                      if(kl>=G.N) kl= kl-G.N;
                     ss2.s[l]=s2.s[kl];    // ... build a subsequence in s2
                                                // It begins on k in particle p2
                  }

                  sim=sequence_similar(ss1,ss2); // Compare the subsequences

                  if (sim==1)  // Globally the same
                  {
  //printf("\n size %i  ",size);
                     // Check if it is better
                     ptemp.x=ss1;
                     ptemp.x.s[size]=ss1.s[size-1];   // WARNING. All diagonal terms of G must be equal to 0
                                                                 // Normally, it is done in read_graph
                      f1=  f(ptemp,-1,-1);

                      ptemp.x=ss2;
                      ptemp.x.s[size]=ss2.s[size-1];
                      f2=  f(ptemp,-1,-1);

                      if (f2<f1)  // Replace ss1 by ss2
                      {

                           p1t.x=s1;
                           for (m=0; m<ss2.size;m++)
                           {
                              im=i+m;
                              if(im>=G.N) im=im-G.N;
                              p1t.x.s[im]=ss2.s[m];
                           }
                            p1t.f=p1.f-f1+f2;  // Update the f-value
                              if (trace>1)
                                 printf("\nsequence_subst. size %i, %f  => %f. ==> %f",size,f1,f2, p1t.f);

                           goto end_subs; // It is not allowed to replace both the sequence
                                                   // and its complementary: it would mean just replace
                                                   // p1.x by p2.best_x. Not interesting.
                        }


                        if (p2.best_f-f2<p1.f-f1) // The complementary susbsequence is better
                        {

                           p1t.x=s2;
                           for (m=0; m<ss1.size;m++)
                           {
                              km=k+m;
                              if(km>=G.N) km=km-G.N;
                              p1t.x.s[km]=ss1.s[m]; // Keep ss1
                           }
                           p1t.f=p2.best_f-f2+f1;   // Update the f-value
                            if (trace>1)
                               printf("\nsequence_subst. size %i, %f  => %f, ==> %f",G.N-size,p1.f-f1,p2.best_f-f2,p1t.f);
                           goto end_subs;
                        }
                        goto end_sim;
                    end_subs:

                        // Eventually update memory and return
                           // if ( p1t.f< p1t.best_f)
                           if ( p1t.f<best_best.best_f)
                           {
                             if (trace>1)
                                 printf("\n        sequence_subst %f ==> %f",p1.f,p1t.f);
                              p1t.best_x=p1t.x;
                              p1t.best_f=p1t.f;
                              // Eventually rotate
                              if (p1t.x.s[0]!=0) p1t.x=rotate(p1t.x,0);

                              return p1t;
                           }

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
float  h2;
int   j;
int   k;
struct   particle pt;
int         size;

if (splice_around_option==1) goto hood;
 // The whole swarm
 // Look, compare and eventually move
// if (splice_around_option==0 )
     size=sw.size;
 //else  size=(G.N*splice_around_option)/100;
      for(k=0;k<size;k++)
      {
         if (k==p.rank) continue;
               pt=sequence_subst(p,sw.p[k],size_max);
          if (pt.best_f<best_best.best_f) return pt; // Stop asa enough improvement
         //if (pt.best_f<p.best_f) return pt; // Stop asa there is improvement
      }
 printf("\n splice=> NO improvement");
 return pt;

  hood:
 // Case hood_type==0 (social hood)
/*--------------------- Neighbourhood. Super simple method: nearest by the rank */
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
 //printf("\n splice_around. %f %f %f %f",p.f,p.best_f,pt.f,pt.best_f);
          if (pt.best_f<best_best.best_f) return pt;
          // if (pt.best_f<p.best_f) return pt; // Stop asa there is improvement
      }
    printf("\n splice=> NO improvement");
  return pt;

 }

 //------------------------------------------------------------------- ALEA_VELOCITY
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

// ------------------------------------------------------------------- AUTO_MOVE
struct particle auto_move(struct particle p,int type, int monotony, int levelling,int trace)
/* The particle moves slowly, looking for a better position */
{
int               b,d,f;
int 			i,j,k;
int               improv;
int				levelling0;
int                  loop,loop_max;
struct particle	pt,pt1;
struct velocity v;

if (trace>1)
	printf("\n Auto-move type %i for particle %i. Size %i \n",type,p.rank+1,p.x.size);
pt=p;

 // loop_max=G.N;
 loop_max=1;
if (type==0) // Lazy Descent Method
	{
      for(loop=0;loop<loop_max;loop++)
      {
	for (i=0;i<G.N;i++) // Move at random.
		{
		pt.v=alea_velocity(1);
		pt=pos_plus_vel(pt,pt.v,monotony,1,levelling);

		if(pt.best_f<p.best_f) // Stop as soon as a better position is found
			{
			return pt;
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
	return pt;
	}


 if (type==6) goto three_opt;

//same_best_thresh

if (type>=2 && type<=5) /*
				Check all immediate physical neighbours.
            If one is better, move,
            if (type=3 or 5) check again
			*/
	levelling0=levelling;
if (type>2) levelling0=1;

	v.size=1;
   improv=0;
	next_step2:
	for (j=0;j<G.N-1;j++)  // For each neighbour




		{
		v.comp[0][0]=j;
		for (k=j+1;k<G.N;k++)
			{
			v.comp[0][1]=k;
			pt1=pos_plus_vel(pt,v,monotony,1,levelling);

			if((levelling==0 && pt1.best_f<pt.best_f) || (levelling>0 && f_levelling(G,pt,pt1)<pt.fl)) // Move towards the best neighbour
				{
				if (trace>1)
					{printf("\n auto move => ");display_position(pt,1);}
				pt=pt1;
                     improv=1;
				if (type==3 || type==5) goto next_step2;
				}
			}
		}
      if (improv==0)
      {
      	if (trace>1) {printf("\n auto move => no move ");display_position(p,1);}
	return p;
      }
   else
	return pt;


 three_opt:
 if (trace>1)
    printf("\n auto_move, 3-opt");
 //scanf("%i",&b);
 v.size=3;
    for (b=0;b<G.N;b++)
    {
       v.comp[0][0]= p.x.s[b];

       for (d=0;d<G.N;d++)
       {
          v.comp[1][0]= p.x.s[d];
          v.comp[0][1]= p.x.s[d];

          for (f=0;f<G.N;f++)
          {
             v.comp[2][0]= p.x.s[f];
             v.comp[1][1]= p.x.s[f];

              v.comp[2][1]= p.x.s[b];
          pt=pos_plus_vel(pt,v,0,0,0);

		if(pt.f<p.f) // Stop as soon as a better position is found
		{
			return pt;
		}

           } // f
       } // d
    }  // b
    return pt;
}

/*------------------------------------------------------------- COEFF_POS_MINUS_POS */
struct velocity	coeff_pos_minus_pos(struct particle p1,struct particle p2,double coeff,int monotony,int equiv_v, int trace)
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
shunt3:  // This one is the nearest one to  "theoretical" PSO
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

  	v1.size=distance(p,pg,type_dist);   // Radius
   v2.size=distance(pi,pg,type_dist);
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
   pit=pos_plus_vel(p,v1,0,1,0);      // A point around pi

   v1=alea_velocity(v1.size);
   pgt=pos_plus_vel(pg,v1,0,1,0);  // A point around pg

   v1=coeff_pos_minus_pos(pgt,pit,c2,0,equiv_v,trace); // c2*(pgt-pit)

   pt=pos_plus_vel(pi,v1,0,1,0); // New position
   pt.v=coeff_pos_minus_pos(pt,p,1,0,equiv_v,trace);  // New velocity (not really necessary)


goto end;


shunt9: // "Spherical" distribution  around p

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


double			ft[Vmax+1];
int				i,i1;

int				last_trans;
int				n1,n2;
struct particle	pt,ptemp;
int				rank1,rank2;

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
 /*
			if(ptemp.f<extra_best.f)
				{
				extra_best=ptemp;
				}
   */
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
  /*
		if(ptemp.f<extra_best.f)
			{
			extra_best=ptemp;
			}
   */
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
if (pt.x.s[0]!=0) pt.x=rotate(pt.x,0);
//if (extra_best.x.s[0]!=0) extra_best.x=rotate(extra_best.x,0);

if (trace>2)
	{
	printf("\n            particle after:");
	display_position(pt,1);
	}
return pt;
}


//====================================================== SEQUENCE_SUBST
struct   particle    sequence_subst(struct particle p1, struct particle p2,int size_max)
{/*
 s1= position of p1, s2=position of p2

 In s2, look for a sub sequence ss2 (size>=4) that is globally similar to one (ss1) in s1
 and so that s1/ss1=>ss2 is a better sequence (smaller total  length regarding graph G).
  f1 is the total length of the sequence s1.

 return: the new better sequence, after substitution

 G is a global variable.
 WARNING: each sequence is supposed to be a permutation of the G nodes, beginning in 0
 plus again the node 0 at the end (cycle).
 */
  double f1,f2;
  float  GN2;
 int        i;
 int        ij;
 int        im;
 int        j;
 int        k;
 int        km;
 int        kl;
 int        l;
 int        m;
 struct  particle p1t;
 struct particle ptemp;
 struct  seq   s1,s2, ss1,ss2;
 int        sim;
 int        size;
 double  zzz;

 GN2=G.N/2;
 s1=p1.x;
 s2=p2.best_x;
   p1t=p1;
  size=4;

  loop:
    ss1.size=size;  ss2.size=size;

   for (i=0;i<G.N;i++)    // Try all subsequences of size "size" and G.N-size
   {
      for (j=0;j<size;j++)
      {
           ij=i+j;
         if (ij>=G.N) ij=ij-G.N;
         ss1.s[j]=s1.s[ij];    // Build a subsequence in s1, for particle p1
                                    // It begins on i in particle p1
      }

       for (k=0;k<G.N;k++)  // For the second particle ...
       {
                  for (l=0;l<size;l++)
                  {
                      kl=k+l;
                      if(kl>=G.N) kl= kl-G.N;
                     ss2.s[l]=s2.s[kl];    // ... build a subsequence in s2
                                                // It begins on k in particle p2
                  }

                  sim=sequence_similar(ss1,ss2); // Compare the subsequences

                  if (sim==1)  // Globally the same
                  {
  //printf("\n size %i  ",size);
                     // Check if it is better
                     ptemp.x=ss1;
                     ptemp.x.s[size]=ss1.s[size-1];   // WARNING. All diagonal terms of G must be equal to 0
                                                                 // Normally, it is done in read_graph
                      f1=  f(ptemp,-1,-1);

                      ptemp.x=ss2;
                      ptemp.x.s[size]=ss2.s[size-1];
                      f2=  f(ptemp,-1,-1);

                      if (f2<f1)  // Replace ss1 by ss2
                      {

                           p1t.x=s1;
                           for (m=0; m<ss2.size;m++)
                           {
                              im=i+m;
                              if(im>=G.N) im=im-G.N;
                              p1t.x.s[im]=ss2.s[m];
                           }
                            p1t.f=p1.f-f1+f2;  // Update the f-value
                              if (trace>1)
                                 printf("\nsequence_subst. size %i, %f  => %f. ==> %f",size,f1,f2, p1t.f);

                           goto end_subs; // It is not allowed to replace both the sequence
                                                   // and its complementary: it would mean just replace
                                                   // p1.x by p2.best_x. Not interesting.
                        }


                        if (p2.best_f-f2<p1.f-f1) // The complementary susbsequence is better
                        {

                           p1t.x=s2;
                           for (m=0; m<ss1.size;m++)
                           {
                              km=k+m;
                              if(km>=G.N) km=km-G.N;
                              p1t.x.s[km]=ss1.s[m]; // Keep ss1
                           }
                           p1t.f=p2.best_f-f2+f1;   // Update the f-value
                            if (trace>1)
                               printf("\nsequence_subst. size %i, %f  => %f, ==> %f",G.N-size,p1.f-f1,p2.best_f-f2,p1t.f);
                           goto end_subs;
                        }
                        goto end_sim;
                    end_subs:

                        // Eventually update memory and return
                           // if ( p1t.f< p1t.best_f)
                           if ( p1t.f<best_best.best_f)
                           {
                             if (trace>1)
                                 printf("\n        sequence_subst %f ==> %f",p1.f,p1t.f);
                              p1t.best_x=p1t.x;
                              p1t.best_f=p1t.f;
                              // Eventually rotate
                              if (p1t.x.s[0]!=0) p1t.x=rotate(p1t.x,0);

                              return p1t;
                           }

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
float  h2;
int   j;
int   k;
struct   particle pt;
int         size;

if (splice_around_option==1) goto hood;
 // The whole swarm
 // Look, compare and eventually move
// if (splice_around_option==0 )
     size=sw.size;
 //else  size=(G.N*splice_around_option)/100;
      for(k=0;k<size;k++)
      {
         if (k==p.rank) continue;
               pt=sequence_subst(p,sw.p[k],size_max);
          if (pt.best_f<best_best.best_f) return pt; // Stop asa enough improvement
         //if (pt.best_f<p.best_f) return pt; // Stop asa there is improvement
      }
 printf("\n splice=> NO improvement");
 return pt;

  hood:
 // Case hood_type==0 (social hood)
/*--------------------- Neighbourhood. Super simple method: nearest by the rank */
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
 //printf("\n splice_around. %f %f %f %f",p.f,p.best_f,pt.f,pt.best_f);
          if (pt.best_f<best_best.best_f) return pt;
          // if (pt.best_f<p.best_f) return pt; // Stop asa there is improvement
      }
    printf("\n splice=> NO improvement");
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

/*------------------------------------------------------------------- F */
double f(struct particle p,int rank1,int rank2) // Function to minimize
// Objective function for Traveling Salesman Problem
{
double				d;
double				delta_f;
double				ft;
int 				i;
int					n,n0,n1,n2,ni;
int					no_arc;
double				val1,val2;

eval_f=eval_f+(double)p.x.size/G.N;  // Sometimes, it is just a partial evaluation of the objective function
                                                         // so it wouldn't be fair to systematically add 1
no_arc=0;


if (rank1>=0) goto modif;

ft=0;

if (trace>3) printf("\n\n Compute f for particle %i",p.rank+1);

for (i=0;i<p.x.size;i++) /* For each arc in the particle ... */
	{
	n=p.x.s[i];
	n0=p.x.s[i+1];

// TEST
if (n0>G.N || n>G.N)
	{
	printf("\nERROR arc %i=>%i",n+1,n0+1);
	printf(" value %f",G.v[n][n0]);
	}


	/* ----------------- Penalty for unexistent arcs */
	if (G.v[n][n0]<0) /* If it is not an arc in the graph ... */
		{
		delta_f=tax_no_arc(no_arc,G); /* ... you pay a tax */
		no_arc=no_arc+1;/* ... (depending of how many no_arc have already been count)... */

//printf("\n no arc. Tax %f",delta_f);
		}
	else  /* If it is a "valid" arc ... */
		{
		delta_f=G.v[n][n0]; /* ...you pay the right price ... */
		}
	ft=ft+delta_f;

	}
goto end;


//--------------------------
modif: /* IMPORTANT. This works only when taxes_noarc is a CONSTANT
			Also, it supposes G(i,i)=0
		 In the particle p, just two nodes, at ranks rank1 and rank2,
		   has been swapped. It is less costly to just modify the f value
		*/
n1=p.x.s[rank1];
n2=p.x.s[rank2];
ft=p.f;
//display_position(p,0,G);
//printf("\n f before %f, swapped nodes  %i, %i, ranks %i and %i",p.f,n1+1,n2+1, rank1,rank2);

if (rank1==0) i=G.N-1; else i=rank1-1;
ni=p.x.s[i];
// arc p(i)=>n1  was arc p(i)=>n2
if (G.v[ni][n1]<0) val1=tax_no_arc(no_arc,G); else val1=G.v[ni][n1];
if (G.v[ni][n2]<0) val2=tax_no_arc(no_arc,G); else val2=G.v[ni][n2];

ft=ft-val2+val1;


if (rank1==G.N-1) i=0; else i=rank1+1;
ni=p.x.s[i];
// arc n1=>p(i)  was arc n2=>p(i)
if (G.v[n1][ni]<0) val1=tax_no_arc(no_arc,G); else val1=G.v[n1][ni];
if (G.v[n2][ni]<0) val2=tax_no_arc(no_arc,G); else val2=G.v[n2][ni];

ft=ft-val2+val1;

//---------

if (rank2==0) i=G.N-1; else i=rank2-1;
ni=p.x.s[i];

	// arc p(i)=>n2  was arc p(i)=>n1
	if (G.v[ni][n1]<0) val1=tax_no_arc(no_arc,G); else val1=G.v[ni][n1];
	if (G.v[ni][n2]<0) val2=tax_no_arc(no_arc,G); else val2=G.v[ni][n2];

	ft=ft-val1+val2;


if (rank2==G.N-1) i=0; else i=rank2+1;
ni=p.x.s[i];

	// arc n2=>p(i)  was arc n1=>p(i)
	if (G.v[n1][ni]<0) val1=tax_no_arc(no_arc,G); else val1=G.v[n1][ni];
	if (G.v[n2][ni]<0) val2=tax_no_arc(no_arc,G); else val2=G.v[n2][ni];
	ft=ft-val1+val2;

if (abs(rank1-rank2)==1 || abs(rank1-rank2)==G.N-1) // Particular cases n1=>n2 or n2=>n1
	{
	if (G.v[n1][n2]<0) val1=tax_no_arc(no_arc,G); else val1=G.v[n1][n2];
	if (G.v[n2][n1]<0) val2=tax_no_arc(no_arc,G); else val2=G.v[n2][n1];
	ft=ft-val1-val2;
	}

//printf("   f after %f",ft);


end:

/*
if (ft<extra_best.f)
	{
	extra_best=p;
	extra_best.f=ft;
	if (extra_best.x.s[0]!=0) extra_best.x=rotate(extra_best.x,0);
	}
*/



return ft;

// Just for partial Map
d=distance(best_solution,p,type_dist);
	fprintf(f_trace,"%f %f\n",d,ft);

return ft;

}

/*------------------------------------------------------------------- 	TAX_NO_ARC */
double tax_no_arc(int l,struct graph G)
	{
	/*
	Penalize the l-th unexistent arc
	*/

double 	tax=1.1; /* >=1 */
double 	x;

	//x=tax*G.l_max+(G.N-l)*(G.l_max-G.l_min);
	x=tax*G.l_max+(G.N-1)*(G.l_max-G.l_min);

	return x;

	}

/*------------------------------------------------------------------- DISPLAY_POSITION */
void display_position(struct particle p,int type)
{
int 	i;
float 	x;

printf("\n particle %i: ",p.rank+1);

if (type>1) goto best_previous;

for (i=0;i<p.x.size+1;i++)
	printf(" %i",p.x.s[i]+1);

if (trace>1)
	{
	printf("\n arc values: ");
	for (i=0;i<p.x.size;i++)
		{
		x=G.v[p.x.s[i]][p.x.s[i+1]];
		if (x<0) printf("? "); else printf("%f ",x);
		}
	}

	 printf(" f value:%f",p.f);
	printf(" v.size: %i,",p.v.size);

   if (trace>2)
	display_velocity(p.v);

return;

best_previous:
	printf("\n best previous: ");
	for (i=0;i<p.best_x.size+1;i++)
		printf(" %i",p.best_x.s[i]+1);

   	 printf(" f value:%f",p.best_f);

}
/*------------------------------------------------------------------- DISPLAY_SWARM */
void display_swarm(struct swarm sw)
{
int i,i_best;

i_best=0;
printf("\n SWARM");
for (i=0;i<sw.size;i++) /* For each "particle"  */
	{
	display_position(sw.p[i],1);
	if (sw.p[i].f<sw.p[i_best].f) i_best=i;

	}
printf("\n Best particle: %i, f value %.1f",i_best+1,sw.p[i_best].f);
printf("\n");


}
/*------------------------------------------------------------------- DISPLAY_VELOCITY */
void display_velocity(struct velocity v)
{
int  i;

printf("\n  velocity (size=%i) :",v.size);

for (i=0;i<v.size;i++)
	{
	printf(" %i<=>%i ",v.comp[i][0]+1,v.comp[i][1]+1);
	}
}
