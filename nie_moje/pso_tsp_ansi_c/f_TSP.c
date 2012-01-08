#include "stdinc.h"

/*------------------------------------------------------------------- F */
double f(struct particle p,int rank1,int rank2) // Function to minimize 
// Objective function for Traveling Salesman Problem
// Particular case: if rank1 (and rank2) are >=0, just switch the two node
{
//double				d;
double				delta_f;
double				ft;
//double                     ft_check;
int 				i,j,k,m;
int					n,n0,n1,n2,ni,nj,nk,nm;

int                       rank1a,rank2a;
double               tax;

double                     v1,v2,v3,v4,v5;
double                     vp1,vp2,vp3,vp4,vp5;


//no_arc=0;
 tax=tax_no_arc(0,G);

if (rank1>=0) goto modif;
//if (rank1<0) 
eval_f=eval_f+(double)p.x.size/G.N;  // Sometimes, it is just a partial evaluation of the objective function
                                                         // so it wouldn't be fair to systematically add 1
ft=0;

if (trace>3) printf("\n\n Compute f for particle %i",p.rank+1);

for (i=0;i<p.x.size;i++) /* For each arc in the particle ... */
	{
	n=p.x.s[i];
	n0=p.x.s[i+1];
      if ((i+1)>=p.x.size) n0=p.x.s[0];
	
/*
// TEST
if (n0>G.N || n>G.N)
	{	
	printf("\nERROR. f_TSP arc %i=>%i",n+1,n0+1);	
	printf(" value %f",G.v[n][n0]);	
	}
*/	
	
	/* ----------------- Penalty for unexistent arcs */
	if (G.v[n][n0]<0) /* If it is not an arc in the graph ... */
		{
		//delta_f=tax_no_arc(no_arc,G); /* ... you pay a tax */
      delta_f=tax;
		//no_arc=no_arc+1;/* ... (depending of how many no_arc have already been count)... */
		 
//printf("\n no arc. Tax %f",delta_f);
		}
	else  /* If it is a "valid" arc ... */
		{
		delta_f=G.v[n][n0]; /* ...you pay the right price ... */
		}
	ft=ft+delta_f;
	
	}
 //ft_check=ft;
if (rank1<0)
   goto end;


modif:
//--------------------------   Method to use when there has been just a transposition
/* IMPORTANT. This works only when taxes_noarc is a CONSTANT
			Also, it supposes G(i,i)=0
		 In the particle p, just two nodes, at ranks rank1 and rank2,
		   HAS BEEN swapped. It is less costly to just modify the f value
		*/


if(rank1<rank2)
    {rank1a=rank1;rank2a=rank2;}
else
    {rank1a=rank2;rank2a=rank1;}
    
n1=p.x.s[rank1a]; // This node _was_ at rank rank2
n2=p.x.s[rank2a];  // This node _was_ at rank rank1


 if ((rank2a==rank1a+1) ||  (rank1a==0 && rank2a==p.x.size-1)) // particular case (adjacent nodes)
       goto adj;

 // "normal" case      
i=rank1a-1; if (i<0) i=p.x.size-1;
j=(rank1a+1);
k=(rank2a-1);
m=(rank2a+1); if (m>=p.x.size) m=0;

ni=p.x.s[i];
nj=p.x.s[j];
nk=p.x.s[k];
nm=p.x.s[m];

 // Arcs to remove
// ni => n2 => nj =>=> nk => n1 =>nl   (old tour). Value p.f
vp1=G.v[ni][n2];   if(vp1<0) vp1=tax;
vp2=G.v[n2][nj];   if(vp2<0) vp2=tax;
vp3=G.v[nk][n1];   if(vp3<0) vp3=tax;
vp4=G.v[n1][nm];    if(vp4<0) vp4=tax;


// Arcs to add
// ni => n1 => nj =>=> nk => n2 =>nl   (new tour)
v1=G.v[ni][n1];   if (v1<0) v1=tax;
v2=G.v[n1][nj];     if (v2<0) v2=tax;
v3=G.v[nk][n2];    if (v3<0) v3=tax;
v4=G.v[n2][nm];      if (v4<0) v4=tax;

ft=p.f-vp1-vp2-vp3-vp4+v1+v2+v3+v4;

    eval_f=eval_f+4.0/p.x.size; // Just four values to update.

/*
     if (ft_check!=ft)
 {
     printf("\n\n ERROR ft. p.f=%f. %i <=> %i %f instead of %f?",p.f,rank1,rank2,ft,ft_check);
     printf("\n %i %i %i %i %i %i",i,rank1a,j,k,rank2a,m);
     printf("\n %i %i %i %i %i %i",ni,n1,nj,nk,n2,nm);
     printf("\n %f %f %f %f / %f",vp1,vp2,vp3,vp4,vp5);
     printf("\n %f %f %f %f / %f",v1,v2,v3,v4,v5);
  trace=2;display_position(p,1); trace=0;
  }
 */
    goto end;

adj:    // Adjacent nodes
if (rank1a==0 && rank2a==p.x.size-1)
{
 i=p.x.size-2;
 m=1;
 nk=n1;n1=n2;n2=nk;
}
else
{
i=rank1a-1; if (i<0) {i=p.x.size-1;  m=2;goto nil;}
m=(rank2a+1); if (m>=p.x.size) {m=0; i= p.x.size-3;}
}

nil:
ni=p.x.s[i];
nm=p.x.s[m];

 // Arcs to remove
// ni => n2 => n1 =>nl   (old tour). Value p.f
vp1=G.v[ni][n2];   if(vp1<0) vp1=tax;
vp5=G.v[n2][n1];      if (vp5<0) vp5=tax;
vp4=G.v[n1][nm];    if(vp4<0) vp4=tax;


 // Arcs to add
// ni => n1 => n2 =>nl   (new tour)
v1=G.v[ni][n1];   if (v1<0) v1=tax;
v5=G.v[n1][n2];      if (v5<0) v5=tax;
v4=G.v[n2][nm];      if (v4<0) v4=tax;


    ft=p.f-vp1-vp4-vp5+v1+v4+v5;
    eval_f=eval_f+3.0/p.x.size; // Just three values to update.

/*
 if (ft_check!=ft)
 {
     printf("\n\n ERROR ft. p.f=%f. %i <=> %i %f instead of %f?",p.f,rank1,rank2,ft,ft_check);
     printf("\n %i %i %i %i ",i,rank1a,rank2a,m);
     printf("\n %i %i %i %i ",ni,n1,n2,nm);
     printf("\n %f %f %f ",vp1,vp5,vp4);
     printf("\n %f %f %f ",v1,v5,v4);
     trace=2;display_position(p,1); trace=0;
  }

 */ 

end:

/*
if(ft<0)
{
printf("\n ERROR. f_TSP. Abnormale value %f. rank1 %i, rank2 %i, ft %f, p.f %f,val1 %f,val2,%f",ft,rank1,rank2,ft,p.f,val1,val2);
trace=2;
display_position(p,1);
display_position(p,2);
printf("\n Break manually, please");
printf("\n Break manually, please");
infinite:;goto infinite;
 }
*/

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

	x=tax*G.l_max+(G.N-1)*(G.l_max-G.l_min);

	return x;

	}

