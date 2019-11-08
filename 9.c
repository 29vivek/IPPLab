#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
/* Define length of dot product vectors */
#define VECLEN 100
int min(int a, int b)
{
	return(a<b?a:b);
}

int main (int argc, char* argv[])
{
	int i,myid, numprocs, len=VECLEN;
	double *a, *b;
	double mysum, allsum=0.0;
	int nbar,extra,index,offset=0;
/* MPI Initialization */

	MPI_Init (&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank (MPI_COMM_WORLD, &myid);
	


	if (myid == 0)
		printf("Starting omp_dotprod_mpi. Using %d tasks...\n",numprocs);
/* Assign storage for dot product vectors */
	a = (double*) malloc (len*sizeof(double));
	b = (double*) malloc (len*sizeof(double));
	
/* Initialize dot product vectors */
	for (i=0; i<len; i++) 
	{
		a[i]=2.0;
		b[i]=a[i];
	}
	
/* Perform the dot product */

	mysum = 0.0;
	nbar=len/numprocs;
	extra=len%numprocs;
	if(myid<extra)
		offset=1;
	
	double t1=MPI_Wtime();	
	for (i=0; i<(nbar+offset); i++)
	{
		index=myid*nbar+i+min(extra,myid);
		mysum += a[index] * b[index];
	}
	printf("Task %d partial sum = %f\n",myid, mysum);

/* After the dot product, perform a summation of results on each node */
	MPI_Reduce (&mysum, &allsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	double t2=MPI_Wtime()-t1;
	if (myid == 0)
	{
		printf ("Done. MPI version: global sum = %f \n", allsum);
		printf("time of execution for %d procs= %lf\n",numprocs,t2);
	}
	free (a);
	free (b);
	MPI_Finalize();
}
