
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <time.h>
#include "tempo.h"

#define TAM 150000

int vetor[TAM];
int bottom;
int top;
int swapped;
int numThread;
void inicializa_vetor();
void mostra_vetor();


int main(int argc, char *argv[])
{
	printf("\nDigite o número de Threads desejadas: ");
	scanf("%d",&numThread);
	//printf("\nO método de mostrar foi comentado para ganhar desempenho.");
	
	inicializa_vetor();
	mostra_vetor();
	
	printf("\nIniciando Ordenação - Flags ON\n");
	
	omp_set_num_threads (numThread);
	//omp_set_num_threads (8); //melhor performance com flags
	
    int length,i,j,aux = 0;
    length = TAM;
    bottom = 0;
    top = length - 1;
    swapped = 0; 
    
    tempo1();
    
    while(swapped == 0 && bottom < top)//Se não houver troca de posições ou o ponteiro que
    {                                   //sobe ultrapassar o que desce, o vetor esta ordenado
		
        swapped = 1; 
        
        #pragma omp parallel
		{
			//Este for é a “ida” para a direita "empurrando" o maior valor mais para a direita
			#pragma omp for private(i,aux) 
				for(i = bottom; i < top; i ++)
				{
					if(vetor[i] > vetor[i + 1]) 
					{   						  
						aux=vetor[i];
						vetor[i]=vetor[i+1];
						vetor[i+1]=aux;		
						swapped = 0;
					}
					
				}
				
					/*top = top - 1; diminui o  `top` porque o elemento com o maior valor já está na direita (atual posição top)
									  //foi comentado pois o código paralelizado não funciona com essa linha */	
				 
		  //Este for é a “ida” para a esquerda "empurrando" o menor valor mais para a esquerda
		 #pragma omp for private(i,aux)   
			for(i = top; i > bottom; i --)
			{  if(vetor[i] < vetor[i - 1]) 
				{
					aux=vetor[i];
					vetor[i]=vetor[i-1];
					vetor[i-1]=aux;
							
					swapped = 0;
				}
			}
			
			/*bottom = bottom + 1; aumenta o `bottom` porque o menor valor já está na posição inicial (bottom)
								   foi comentado pois o código paralelizado não funciona com essa linha, uma vez que cada thread vai guardar
								   o menor elemento dentro do seu alcance e somente o menor valor da thread que trabalha com a primeira
								   parte do vetor irá ser salvo, causando erro na ordenação, pois o menor valor pode estar 
								   na última posição do vetor. O mesmo acontece com o top, porém trabalhando com o maior elemento. */
			
		}//fecha parallel	
    }//fecha while 

	tempo2();
	
	printf("\n Vetor Ordenado: \n");
	
	mostra_vetor();

	tempoFinal("mili segundos", argv[0], 0);

}

void mostra_vetor()
{
        int z;
        printf("\n v e t o r \n");
        for (z=0; z < TAM; z ++){
                printf("\n Posicao %d - valor %d", z, vetor[z]);
        }
	printf("\n\n\n");
}

void inicializa_vetor()
{
        int z;
        //printf("\n inicializando vetor ...\n");
        for (z=0; z < TAM; z ++)
                vetor[z] = rand()%100000;
}
