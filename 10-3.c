﻿#include <stdio.h>
#include <stdlib.h>

#define N 8

int main(void)
{
	double *a;
	int i;
	
	if( (a=malloc(sizeof(double)*N) ) == NULL)
	{
		printf("メモリが確保できず(vector a)\n");
		exit(1);
	}
	
	for(i=0; i<N ;i++) a[i] = 2.0*i;
	
	for(i=0; i<N; i++)
	{
		printf("a[%d] = %f\n", i, a[i]);
	}
	
	free(a);
	
	return 0;
	
	
}