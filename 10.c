#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>
#include<time.h>

int main(int argc, char * argv[]) {
  
  int niter = 1000000;
  int myid; //holds process's rank id
  double x, y; //x,y value for the random coordinate
  int i,size;
  int count = 0; //Count holds all the number of how many good coordinates
  double z; //Used to check if x^2+y^2<=1
  double pi,wtime; //holds approx value of pi
  int reducedcount; //total number of "good" points from all nodes
  MPI_Init( & argc, & argv); //Start MPI
  MPI_Comm_rank(MPI_COMM_WORLD, & myid); //get rank of node's process
	MPI_Comm_size(MPI_COMM_WORLD, & size);
  /* Everyone can do the work */
	
	wtime = MPI_Wtime ( );
  for (i = 0; i < niter; ++i) //main loop
  {
    srand48(time(NULL) + myid); //Give rand() a seed value unique on each node (times are synced)
    x = (double) random() / RAND_MAX; //gets a random x coordinate
    y = (double) random() / RAND_MAX; //gets a random y coordinate
    z = sqrt((x * x) + (y * y)); //Checks to see if number in inside unit circle
    if (z <= 1) {
      ++count; //if it is, consider it a valid random point
    }
  }

  MPI_Reduce( & count, &
    reducedcount,
    1,
    MPI_INT,
    MPI_SUM,
    0,
    MPI_COMM_WORLD);

  wtime = MPI_Wtime( ) - wtime;
  if (myid == 0) //if root process
  {
    pi = ((double) reducedcount / (double) (niter*size)) * 4.0; //p = 4(m/n)
    printf("Pi: %f\n%i\n", pi, reducedcount);
    //Print the calculated value of pi
  	printf("time: %lf\n",wtime);

  }

  MPI_Finalize(); //Close the MPI instance
  return 0;
}
