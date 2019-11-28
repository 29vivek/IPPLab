#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void ringIO(int p, int r) {
    int messageSizes[4] = {100, 1000, 10000, 100000};
    int *m;
    int source, dest;
    double t1, t2, tAvg, tMin, tMax, t;
    MPI_Status status;

    for(int i=0; i<4; i++) {
        tAvg = 0;
        tMin = 10; // some random max value
        tMax = 0;
        m = (int *) malloc(sizeof(int)*messageSizes[i]);
        if(r == 0) {
            dest = 1;
            source = p-1;
            
            for(int j=0; j<10; j++) {
                t1 = MPI_Wtime();
                MPI_Send(m, messageSizes[i], MPI_INT, dest, 0, MPI_COMM_WORLD);
                MPI_Recv(m, messageSizes[i], MPI_INT, source, 0, MPI_COMM_WORLD, &status);
                t2 = MPI_Wtime();
                t=t2-t1;
                tAvg+=t;
                if(t < tMin) {
                    tMin = t;
                }
                if(t > tMax) {
                    tMax = t;
                }
            }
            tAvg/=10;
            printf("ringio of %d integers is: %0.6f, max: %.6f, min: %.6f\n", messageSizes[i], tAvg, tMax, tMin);
        } else {
            dest = (r+1)%p;
            source = r-1;
            for(int j=0; j<10; j++) {
                MPI_Recv(m, messageSizes[i], MPI_INT, source, 0, MPI_COMM_WORLD, &status);
                MPI_Send(m, messageSizes[i], MPI_INT, dest, 0, MPI_COMM_WORLD);
            }
        }
    }
}


int main(int argc, char* argv[]) {
    int rank, processes;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    ringIO(processes, rank);
    
    MPI_Finalize();
    return 0;
}