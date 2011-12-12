adaptive=1; 		// 0 : constant threshold for rehope. 1 = depending on the current best value. See nohope() 
auto_move_type; 	/* 	0 = move randomly, stop as soon as a bes position is found. 
						1 = move randomly, as long as a better position is found 
						2 = move systematically (check all positions) as long as a better position is found
						3 = as 2, but use temporary objective function ("levelling")
						9 = adaptive (0 then 1 then 2 then 3). See rehope()
					suggested: 9
					*/
best_hood_type=1;	// 0 = use best neighbour; 1 = use best neighbour's previous best
eval_f;				// Number of tour evaluations
equiv_v=0; 			// >0 = try to find a smaller equivalent velocity. 0 = do nothing. Cf equiv_vel()
explicit_vel=1;		/* 0 = no explicit velocities for particles (see move_towards). 1 = there ARE explicit velocities
					suggested: 1
					*/
LIL=0;				// 0 = no Local Iterative Levelling; >0 = up to LIL times
monotony=0; 		// see pos_plus_vel, coeff_times_vel
nohope=0.5;			/* For nohope threshold. Standard value: 0.5.  On [0,1]  .See no_hope. 
						The greater it is, the more often Re-hope process is launched */
parallel=0 ;		/* 	0 = each particle is moved one after the other
			  			1 = simulate global move of the swarm */
reduce=1;			 /* 0 = no reduced swarm ; 1 = try to reduce the swarm
					suggested/ 1
					*/						
rehope; 			//  0 = no rehope at all. 1 = rehope, if necessary
rehope_method=1;	/* 0 = simple method, 1 = sophisticated method
					suggested: 1
					*/
same_best_thresh=2; // Threshold for NoHope for this criterion
save=0;				// If = 0, no save file for the run
step=1; 			/* 1 = at each "unit" step, check the f value. 0 = don't check
					suggested: 1
					*/
type_dist=1;		/* 0 = "distance" depending on common arcs; 1 = true distance
					suggested: 1
					*/
hood_type;			// 0 = social hood. 1 = physical hood
trace=0;			// Default value, to begin. The higher is is, the more verbose is the program