#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<omp.h>
#include<string.h>

int* primetable(int primenum)
{
	int i,j,p;
	int prime;
	int* primes;
	primes=(int*)malloc(sizeof(int)*primenum);
	i=2;
	p=0;
	while(p<primenum)
	{
		prime=1;
		for(j=2;j<i;j++)
		{
			if(i%j==0)
			{
				prime=0;
				break;
			}
		}
		if(prime)
		{
			primes[p]=i;
			p++;
		}
		i=i+1;
	}
	for(int i=0;i<primenum;i++)
	{
		printf("%d ",primes[i]);
	}
	return primes;
}

double* sinetable(int sinenum)
{
	int i;
	double sine;
	double* sines;
	sines=(double*)malloc(sizeof(double)*sinenum);
	double pi=3.1415926;
	int j;
	for(i=0;i<sinenum;i++)
	{
		sines[i]=0;
		for(j=0;j<=i;j++)
		{
			sine=(double)(j)*pi/(double)(sinenum-1);
			sines[i]=sines[i]+sin(sine);
		}
		
	}
	printf("\n");
	for(int i=0;i<sinenum;i++)
	{
		printf("%lf ",sines[i]);
	}
	return sines;
}

int main()
{
	int i,j;
	double* b;
	int* a;
	int n;
	printf("enter the value of n\n");
	scanf("%d",&n);
	b=(double*)malloc(sizeof(double)*n);
	a=(int*)malloc(sizeof(int)*n);
	double t,s;
	int k,z;
	z=0;
	for(k=1;k<=8;k=pow(2,z))
	{
		t=omp_get_wtime();
		#pragma omp parallel sections num_threads(k)
		{
			#pragma omp section
				a=primetable(n);
			#pragma omp section
				b=sinetable(n);
		}
		s=omp_get_wtime()-t;
		z++;
		printf("\n");
		printf("numthreads:%d -------->time:%lf\n",k,s);
	}
	return 0;
}

/* 
	gcc 3.c -fopenmp -lm
*/
