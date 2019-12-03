#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

int main(){
	float t1,t2;
	int n = 100;
	int m1[n][n],m2[n][n],mn[n][n];
	int i,j,k;
	for (int i=0; i<n; i++) {
		for(int j=0;j<n;j++)
			m1[i][j] = rand() % 50 + 1;
	}
	for (int i=0; i<n; i++) {
		for(int j=0;j<n;j++)
			m2[i][j] = rand() % 100 + 1;
	}
	printf("Matrix 1 ...\n");
	for (int i=0; i<n; i++) {
		for(int j=0;j<n;j++)
			printf("%d ",m1[i][j]);
		printf("\n");
	}
	printf("\n");
	printf("Matrix 2 ...\n");
	for (int i=0; i<n; i++) {
		for(int j=0;j<n;j++)
			printf("%d ",m2[i][j]);
		printf("\n");
	}
//	omp_set_num_threads(8);
// 	change this
	float start = omp_get_wtime();
	#pragma omp parallel for shared(m1,m2) private(i,j,k)
	for(i=0;i<n;i++) { 
		for(j=0;j<n;j++) {
			mn[i][j] = 0;
			for(k=0;k<n;k++){
				mn[i][j] += m1[i][k]*m2[k][j];
			}
		}
	}
	float end = omp_get_wtime();
	printf("\n");
	printf("Matrix n ...\n");
	for (int i=0; i<n; i++){
		for(int j=0;j<n;j++)
			printf("%d ",mn[i][j]);
		printf("\n");
	}
	printf("\n");
	printf("Execution Time : %f\n",end-start);
}

/* 
	gcc 2.c -fopenmp 
*/
