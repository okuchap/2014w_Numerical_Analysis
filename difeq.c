#include <stdio.h>
#include <stdlib.h>

#define a 0.0 /* 区間の左端 */
#define b 10.0 /* 区間の右端 */

int main(void)
{

	FILE *fout;
	
	int select; /* 条件分岐用 */
	int i; /* 繰り返し処理用 */
	double c; /* cの値（所与） */
	int n; /* 分割数 */
	double h; /* 刻み幅 */
	double coef; /* y_nに乗ずる係数 */
	
	/* 初期値 */
	double xn = a;
	double yn = 1.0;
	
	
	/* ファイルのオープン */
	if( (fout = fopen("output_difeq.dat","w")) == NULL)
	{
		printf("cannot create the file");
		exit(1);
	}
	
	
	/* 問題の情報を入力 */
	printf("cの値は？(-1 or -10):");
	scanf("%lf", &c);
	
	printf("分割数nは？");
	scanf("%d", &n);
	
	h = (b-a)/n;
	
	printf("区間[%.2f,%.2f]において刻み幅%.2fで常微分方程式を数値的に解く(c=%.2f)\n",a,b,h,c);
	
	
	
	printf("使用する方法を選択: 0.陽的オイラー 1.陰的オイラー 2.台形則\n");
	scanf("%d", &select);
	
	switch(select){
		case 0: coef = 1+c*h; break;
		case 1: coef = 1/(1-c*h); break;
		case 2: coef = (2+c*h)/(2-c*h); break;
	}
	

	fprintf(fout, "%f \t %f\n",xn,yn);
	printf("x=%.3f \t y=%.3f\n",xn,yn);
	
	for(i=0; i<n; i++)
	{
		yn = coef*yn;
		xn += h;
		fprintf(fout, "%f \t %f\n",xn,yn);
		printf("x=%.3f \t y=%.3f\n",xn,yn);
	}

}