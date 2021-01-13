#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <sys/time.h>
//#include <assert.h>

#define N 2048
#define QTD_GERACOES 2000
#define SRAND_VALUE 1985
#define QTD_THREADS 2


int **AlocaMatriz(int m, int n); //Retorna uma matriz alocada dinamicamente com dimensoes m x n
void LiberaMatriz(int **M, int n); //Libera a memoria usada pela matriz
void PrintarMatriz(int **M, int n); // Printa todos elementos de uma matriz n por n.
void NovaGeracao(int **nova_geracao, int **M, int n); //Computa a nova geracao de 'M' em 'nova_geracao'
int getNeighbors(int** M, int i, int j, int n); // Retorna a quantidade de vizinhos vivos do elemento M[i][j], considerando a matriz com BORDAS INFINITAS
int QtdCelulasVivas(int **M,int n); //Retorna a quantidade de celulas vivas na matriz
int resultado; //Variavel global que conta a quantidade de celulas vivas na geracao atual.

int main() {

    long int i,j;
    int **grid; // Matriz que representa a geracao atual
    int **newgrid; // Matriz que representa a proxima geracao.
    int **troca_aux; //Variavel auxiliar para troca de matrizes.
    struct timeval tempo_inicial ;
    struct timeval tempo_final ;


    grid = AlocaMatriz(N,N); //Criando a nova matriz dinamicamente
    newgrid = AlocaMatriz(N,N); //Criando a matriz da nova geracao
    srand(SRAND_VALUE); // Valor definido para gerar uma matriz aleatoria
    omp_set_num_threads(QTD_THREADS);

    gettimeofday ( & tempo_inicial, NULL ) ;

// Definindo valores aleatorios a matriz utilizando omp
//#pragma omp parallel private(j)
//#pragma omp for
{
    for (i=0; i<N; i++)
        for(j=0; j<N; j++)
            grid[i][j] = rand() % 2;}


    printf("\n Condicao 0: %d",QtdCelulasVivas(grid,N));
    for(i=0;i<QTD_GERACOES;i++){
        NovaGeracao(newgrid,grid,N);
        troca_aux = grid;
        grid = newgrid;
        newgrid = troca_aux;

        printf("\n Geracao %ld: %d",i+1,QtdCelulasVivas(grid,N));
    }
    gettimeofday ( & tempo_final , NULL ) ;

    printf("\nseconds : %ld\nmicro seconds : %ld",tempo_final.tv_sec - tempo_inicial.tv_sec, tempo_final.tv_usec - tempo_inicial.tv_usec);

    LiberaMatriz(grid,N);
    LiberaMatriz(newgrid,N);
}

int **AlocaMatriz(int m, int n){
  int **M;
  int i;

  M = (int **)malloc(sizeof(int *)*m);
  if(M == NULL){
    printf("Memoria insuficiente.\n");
    exit(1);
  }
  for(i = 0; i < m; i++){
    M[i] = (int *)malloc(sizeof(int)*n);
    if(M[i] == NULL){
      printf("Memoria insuficiente.\n");
      exit(1);
    }
  }
  return M;
}

void LiberaMatriz(int **M, int n){
  int i;
  for(i = 0; i < n; i++)
    free(M[i]);
  free(M);
}

void PrintarMatriz(int **M, int n){
  int i,j;

  for(i = 0; i<n; i++){
    for(j = 0; j<n; j++) printf("%d ",M[i][j]);
    printf("\n");
  }
}

int getNeighbors(int** M, int i, int j, int n){
    int qtd_vizinhos = 0;
    int superior, inferior, esquerda, direita;

    superior = i - 1;
    inferior = i + 1;
    esquerda = j - 1;
    direita = j + 1;

    if(i == 0) superior = n-1; // Faz o ajuste necessario para vizinhos superiores caso a celula esteja na primeira linha
    if(i == n-1) inferior = 0; // Faz o ajuste necessario para vizinhos inferiores caso a celula esteja na ultima linha
    if(j == 0) esquerda = n-1;  // Faz o ajuste necessario para vizinhos a esquerda caso a celula esteja na primeira coluna
    if(j == n-1) direita = 0;  // Faz o ajuste necessario para vizinhos a direita caso a celula esteja na ultima coluna

    if( M[superior][esquerda] == 1) qtd_vizinhos++;
    if( M[superior][j] == 1) qtd_vizinhos++;
    if( M[superior][direita] == 1) qtd_vizinhos++;
    if( M[i][esquerda] == 1) qtd_vizinhos++;
    if( M[i][direita] == 1) qtd_vizinhos++;
    if( M[inferior][esquerda] == 1) qtd_vizinhos++;
    if( M[inferior][j] == 1) qtd_vizinhos++;
    if( M[inferior][direita] == 1) qtd_vizinhos++;

    return qtd_vizinhos;

}

void NovaGeracao(int **nova_geracao, int **M, int n){
    int i,j;
    int qtd_vizinhos_vivos;


#pragma omp parallel private(j,qtd_vizinhos_vivos)
{
#pragma omp for    
    for (i=0; i<N; i++){
        for(j=0; j<N; j++){
            qtd_vizinhos_vivos = getNeighbors(M, i, j, n);

            if(M[i][j] == 1){
               if(qtd_vizinhos_vivos < 2)  nova_geracao[i][j] = 0;
               if(qtd_vizinhos_vivos == 2) nova_geracao[i][j] = 1;
               if(qtd_vizinhos_vivos == 3) nova_geracao[i][j] = 1;
               if(qtd_vizinhos_vivos >= 4) nova_geracao[i][j] = 0;
            }
            else{
                if(qtd_vizinhos_vivos == 3) nova_geracao[i][j] = 1;
                else nova_geracao[i][j] = 0;
            }

        }
    }
}
}

int QtdCelulasVivas(int **M,int n){
    int i,j;
    resultado =	 0;
#pragma omp parallel private(j)
{
#pragma omp for
    for (i=0; i<N; i++)
        for(j=0; j<N; j++)
	#pragma omp critical
            resultado = resultado + M[i][j];
}

    return resultado;
}
