#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<omp.h>
#include<string.h>
#define NAME "word.txt"
#define count 4
char search[count][20]={"hi","naga","helo","bye"};

int d[count];

int isalphaa(char c)
{
	return(((c>=65)&&(c<=90)) ||((c>=97)&&(c<=122)));
}

int isequal(char *a, char const *key)
{
	char b[20];
	strcpy(b,key);
	int i;
	int lena=strlen(a);
	int lenb=strlen(b);
	if(lena!=lenb)
		return 0;
	for(i=0;i<lena;i++)
	{
		if(a[i]>90)
			a[i]-=32;
		if(b[i]>90)
			b[i]-=32;
	}
	return(strcmp(a,b)==0);
}

void readword(FILE *fp,char *temp)
{
	char c=fgetc(fp);
	int i=0;
	while(c!=EOF && isalphaa(c)!=0)
	{
		temp[i++]=c;
		c=fgetc(fp);
	}
	temp[i]='\0';
}

int determine(char const *file,char const *key)
{
	int wcount=0;
	FILE *fp=fopen(file,"r");
	char temp[40];
	while(feof(fp)==0)
	{
		readword(fp,temp);
		if(isequal(temp,key)!=0)
			wcount++;
	}
	return wcount;
}


int main()
{
	int nt,i;
	for(i=0;i<count;i++)
	{
		d[i]=0;
	}
	
	double t,s;
	int z=0;
	for(nt=1;nt<=8;nt=pow(2,z))
	{
		t=omp_get_wtime();
		#pragma omp parallel for num_threads(nt)
			for(i=0;i<count;i++)
				d[i]=determine(NAME,search[i]);
		s=omp_get_wtime()-t;
		z++;
		printf("time taken:%lf----->numthreads:%d\n",s,nt);
		for(i=0;i<count;i++)
	{
		printf("%s------%d\n",search[i],d[i]);
	}
	}
	
	return 0;
}

/* 
	gcc 4.c -fopenmp -lm
	create a word.txt file with words
*/
