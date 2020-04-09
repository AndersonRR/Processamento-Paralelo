
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include "tempo.h"

int topDown(long int *start, long int *end);
int bottomUp();
long TAM;

int main(int argc,char *argv[])
{
	
    MPI_Status status;
    int  myid, numprocs;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	
	

    if (myid == 0) 
    {
			tempo1();
			FILE *file;
			file = fopen("texto.txt","r");	  

			fseek(file, 0L, SEEK_END); // move para o final para saber o tamanho
			TAM = ftell(file);
			fclose(file);
			
			long divisao = TAM/(numprocs);
			long auxDivisao = TAM % (numprocs); //recebe o resto da divisao e soma no primeiro processo
			long vetaux [numprocs];
			vetaux[0] = 0L;
			long vetDeOrdem[2], vetDeOrdemFinal[3];
			int w, vetDevolvido[2];
		
			for (w = 0; w < numprocs; w++){
				vetaux[w+1] = vetaux[w] + (divisao) + auxDivisao;
				auxDivisao = 0L;
			}
			

    		int  m, auxResto, i, mestreRes, mestreResBot,contTop = 0, contBot = 0, td1 = 0, bu1 = 0, td2 = 0, bu2 = 0;
    		printf("\nNumero divisão do trabalho: %ld caracteres\n", divisao);
			printf("\nNumero total de processos: %d\n\n", numprocs);
			
					for (i=1; i < numprocs; i++){//mestre 0 envia para todos (1 ate n-1) escravos em ordem de i 
						vetDeOrdem[0] = vetaux[i]+1;
						vetDeOrdem[1] = vetaux[i+1];
						MPI_Send(&vetDeOrdem[0], 2, MPI_LONG, i, 4, MPI_COMM_WORLD);
						}
						
				//parte em que o mestre começar a contar
				auxResto = vetaux[1] % 2;
				vetDeOrdemFinal[0] = vetaux[0];
				auxResto = vetaux[1] % 2;
				vetDeOrdemFinal[1] = ((vetaux[1] - vetaux[0]) / 2) + auxResto + vetaux[0]; //vai até a metade mais o resto em casos ímpares
				vetDeOrdemFinal[2] = vetaux[1];
				
				omp_set_num_threads (2);	
				#pragma omp parallel
					 #pragma omp sections 				// cada secao roda em uma thread
					 {
						#pragma omp section
						{ 	
							td1 = topDown(&vetDeOrdemFinal[0], &vetDeOrdemFinal[1]);
							bu1 = bottomUp(&vetDeOrdemFinal[0], &vetDeOrdemFinal[1]);
							}
						#pragma omp section
						{ 	
							td2 = topDown(&vetDeOrdemFinal[1], &vetDeOrdemFinal[2]);
							bu2 = bottomUp(&vetDeOrdemFinal[1], &vetDeOrdemFinal[2]);
							}
					 }
					
					//mestre recebe de todos os escravos em qualquer ordem
					for (i=1; i < numprocs; i++){
						MPI_Recv(vetDevolvido, 2, MPI_INT, MPI_ANY_SOURCE, 4, MPI_COMM_WORLD, &status);
						printf("mestre recebeu do slave %d: TopDown = %d | BottomUp = %d\n",status.MPI_SOURCE,vetDevolvido[0],vetDevolvido[1]);
							contTop += vetDevolvido[0];
							contBot += vetDevolvido[1];
						}
				
			
			printf("\nMestre contou %d ocorrências TopDown e %d BottomUp.\n",td1 + td2, bu1 + bu2);
			printf("\nTotal na Pesquisa TopDown: %d\n",contTop + td1 + td2);
			printf("\nTotal na Pesquisa BottomUp: %d\n",contBot + bu1 + bu2);
			tempo2();
			printf("Tempo: ");
			tempoFinal("", argv[0], 0);

    }else{
			
			long vetDeOrdem2[2], vetDeOrdemFinal[3];
    		int i, vetDeDevolucao[2], auxResto = 0, td1 = 0, bu1 = 0, td2 = 0, bu2 = 0;
            
            MPI_Recv(vetDeOrdem2, 2, MPI_LONG, 0, 4, MPI_COMM_WORLD, &status);
			printf("slave %d começa em %ld e termina em %ld.\n", myid, vetDeOrdem2[0], vetDeOrdem2[1]);
			
			vetDeOrdemFinal[0] = vetDeOrdem2[0];
			auxResto = vetDeOrdem2[1] % 2;
			vetDeOrdemFinal[1] = ((vetDeOrdem2[1] - vetDeOrdem2[0]) / 2) + auxResto + vetDeOrdem2[0]; //vai até a metade mais o resto em casos ímpares
			vetDeOrdemFinal[2] = vetDeOrdem2[1];
			
			
			omp_set_num_threads (2);
			
			#pragma omp parallel
				 #pragma omp sections 				// cada secao roda em uma thread
				 {
					#pragma omp section
					{ 	
						td1 = topDown(&vetDeOrdemFinal[0], &vetDeOrdemFinal[1]);
						bu1 = bottomUp(&vetDeOrdemFinal[0], &vetDeOrdemFinal[1]);
						}
					#pragma omp section
					{ 	
						td2 = topDown(&vetDeOrdemFinal[1], &vetDeOrdemFinal[2]);
						bu2 = bottomUp(&vetDeOrdemFinal[1], &vetDeOrdemFinal[2]);
						}
				 }
	

			vetDeDevolucao[0] = td1 + td2;
			vetDeDevolucao[1] = bu1 + bu2;
				
            MPI_Send(&vetDeDevolucao[0], 2, MPI_INT, 0, 4, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}

int topDown(long *start, long *end){
	
	long inicio,fim;
	inicio = *start;
	fim = *end;
	
	
	FILE *file;
	file = fopen("texto.txt","r");	  

	int ola =0, num = 0;

	char code[4] = {"DHGK"}, c, buf[4];
	
	fseek(file, (long)inicio, SEEK_SET);
	
	c=getc(file);  // leitura do primeiro char do arquivo
	buf[0] = c;
	
	c=getc(file);  // leitura do segundo char do arquivo
	buf[1] = c;
	
	c=getc(file);  // leitura do terceiro char do arquivo
	buf[2] = c;
	
	long count;
	
	for(count = inicio + 4L;count <= fim+3 && buf[3] != EOF;count++){
	
		buf[3] = getc(file);

		if ((code[0] == buf[0]) 
		 && (code[1] == buf[1]) 
		 && (code[2] == buf[2]) 
		 && (code[3] == buf[3])){
			num ++;
		}
		
		buf[0] = buf[1];
		buf[1] = buf[2];
		buf[2] = buf[3];
	
	}
	
	fclose(file);
	return num;
}

int bottomUp(long *start, long *end){
	
	long inicio,fim, mover = 0L;
	inicio = *start;
	fim = *end;

	FILE *file;
	file = fopen("texto.txt","r");	  

	fseek(file, 0L, SEEK_END);
	long sz = ftell(file);
	
	mover = sz - fim; 

	int num = 0, incremento = 0;
	
	if (inicio != 0)
		incremento = 3;

	char code[4] = {"DHGK"}, c, buf[4];
	
	
	fseek(file, -(1L + mover), SEEK_END);
	
	c=getc(file);  // leitura do primeiro char do arquivo de trás pra frente
	buf[0] = c;

	fseek(file, -(2L + mover), SEEK_END);
	
	c=getc(file);  // leitura do segundo char do arquivo de trás pra frente
	buf[1] = c;

	fseek(file, -(3L + mover), SEEK_END);
	
	c=getc(file);  // leitura do terceiro char do arquivo de trás pra frente
	buf[2] = c;
	
	long count;
	count = fim - 4L;
	mover += 4L;
	
	while(count >= inicio + incremento){
		
		fseek(file, -mover, SEEK_END);
	
		buf[3] = getc(file);

		if ((code[0] == buf[0]) 
		 && (code[1] == buf[1]) 
		 && (code[2] == buf[2]) 
		 && (code[3] == buf[3])){
			num ++;
		}
		
		buf[0] = buf[1];
		buf[1] = buf[2];
		buf[2] = buf[3];
		mover++;
		count--;
	}
	fclose(file);
	return num;
}



//@Anderson Felipe
