
# Flags de compilação processador: -march=ivybridge -O3 -pipe
all: compile
	
compile:
	gcc -o 1 1criar.c
	gcc -o 2 2pesquisar.c -march=ivybridge -O3 -pipe
	mpicc -o search search.c -march=ivybridge -O3 -pipe
	

clean:
	rm -rf ?
	rm -rf *.txt


