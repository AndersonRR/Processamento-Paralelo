# comentario -> para executar digite make 
# Flags de compilação processador: -march=ivybridge -O2 -pipe

all: compile
	
compile:
	clear
	gcc -o t thread.c -lpthread
	gcc -o tflag thread.c -lpthread -march=ivybridge -O2 -pipe
	
	clear
	@echo " "
	@echo "Compilaçao concluida"
	@echo " "

clean:
	rm -rf *.out

