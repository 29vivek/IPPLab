#include<iostream>
#include<omp.h>
using namespace std;


void mergesort(int* a,int start,int end,int nthreads);
void merge(int *a,int left,int mid,int right);

int main()
{
	int n,nthreads=8;
	cout<<"Size of array:\t";
	cin>>n;
	
	int a[n];
	cout<<"Initializing array...\n";
	
	cout<<"Initial array: "<<endl;
	for(int i=0;i<n;i++)
	{
		a[i]=n-i;
		cout<<a[i]<<" ";
	}
	cout<<endl<<endl;
	
	while(nthreads>0)
	{
		double start=omp_get_wtime();
		mergesort(a,0,n-1,nthreads);
		double end=omp_get_wtime();
		cout<<"Number of threads: "<<nthreads<<"\tExecution time: "<<end-start<<endl;
		nthreads/=2;
	}
	
	cout<<"\n\nSorted array: "<<endl;
		for(int i=0;i<n;i++)
			cout<<a[i]<<" ";
		cout<<endl;
	return 0;
}

void mergesort(int* a,int start,int end,int nthreads)
{
	 if(start<end)
	 {
	 	int mid=(start+end)/2;
	 	#pragma omp parallel sections num_threads(nthreads)
	 	{
	 		#pragma omp section
	 		mergesort(a,start,mid,nthreads);
	 		#pragma omp section
	 		mergesort(a,mid+1,end,nthreads);
	 	}
	 	
	 	merge(a,start,mid,end);
	 }
}

void merge(int *a,int left,int mid,int right)
{
	int temp1[mid+1-left],temp2[right-mid];
	
	for(int i=0;i<mid+1-left;i++)
		temp1[i]=a[left+i];

	for(int i=0;i<right-mid;i++)
		temp2[i]=a[mid+i+1];
		
	int i=0,j=0,k=left;
	while(i<mid-left+1 && j<right-mid)
	{
		if(temp1[i]<temp2[j])
			a[k++]=temp1[i++];
		else
			a[k++]=temp2[j++];
	}
	
	while(i<mid-left+1)
		a[k++]=temp1[i++];
		
	while(j<right-mid)
		a[k++]=temp2[j++];
}

// g++ 1.cpp -fopenmp
