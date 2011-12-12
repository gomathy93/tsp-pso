/*
best_hood_type=1;	// 0 = use best neighbour; 1 = use best neighbour's previous best
BB_option=1; // 0 = use an average value, 1= use the min value. Cf BB_update() and BB_use()
equiv_v=1; 			// >0 = try to find a smaller equivalent velocity. 0 = do nothing. Cf equiv_vel()
explicit_vel=1;		// 0 = no explicit velocities for particles (see move_towards). 1 = there ARE explicit velocities
					//suggested: 1
					
hood=3; // Neighbourhood size
hood_type=0; // 0 = social (ranks) 1= physical (distances)
init_option=1;   // 0 => random; 1 => at least partly deterministic
//LIL=0;				// 0 = no Local Iterative Levelling; >0 = up to LIL times
  kappa=0.5;
monotony=0; 		// see pos_plus_vel, coeff_times_vel

// -------------------Move strategy set (0=don't use it)
 move[0]=10; // use normal move . See move_towards() for values
 // 0 don't use it at all!
// 1  Classical
//2 Classical
//3  Theoretical.
//4  Classical
 //5 Classical
//6   Spherical, center pg
//8  Spherical, non uniform combination,  center between pi and pg . **Main Tribes strategy
//9   Spherical, center p
//10 Spherical, center pi
 
move[1]=3; // use auto_move
// 	                           0 = move randomly, stop as soon as a better position is found.
//						1 = move randomly, as long as a better position is found
//						2 = move systematically (check all positions). Stop as soon as a better position is found
                                 3 = move systematically (check all positions) as long as a better position is found
//						//4 = as 2, but use temporary objective function ("levelling")
                                // 5 = as 3, but use temporary objective function ("levelling")
                                 6 = as 3 + eventually 3-opt method
                                 7 = as 3 + eventually 3-opt_weak
					
//move[2]=0; // >0  => use splice_around  for the best particle
//move[3]=1;  // >0 => use splice_around for more particles .
                  //Warning: expensive.  See also splice_around_option.
//move[4]=0;   // >0  => use a tool to directly build a new tour

//1 use randomness //Not good
//2 use arc_around // Not very good, either! 
//3  use BB  (blackboard)  // Better
//9  choose at random in {1,2,3}  // ??

//if (move[3]>0) move[1]=2;  // Rule of thumb

phi=4;
reduce=1;			 // 0 = no reduced swarm ; 1 = try to reduce the swarm
					//suggested						
save=0;				// If = 0, no save file for the run
splice_around_option=1;// 0 = whole swarm, 1 = just the neighbourhood , >1 a given % of the whole swarm
subst_option=1;// 0 = try to improve the current position, 1=try to improve  the best previous position
                        // Useful only if you use splice_around() (see move[2] and move[3])

step=1; 			// 1 = at each "unit" step, check the f value. 0 = don't check
					//suggested: 1
type_dist=1;		// 0 = "distance" depending on common arcs; 1 = true distance, in terms of transposition number
					//suggested: 1
hood_type=0;			// 0 = social hood. 1 = physical hood
trace=0;			// Default value, to begin. The higher it is, the more verbose is the program
 */

BB_option=1;  // Unuseful if move[4]<3
best_hood_type=1 ;
equiv_v=1;
explicit_vel=1;
hood=3;
hood_type=0;

monotony=0;

subst_option=0;
reduce=1;
splice_around_option=0;
step=1;
type_dist=1;
hood_type=0;
 
// =============================================================================================


// br17.This set is not that good.  17/20 (max eval =20000), mean eval 5300
/*
init_option=1; 


move[0]=8;
move[1]=3;  
move[2]=0; 
move[3]=1; // Use splice around for more particle 
move[4]=0; // no rebuild at all
 */
/*
//br17.  This set is bad
init_option=1;
 move[0]=8;
move[1]=3;
move[2]=0;
move[3]=0;
move[4]=1;

*/
/*
 //br17.
init_option=1;
 move[0]=8;
move[1]=3;
move[2]=0;
move[3]=0;
move[4]=0;
*/

 /*
  //br17.
init_option=1;
move[0]=10;
move[1]=3;
move[2]=0;
move[3]=0;
move[4]=0;
*/


 /*
  //br17.
init_option=2;
move[0]=10;
move[1]=3;
move[2]=0;
move[3]=0;
move[4]=0;
 */

/* 
  // SET 203000
 // br17. UNFORTUNATELY VERY GOOD. swarm size 20, eval 201, 20/20
  //ftv33. swarm 400. eval 200000 => 1458
  // x50.1, swarm 400. eval 20000 => 5.93
 init_option=2;
move[0]=0;  // Don't use PSO at all!
move[1]=3;
move[2]=0;
move[3]=0;
move[4]=0;
*/
 
 /*
   // SET 207000
  //  br17. VERY GOOD. swarm_size 20, eval 236, 20/20
   // ftv33. swarm 400. eval 200000 => 1452
   //x50.1, swarm 300, eval 37000 => 5.96
 init_option=2;
move[0]=0;  // Don't use PSO at all!
move[1]=7;
move[2]=0;
move[3]=0;
move[4]=0;
*/ 

 /*
//SET 2-10-7000
 //  br17. VERY GOOD. swarm_size 20, eval 266, 20/20
  // ftv33. swarm 400. eval 200000 => 1452
  //x50.1, swarm 300, eval 27000 => 5.96
 init_option=2;
move[0]=10;
move[1]=7;
move[2]=0;
move[3]=0;
move[4]=0;
 */
 
init_option=2;
move[0]=9;
move[1]=7;
move[2]=0;
move[3]=0;
move[4]=0;
 
 /*
 // SET 2-10-6000
 // quite bad (too many eval)
init_option=2;
move[0]=10;
move[1]=6;
move[2]=0;
move[3]=0;
move[4]=0;
  */
 /*
 //SET 2-10-7010
 // ftv33. swarm 400. eval 200000 => 1365  
   subst_option=0;
   
 //x50.1, swarm 300, eval 26700 => 5.96  
 // ftv33, swarm 154
 subst_option=1;
reduce=1;
splice_around_option=0;
 init_option=2;
move[0]=10;
move[1]=7;
move[2]=0;
move[3]=1;
move[4]=0;
*/

 /*
//SET 207010
 //x50.1, swarm 300, eval 200000 => 5.96
 init_option=2; 
 move[0]=0;
move[1]=7;
move[2]=0;
move[3]=1;
move[4]=0;
*/

/*Not very good
 init_option=2;
 move[0]=0;
move[1]=7;
move[2]=0;
move[3]=0;
move[4]=3;
*/

 /*

  init_option=2;
move[0]=10;
move[1]=6;
move[2]=0;
move[3]=0;
move[4]=0;
 */
 
 /*
 // This set is  OK, but a bit chaotic. 20/20 (max eval 200000)

 //br17. mean eval 29000. Sometimes very low (820), sometimes very high (175000)
 init_option=1;
  move[0]=8;
move[1]=3;
move[2]=0;
move[3]=1;
move[4]=0;
*/


 /*
  init_option=1;
move[0]=10;
move[1]=3;
move[2]=0;
move[3]=0;
move[4]=0;
 */
/*

 init_option=1;
  move[0]=10;
move[1]=7;
move[2]=0;
move[3]=0;
move[4]=0;
*/

/* 

init_option=2;
move[0]=9;
move[1]=7;
move[2]=0;
move[3]=0;
move[4]=0;
*/

 /*

 init_option=2;
move[0]=8;
move[1]=7;
move[2]=0;
move[3]=0;
move[4]=0;
*/

 /*
init_option=2;
move[0]=10;
move[1]=3;
move[2]=0;
move[3]=0;
move[4]=0;
   */


 /*
init_option=2;
move[0]=10;
move[1]=7;
move[2]=0;
move[3]=0;
move[4]=0;
 */




  //================================================

  













