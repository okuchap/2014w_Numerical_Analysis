#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define N 4 /* N次正方行列 */
	
	
/* Gaussの消去法 */

double *gauss(double **a, double *b)
{
	int i,j,k,ip;
	double alpha,tmp;
	double amax,eps =pow(2.0, -50.0);
	
	for(k=1; k<=N; k++) /* ここがおかしい？？？ */
	{
		/* ピボットの選択 */
		amax = fabs(a[k][k]); ip=k;
		for(i=k+1; i<=N; i++){
			if(fabs(a[i][k]) >amax){
				amax= fabs(a[i][k]); ip=i;
			}
		}
		
		/* 正則性の判定 */
		if(amax < eps) printf("not正則\n");
		
		/* 行交換 */
		if(ip!=k){
			for(j=k; j<=N; j++){
				tmp = a[k][j]; a[k][j]=a[ip][j]; a[ip][j]=tmp;
			}
			tmp = b[k]; b[k]=b[ip]; b[ip]=tmp;
		}
		
		/* 前進消去 */
		alpha = 1.0/a[k][k];
		for(j=k; j<=N; j++){
			a[k][j] = alpha*a[k][j]; /* k行目を割る。a[k][k]=1に。 */
		}
		b[k] = alpha*b[k];
		
		for(j=k+1; j<=N; j++){
			tmp = a[j][k];
			for(i=k; i<=N; i++){
				a[j][i]= a[j][i] - tmp*a[k][i];
			}
			b[j]= b[j] - tmp*b[k];
		}
	}
	
	/* エラーチェック用 */
	printf("前進消去後の行列Aの中身は\n");
	for(k=1; k<=N; k++){
		for(j=1; j<=N; j++){
			printf("%5.2f\t", a[k][j]);
		}
		printf("\n");
	}
	
	
	/* 後退代入 */
	for(k=N-1; k>=1; k--){
		tmp = 0.0;
		for(j=k+1; j<=N; j++){
			tmp = tmp + a[k][j]*b[j];
		}
		b[k]= b[k] - tmp;
	}
	
	return b;
}


/* 行列の入力 */
void input_matrix(double **a, char c, FILE *fin, FILE *fout)
{
	int i,j;
	
	fprintf(fout, "行列%cは次の通りです。\n", c);
	for(i=1; i<=N; i++)
	{
		for(j=1; j<=N; j++)
		{
			fscanf(fin, "%lf", &a[i][j]);
			fprintf(fout, "%5.2f\t", a[i][j]);
		}
		fprintf(fout, "\n");
	}
}

	
	
/* 行列の領域確保 */
double **dmatrix(int nr1, int nr2, int nl1, int nl2)
{
	int i, nrow, ncol;
	double **a;
	
	nrow = nr2 - nr1 + 1; /* 行数 */
	ncol = nl2 - nl1 + 1; /* 列数 */
	
	/* 行の確保 */
	if((a = malloc(nrow*sizeof(double *))) == NULL)
	{
		printf("メモリが確保できません(行列a) \n");
		exit(1);
	}
	
	a = a - nr1; /* 行をずらす */
	
	/* 列の確保 */
	for( i=nr1; i<=nr2; i++) a[i] = malloc(ncol*sizeof(double));
	for( i=nr1; i<=nr2; i++) a[i] = a[i] - nl1; /* 列をずらす */
	
	return(a);
	
}

/* 行列の領域解放 */
void free_dmatrix(double **a, int nr1, int nr2, int nl1, int nl2)
{
	int i;
	
	/* メモリの解放 */
	for(i=nr1; i<=nr2; i++) free((void *)(a[i]+nl1));
	free((void *)(a+nr1));
}


/* ベクトルの入力 */
void input_vector(double *b, char c, FILE *fin, FILE *fout)
{
	int i;
	
	fprintf(fout, "ベクトル%cは次の通りです。\n",c);
	for(i=1; i<=N; i++)
	{
		fscanf(fin, "%lf", &b[i]);
		fprintf(fout, "%5.2f\t", b[i]);
		fprintf(fout,"\n");
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

/* ベクトル領域の解放 */
void free_dvector(double *a, int i)
{
	free( (void *)(a + i) );   /*void *型へのキャストが必要*/
}


/* メイン関数 */
int main(void)
{
	FILE *fin, *fout;
	double **a, *b;
	int i;
	
	a = dmatrix(1,N,1,N);
	b = dvector(1,N);
	
	/* ファイルのオープン */
	if((fin = fopen("input.dat", "r"))==NULL)
	{
		printf("file not found: input.dat \n");
		exit(1);
	}
	
	if((fout = fopen("output.dat","w"))==NULL)
	{
		printf("cannot create the file: output.dat \n");
		exit(1);
	}
	
	/* Gaussの消去法 */
	input_matrix(a,'A',fin, fout);
	input_vector(b,'b',fin, fout);
	
	
	
	b = gauss(a,b);
	
	/* 結果の出力 */
	fprintf(fout, "Ax=bの解は次の通りです。\n");
	for(i=1; i<=N; i++){
		fprintf(fout, "%f\n", b[i]);
	}
	
	fclose(fin); fclose(fout);
	free_dmatrix(a,1,N,1,N); free_dvector(b,1);

	return 0;

}