#include<stdio.h>
#include<stdlib.h>
#include<gd.h>
#include<omp.h>
#include<string.h>

int main(int argc,char* argv[])
{
	if(argc<4)
	{
		printf("format:input_file out_file num_threads\n");
		return 1;
	}
	
	char* input=argv[1];
	char* output=argv[2];
	int numthreads=atoi(argv[3]);
	int color,x,y;
	int red,green,blue,average;
	FILE *fp;
	fp=fopen(input,"r");
	if(fp==NULL)
	{
		printf("error opening in the file\n");
		return 1;
	}
	gdImagePtr img;
	img=gdImageCreateFromPng(fp);
	int width=gdImageSX(img);
	int height=gdImageSY(img);
	double t,s;
	t=omp_get_wtime();
	#pragma omp parallel for schedule(dynamic) private(red,green,blue,color,y,average) num_threads(numthreads)
	for(x=0;x<width;x++)
	{
		for(y=0;y<height;y++)
		{
			color=gdImageGetPixel(img,x,y);
			red=gdImageRed(img,color);
			green=gdImageGreen(img,color);
			blue=gdImageBlue(img,color);
			average=(red+green+blue)/3;
			color=gdImageColorAllocate(img,average,average,average);
			gdImageSetPixel(img,x,y,color);
		}
	}
	s=omp_get_wtime()-t;
	fp=fopen(output,"w");
	if(fp==NULL)
	{
		printf("error opening the output file\n");
		return 1;
	}
	gdImagePng(img,fp);
	gdImageDestroy(img);
	fclose(fp);
	printf("file size is %dx%d\n",width,height);
	printf("time taken for %d threads id %lf seconds",numthreads,s);
return 0;
}

// gcc 6.c -lgd -fopenmp
// ./a.out input.png output.png 4
