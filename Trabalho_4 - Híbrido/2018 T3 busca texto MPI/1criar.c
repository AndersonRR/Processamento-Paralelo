#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//1073741824 - 1GB

//#define TAM 1048576*400
#define TAM 1048576*10

void cria_arq();
void mostra_arq();
void mostra_arq2();

void main(void)
{

	cria_arq();
	//mostra_arq();

}


void cria_arq(){

	FILE *file;		
	file = fopen("texto.txt","w");	    
	int n=0, cont;
	char c;

	for (cont=TAM; cont > 0; cont --){

		do {
			n = rand() % 90;
		} while (n < 65);
		//printf("%d ",n);

		c = n + 0;
		//printf("%c ",c);

		putc(c,file);
	}

	fclose(file);    
}





void mostra_arq()   // leitura sequencial
{

	FILE *file;
	file = fopen("texto.txt","r");	  

	char c;

	printf("\n\n");	
	
	while( (c=getc(file)) != EOF){

		printf("%c",c);	
	}

	printf("\n\n");	

	fclose(file);    
}





void mostra_arq2() // leitura em blocos
{

	FILE *file;
	file = fopen("texto.txt","r");	  

	char buffer[40];

	printf("\n\n");	

	while(fgets(buffer,40,file)!= NULL){
		puts(buffer);
	}

	printf("\n\n");	

	fclose(file);    
}
