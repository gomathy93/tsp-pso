// When using f_Sort ... Just an example
G.N=7;
G.l_min=6*G.N;
G.l_max=0;
save=0;
for (i=1;i<G.N;i++)
	{
	q[i]=alea(0,5*G.N);
	if (q[i]<G.l_min) G.l_min=q[i];
	if (q[i]>G.l_max) G.l_max=q[i];
	}
q[0]=0;
q[G.N]=0;


goto parameters;