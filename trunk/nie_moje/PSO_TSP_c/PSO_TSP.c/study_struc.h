struct path			{int length;int path[Nmax+1];};
struct path_list	{int size;struct path paths[(Nmax+1)/2];};
struct sw_dist		{float d[Max_size+1][Max_size+1];}; /* To memorize f values, to speed up the algorithm */
