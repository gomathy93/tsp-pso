
/*------------------------------------------------------------------- F */
float f(struct particle p,int rank1,int rank2) // Function to minimize 
// Objective function for Sorting
// The (integer) number to sort are in the global vector q
{
 float				delta_f;
 float 				ft;
 int 				i;
 int					N;
 int					n,n0,n1,n2,ni;
 float				tax;
 float				val1,val2;
 
 eval_f=eval_f+1;
 
 N=p.x.size;
 tax=N*(G.l_max-G.l_min);
 
 //if (rank1>=0) goto modif;
 
 ft=0;
 
 if (trace>3) printf("\n\n Compute f for particle %i",p.rank+1);
 
 for (i=0;i<N;i++) /* For each arc in the particle ... */
 {
	n=q[p.x.s[i]];
	n0=q[p.x.s[i+1]];
	
	if (n>n0) delta_f=tax;
	else delta_f=n0-n;
	ft=ft+delta_f;
	
 }
 goto end;
 
 
 //--------------------------
 modif: /* 
	 In the particle p, just two nodes, at ranks rank1 and rank2,
	 has been swapped. It is less costly to just modify the f value
	 */
	n1=q[p.x.s[rank1]];
 n2=q[p.x.s[rank2]];
 ft=p.f;
 
 //printf("\n f before %f, swapped nodes  %i, %i, ranks %i and %i",p.f,n1+1,n2+1, rank1,rank2);
 
 if (rank1>0) 
 {
	i=rank1-1;
	ni=q[p.x.s[i]];
	// arc ni=>n1  was arc ni=>n2
	if (ni<n1) val1=n1-ni;else val1=tax; // new value
	if (ni<n2) val2=n2-ni;else val2=tax; // old value
	
	ft=ft-val2+val1;
 }
 
 
 if (rank1<G.N-1)
 {
	i=rank1+1;
	ni=q[p.x.s[i]];
	// arc n1=>ni  was arc n2=>ni
	if (n1<ni) val1=ni-n1;else val1=tax; // new value
	if (n2<ni) val2=ni-n2;else val2=tax; // old value
	
	ft=ft-val2+val1;
 }
 
 //--------- 
 
 if (rank2>0) 
 {
	i=rank2-1;
	ni=q[p.x.s[i]];
	
	// arc ni=>n2  was arc ni=>n1
	if (ni<n1) val1=n1-ni;else val1=tax; // old value
	if (ni<n2) val2=n2-ni;else val2=tax; // new value
	
	ft=ft-val1+val2;
 }
 
 if (rank2<G.N-1)
 {
	i=rank2+1;
	ni=q[p.x.s[i]];
	
	// arc n2=>ni  was arc n1=>ni
	if (n1<ni) val1=ni-n1;else val1=tax; // old value
	if (n2<ni) val2=ni-n2;else val2=tax; // new value
	
	ft=ft-val1+val2;
 }
 
 
 if (rank2==rank1+1)
 {
	// arc n1=>n2 was n2=>n1
	if (n2<n1) val1=n1-n2;else val1=tax; // old value
	if (n1<n2) val2=n2-n1; else val2=tax; // new value
	ft=ft+val1-val2;
 }
 
 if (rank1==rank2+1)
 {
	// arc n2=>n1 was arc n1=>n2
	if (n1<n2) val1=n2-n1;else val1=tax; // old value
	if (n2<n1) val2=n1-n2; else val2=tax; // new value
	ft=ft+val1-val2;
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
}
