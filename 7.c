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
	int color,x,y,i;
	int red,green,blue;
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
	#pragma omp parallel for schedule(dynamic) private(red,green,blue,color,y) num_threads(numthreads)
	for(x=0;x<width;x++)
	{
	#pragma omp critical
		for(y=0;y<height;y++)
		{
			color=gdImageGetPixel(img,x,y);
			red=255-gdImageRed(img,color);
			green=255-gdImageGreen(img,color);
			blue=255-gdImageBlue(img,color);
			color=gdImageColorAllocate(img,red,green,blue);
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
