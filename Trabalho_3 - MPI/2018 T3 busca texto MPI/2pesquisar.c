#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "tempo.h"

int topDown();
int bottomUp();


void main(int argc,char *argv[])
{
	tempo1();
	printf("Pesquisa topDown: %d\n", topDown());
	printf("Pesquisa bottomUp: %d\n", bottomUp());
	tempo2();
	printf("Tempo: ");
	tempoFinal("", argv[0], 0);
}

int topDown(){

	FILE *file;
	file = fopen("texto.txt","r");	  

	int num=0;

	char code[4]={"DHGK"}, c, buf[4];

	c=getc(file);  // leitura do primeiro char do arquivo
	buf[0] = c;

	c=getc(file);  // leitura do segundo char do arquivo
	buf[1] = c;

	c=getc(file);  // leitura do terceiro char do arquivo
	buf[2] = c;

	while((c=getc(file)) != EOF){  // leitura dos n char do arquivo

		buf[3] = c;

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

int bottomUp(){

	FILE *file;
	file = fopen("texto.txt","r");	  

	fseek(file, 0L, SEEK_END);
	int sz = ftell(file);

	int num = 0;

	char code[4] = {"DHGK"}, c, buf[4];
	
	fseek(file, -2L, SEEK_END);
	
	c=getc(file);  // leitura do primeiro char do arquivo
	buf[0] = c;

	fseek(file, -3L, SEEK_END);
	
	c=getc(file);  // leitura do segundo char do arquivo
	buf[1] = c;

	fseek(file, -4L, SEEK_END);
	
	c=getc(file);  // leitura do terceiro char do arquivo
	buf[2] = c;
	
	long count;
	
	for(count = 5L;count<=sz;count++){
		
		fseek(file, -count, SEEK_END);
	
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
