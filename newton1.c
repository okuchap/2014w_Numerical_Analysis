/* 大問１　Newton法 */


#include <stdio.h>
#include <math.h>

#define EPS pow(10.0,-8.0) /* 打ち切りのためのepsilon */
#define NMAX 20 /* 最大反復回数 */
#define a 0.36 /* a^(1/3)を求める */


/* fとdfの定義 */
double f(double t)
{
	return t*t*t - a;
}

double df(double t)
{
	return 3*t*t;
}


/* Newton法 */
void newton(double x)
{

	int n=0;
	double d;
	
	do
	{
		d = -f(x)/df(x);
		x = x + d;
		printf("%d反復目の値: %.6f\n", n+1, x);
		n++;
	}while( fabs(d) > EPS && n < NMAX);

	if(n==NMAX)
		printf("failure");
	else
		printf("収束した値: %f\n", x);

}

int main(void)
{
	
	newton(a);
	
	return 0;

}

