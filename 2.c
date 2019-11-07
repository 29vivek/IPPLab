#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
int main(){
	float t1,t2;
	int m1[3][3],m2[3][3],m3[3][3];
	int i,j,k;
	for (int i=0; i<3; i++) {
		for(int j=0;j<3;j++)
			m1[i][j] = rand() % 50 + 1;
	}
	for (int i=0; i<3; i++) {
		for(int j=0;j<3;j++)
			m2[i][j] = rand() % 100 + 1;
	}
	printf("Matrix 1 ...\n");
	for (int i=0; i<3; i++) {
		for(int j=0;j<3;j++)
			printf("%d ",m1[i][j]);
		printf("\n");
	}
	printf("\n");
	printf("Matrix 2 ...\n");
	for (int i=0; i<3; i++) {
		for(int j=0;j<3;j++)
			printf("%d ",m2[i][j]);
		printf("\n");
	}
//	omp_set_num_threads(8);
// 	change this
	float start = omp_get_wtime();
	#pragma omp parallel for shared(m1,m2) private(i,j,k)
	for(i=0;i<3;i++) {
		for(j=0;j<3;j++) {
			m3[i][j] = 0;
			for(k=0;k<3;k++){
				m3[i][j] += m1[i][k]*m2[k][j];
			}
		}
	}
	float end = omp_get_wtime();
	printf("\n");
	printf("Matrix 3 ...\n");
	for (int i=0; i<3; i++){
		for(int j=0;j<3;j++)
			printf("%d ",m3[i][j]);
		printf("\n");
	}
	printf("\n");
	printf("Execution Time : %f",end-start);
}

/* 
	gcc 2.c -fopenmp 
*/
