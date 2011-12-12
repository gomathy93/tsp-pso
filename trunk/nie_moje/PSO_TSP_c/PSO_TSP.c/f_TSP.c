/*------------------------------------------------------------------- F */
float f(struct particle p,int rank1,int rank2) // Function to minimize 
// Objective function for Traveling Salesman Problem
{
float				d;
float				delta_f;
float 				ft;
int 				i;
int					n,n0,n1,n2,ni;
int					no_arc;
float				val1,val2;

eval_f=eval_f+1;
no_arc=0;


if (rank1>=0) goto modif;

ft=0;

if (trace>3) printf("\n\n Compute f for particle %i",p.rank+1);

for (i=0;i<G.N;i++) /* For each arc in the particle ... */
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
			Also, it suppose G(i,i)=0
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

if (ft<extra_best.f) 
	{
	extra_best=p;	
	extra_best.f=ft;
	if (extra_best.x.s[0]!=0) extra_best.x=rotate(extra_best.x,0);
	}
	
return ft;
	
// Just for partial Map	
d=distance(best_solution,p);
	fprintf(f_trace,"%f %f\n",d,ft);		
	
return ft;


}
