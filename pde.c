/* 課題２　定常移流拡散方程式 */
/* 前進差分・風上差分の計算を実行した際、最後にAborted(コアダンプ)と出てしまうのがどうしても解消できなかった。
* 計算の実行及び実行結果の出力は問題なくできたので、そのまま提出します。*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NE 10 /* 分割数 */
#define N NE+1 /* 格子点の数＝分割数＋１ */
#define DIFF 0.01 /* 拡散係数 */
#define VELO 1.0 /* 流速。今回は固定 */


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

/* Gaussの消去法 部分ピボット */

double *gauss(double **a, double *b)
{
	int i,j,k,ip;
	double alpha,tmp;
	double amax,eps =pow(2.0, -50.0);
	
	for(k=1; k<=N; k++)
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


/* メイン関数 */
int main(void)
{
	double DX = 1.0/NE; /* ステップ幅 */
	double PECLET = VELO/DIFF; /* ペクレ数 */
	double Rc = VELO*DX/DIFF; /* レイノルズ数 */
	double coef1, coef2, coef3; /* 差分方程式の係数 */
	
	
	int i,j; /* 条件分岐用 */
	int select; /* switch文用 */
	FILE *fout;
	
	double* u; /* 解 */
	u = dvector(1,N);
	
	/* ファイルのオープン */
	if((fout = fopen("output.dat","w"))==NULL)
	{
		printf("cannot create the file: output.dat \n");
		exit(1);
	}
		
	
	printf("実行方法を選択: 1:真の解 2:中央差分 3:風上差分");
	scanf("%d", &select);
	
	switch(select){
		case 1: /* 真の解 (exp(PECLET*x)-1.0)/(exp(PECLET)-1.0) */
			u[1]=0; u[N]=1.0; /* 境界条件 */
			
			/* 解を計算 */
			for(i=2; i<N; i++){
				u[i]=(exp(PECLET*(i-1)*DX)-1.0)*(1.0/(exp(PECLET)-1.0));
			}
			
			/* 解を出力 */
			for(i=1;i<=N;i++){
				printf("%f\t%f\n", DX*(i-1), u[i]); /* x u の順番で並ぶ*/
				fprintf(fout, "%f\t%f\n", DX*(i-1), u[i]);
			}
			
			fclose(fout);
			free_dvector(u,1);
			return 0;
		
		case 2: /* 中央差分 */
			/* coef1*u_(i-1) + coef2*u_i + coef3*u_(i+1) = 0 となるように設定 */
			coef1 = 2.0+Rc; coef2 = -4.0; coef3 = 2.0-Rc;
			break;
		
		case 3: /* 風上差分 */
			coef1 = 1.0+Rc; coef2 = -2.0-Rc; coef3 = 1.0;
			break;
		}
		
	/* 領域確保・初期化 */
	double **A;
	double *b;
	A = dmatrix(1,N,1,N);
	b = dvector(1,N);
	
	for(i=1; i<=N; i++){
		for(j=1; j<=N; j++){
			A[i][j]=0.0;
		}
	}
	
	for(i=1; i<=N; i++){
		b[i]=0.0;
	}
	
	/* 境界条件 */
	A[1][1]=1.0; A[N][N]=1.0;
	b[1]=0.0; b[N]=1.0;
	
	/* 差分方程式の係数をAに代入 */
	for(i=2; i<N; i++){
		A[i][i-1] =coef1;
		A[i][i]   =coef2;
		A[i][i+1] =coef3;
	}
	
	u = gauss(A,b); /* 連立方程式を解く */
		
	/* 解を出力 */
	for(i=1;i<=N;i++){
		printf("%f\t%f\n", DX*(i-1), u[i]); /* x u の順番で並ぶ*/
		fprintf(fout, "%f\t%f\n", DX*(i-1), u[i]);
	}
	
	/* 領域解放・ファイルのクローズ */
	fclose(fout);
	free_dmatrix(A,1,N,1,N); free_dvector(b,1); free_dvector(u,1);
	return 0;

}