echo +-------------------------------------+
echo +------- Iniciando sequencial --------+
echo +-------------------------------------+
echo +  para executar digite: bash run.sh  +
echo +-------------------------------------+

export cont=1
for j in 1 2 3 4 5 6 7 8 9 10
do	
	for i in 1 2 3 4 5 6 7 8 9 10
	do
		mpirun -np $cont search
		
	done
	cont=$(( $cont + 1 ))
	echo "";
done
