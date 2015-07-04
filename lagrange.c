/* 大問２　ラグランジュ補間 */
/* 補間結果を0.01(CUTの値)刻みで出力してグラフに */

#include <stdio.h>
#include <stdlib.h>

#define CUT 0.01 /* グラフの刻み幅 */

/* ベクトルの入力 */
/* b[m...n]の入力 */

void input_vector3( double *b, int m, int n, FILE *fin)
{
	int i;
	for (i=m; i<=n; i++)
	{
		fscanf(fin, "%lf", &b[i]);
	}
}

/* ベクトル領域の確保 */
double *dvector(int i,int j)
{
	double *a;
	
	if( (a = malloc( ((j-i+1)*sizeof(double))) ) == NULL )
	{
		printf("メモリが解放できません。(from dvector)\n");
		exit(1);
	}
	
	return(a-i);
}

/* 領域の解放 */
void free_dvector(double *a, int i)
{
	free( (void *)(a + i) );   /*void *型へのキャストが必要*/
}

/* ラグランジュ補間 */
double lagrange( double *x, double *y, int m, int n, double xi )
{
	int i,k;
	double pn = 0.0, li;
	
	/* P_n(x)の計算 */
	for (i=m; i<=n; i++)
	{
		li = 1.0;
		/* l_i(x)の計算 */
		for(k=m; k<=n; k++)
		{
			if(k != i) li *= (xi - x[k]) / (x[i] - x[k]);
		}
		pn += li * y[i];
	}
	
	return pn;
}


int main(void)
{
	FILE *fin, *fout;
	double *x, *y, xi;
	int n;
	
	printf("データの個数を入力:\n");
	scanf("%d", &n);
	n -= 1; /*データ数がnなので、添え字を0,1,...,nとする。*/
	
	
	/* ベクトルの領域確保 */
	x = dvector(0,n);
	y = dvector(0,n);
	
	/* ファイルのオープン */
	if( (fin = fopen("input_lag.dat", "r") ) == NULL)
	{
		printf("file NOT found: input_lag.dat\n");
		exit(1);
	}
	
	if( (fout = fopen("output_lag.dat","w") ) == NULL)
	{
		printf("cannot create the file: output_lag.dat");
		exit(1);
	}
	
	input_vector3(x,0,n,fin); /* vector x の入出力 */
	input_vector3(y,0,n,fin); /* vector y の入出力 */
	
	
	/* グラフを描くために、結果をファイルに出力 */
	for(xi=x[0]; xi<=x[n]; xi+=CUT)
	{
		fprintf(fout, "%f \t %f\n", xi, lagrange(x,y,0,n,xi) );
	}
	
	fclose(fin); fclose(fout);
	
	/* 領域解放 */
	free_dvector(x,0); free_dvector(y,0);

	return 0;

}

