
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


/* ベクトルの入力その２ */
/* b[m...n]の入力 */

void input_vector3( double *b, int m, int n, FILE *fin)
{
	int i;
	for (i=m; i<=n; i++)
	{
		fscanf(fin, "%lf", &b[i]);
	}
}


/* エラーチェック用 */
fprintf(stderr, "check\n");



