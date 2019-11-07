#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include<algorithm.h>
#define NRA 3               /* number of rows in matrix A */
#define NCA  3               /* number of columns in matrix A */
//#define NCB 5                  /* number of columns in matrix B */
#define MASTER 0               /* taskid of first task */
#define FROM_MASTER 1          /* setting a message type */
#define FROM_WORKER 2          /* setting a message type */

int main (int argc, char *argv[])
{
int	numtasks,              /* number of tasks in partition */
	taskid,                /* a task identifier */
	numworkers,            /* number of worker tasks */
	source,                /* task id of message source */
	dest,                  /* task id of message destination */
	mtype,                 /* message type */
	rows,                  /* rows of matrix A sent to each worker */
	averow, extra, offset, /* used to determine rows sent to each worker */
	i, j, k, rc;           /* misc */
int	a[NRA][NCA],           
	c[NRA]={0};           /* result matrix C */
MPI_Status status;

MPI_Init(&argc,&argv);
double start = MPI_Wtime();
MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
if (numtasks < 2 ) {
  printf("Need at least two MPI tasks. Quitting...\n");
  MPI_Abort(MPI_COMM_WORLD, rc);
  exit(1);
  }
numworkers = numtasks-1;




/**************************** worker task ************************************/
   if (taskid > MASTER)
   {
      mtype = FROM_MASTER;
      MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&a[offset][0], rows*NCA, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
      //MPI_Recv(&b, NCA*NCB, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
	//printf("recieved from %d \n",offset);
	int ss=0;
      for(int i=offset;i<(offset+rows);i++)
	{
	//printf("\n%d offset\n",offset);
	for(int j=0;j<NCA;j++)
	{
		//printf("%d ",a[i][j]);
		ss+=abs(a[i][j]);
	}
	printf("\n");
	}
	
      mtype = FROM_WORKER;
      MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
      MPI_Send(&ss, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
	//printf("sent from %d",offset);
	//printf(" %d \n",ss);
   }
/**************************** master task ************************************/
   if (taskid == MASTER)
   {
      printf("mpi_mm has started with %d tasks.\n",numtasks);
      printf("Initializing arrays...");
      for (i=0; i<NRA; i++)
	{
         for (j=0; j<NCA; j++)
		{
            	a[i][j]= i+j;
		printf("%d  ",a[i][j]);
		}
	   printf("\n");
  	}
	
      

      /* Send matrix data to the worker tasks */
      averow = NRA/numworkers;
      extra = NRA%numworkers;
      offset = 0;
      mtype = FROM_MASTER;
      for (dest=1; dest<=numworkers; dest++)
      {
         rows = (dest <= extra) ? averow+1 : averow;   	
         //printf("Sending %d rows to task %d offset=%d\n",rows,dest,offset);
         MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
         MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
         MPI_Send(&a[offset][0], rows*NCA, MPI_INT, dest, mtype,
                   MPI_COMM_WORLD);
         //MPI_Send(&b, NCA*NCB, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
         offset = offset + rows;
      }

      /* Receive results from worker tasks */
      mtype = FROM_WORKER;
	//printf("here\n");
      for (i=1; i<=numworkers; i++)
      {
         source = i;
         MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
         MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
         MPI_Recv(&c[offset], 1, MPI_INT, source, mtype, 
                  MPI_COMM_WORLD, &status);
	
         //printf("Received results from task %d\n",source);
      }

      /* Print results */
      printf("******************************************************\n");
      //printf("Result Matrix:\n");
	int m=c[0];
      for (i=1; i<NRA; i++)
      {
         if(m<c[i])
	   m=c[i];
         
      }
	
	
	printf(" norm of matrix= %d\n",m);
	
      printf("\n******************************************************\n");
      printf ("Done.\n");
   

	double final = MPI_Wtime();
	printf("time execution= %lf",final-start);
}
   MPI_Finalize();
}
