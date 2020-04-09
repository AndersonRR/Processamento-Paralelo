#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "tempo.h"

#define L 301
#define C L
#define P L
#define ITER 200

float a[L][C][P];
float b[L][C][P];

void inicializa_cubo();
void *mostra_cubo2(void *arg);
void irriga();
void *propagacao(void *arg);
void evapora();
void move_cubo();
void mostra_cubo();
int NUMTHREAD, divisao;
void propagacao_superficie();

int main(int argc, char *argv[])
{	printf("\n OBS: Digite ./nome do executavel + numero de threads\n");
	
	NUMTHREAD = atoi(argv[1]);
	
	
	if (NUMTHREAD < 1){
		printf("\n OBS: Digite ./nome do executavel + numero de threads\n");
		return 0;
	}
	
	divisao = (int) L / NUMTHREAD;
	
	if (divisao == 1){
			NUMTHREAD = L-1; /*se o número de threads não for suficiente para que divisão seja maior que 1, entao
									é necessário criar threads conforme o tamanho do cubo para que todo ele seja percorrido */
	}
	
	pthread_t 	threads[NUMTHREAD];	

	printf(" Número de Threads: %d\n",NUMTHREAD);
	printf(" Cada thread trabalhará com %d unidades\n",divisao);
	
	int i = 0;
	tempo1();

	printf(" gerando sistema ... \n");
	inicializa_cubo();
	printf(" irrigando ... \n");
	
 	for (i=0; i < ITER; i++) {
		irriga();
		int j, inicio[NUMTHREAD];
		inicio[0] = 0;
		
		//criando as threads
		for (j=0; j < NUMTHREAD; j++){
			pthread_create(&threads[j], NULL, propagacao, (void *)&inicio[j]);
			//printf("\n.:thread %d criada para rodar a partir da posição = %d \n", j, inicio[j]);
			inicio[j+1] = inicio[j] + divisao;
		}
		
		for (j=0; j < NUMTHREAD; j ++)     
			pthread_join(threads[j], NULL);
		
		// para ganhar desempenho, o método propagação superfície executa apenas no final uma vez e não por todas as threads
		propagacao_superficie();
		evapora();
		move_cubo();	
	}
	
	
	//mostra_cubo();
	
	tempo2();

	tempoFinal("mili segundos", argv[0], 0);
}


void *propagacao(void *arg)

{	
	int pos = *((int*)arg);
	int i, j, k, s, x,w = 0;
	
	if (pos == 0)
		w = 1;
		
	if (divisao == L) //se for 1 thread ele vai até lado -1;
			divisao-= 1;							
	
	for (i = pos + w ; i < pos + divisao; i++) {
		for (j = 1 ; j < C-1; j++)
			for (k = 1 ; k < P-1; k++)									
		       b[i][j][k] = ((a[i][j][k] + a[i][j][k-1] + a[i-1][j][k] + a[i][j-1][k] + a[i][j][k+1] + a[i+1][j][k] + a[i][j+1][k] ) / 7);   		     			        
	}
}
void propagacao_superficie()
{
	int s, x;
	//superficie
	for (s = (P/2); s > -1; s--)
		for (x = (C/2); x > -1; x--)
			b[0][s][x] =  ((a[0][s][x] +  a[0][s-1][x] + a[1][s][x] + a[0][s+1][x] + a[0][s][x+1] + a[0][s][x-1]) / 6);
	
	for (s = (P/2)+1; s < P; s++)
		for (x = (C/2)+1; x < C; x++)
			b[0][s][x] =  ((a[0][s][x] +  a[0][s-1][x] + a[1][s][x] + a[0][s+1][x] + a[0][s][x+1] + a[0][s][x-1]) / 6);
	
	for (s = (P/2); s > -1; s--)
		for (x = (C/2) + 1; x < P; x++)
			b[0][s][x] =  ((a[0][s][x] +  a[0][s-1][x] + a[1][s][x] + a[0][s+1][x] + a[0][s][x+1] + a[0][s][x-1]) / 6);		 	   
	
	for (s = (P/2)+1; s < P; s++)
		for (x = (C/2); x > -1; x--)
			b[0][s][x] =  ((a[0][s][x] +  a[0][s-1][x] + a[1][s][x] + a[0][s+1][x] + a[0][s][x+1] + a[0][s][x-1]) / 6);	
}

void evapora()
{	
	int s,x,u;
	
	for (s = 0; s < P; s ++)
		for (x = 0; x < C; x++)
				a[0][s][x] =  a[0][s][x] * 0.78;   
}

void mostra_cubo()
{		
        int z, v, x;

        for (v = 0; v < L; v ++){
			for (x = 0; x < C ; x++){ 
				for (z = 0; z < P ; z ++){
					printf(" %4.3f", a[v][x][z]);
				}
				printf("\n");
			}printf("\n");
        	
		}	
}

void irriga()
{
		a[0][C/2][P/2] = a[0][C/2][P/2] + 2.0 ; 
}


void move_cubo()
{
        int z, v, x;
       
        for (z=0; z < L; z ++)
			for (v=0; v < C; v++)
				for (x=0; x < P; x++){
					a[z][v][x] = b[z][v][x];			
			}
}


void inicializa_cubo()
{
        int z, v, x;
 
        for (z=0; z < L; z ++)
			for (v=0; v < C; v++)
				for (x=0; x < P; x ++){
					a[z][v][x] = 0.0;
					b[z][v][x] = 0.0;			
			}
}



