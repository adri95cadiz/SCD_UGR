#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h> 	
#include <iostream>

#define Camarero 10
#define TAGSentarse 0
#define TAGLevantarse 1
#define TAGCogeTenedor 2
#define TAGSueltaTenedor 3

using namespace std;

void Filosofo(int rank, int size){
	int peticion;
	int izq = rank + 1;
	int der;
	if(rank == 0)
		der = 9;
	else
		der = rank - 1;
	while(true){
		cout << "Filosofo " << rank << " pensando " << endl;
		sleep(rand()%2);
			
		cout << "Filosofo " << rank << " sentandose " << endl;
		MPI_Ssend( &peticion, 1, MPI_INT,Camarero,TAGSentarse, MPI_COMM_WORLD );	
	
	  	MPI_Ssend( &rank, 1, MPI_INT,izq,TAGCogeTenedor, MPI_COMM_WORLD);
       		cout << "Filosofo " << rank << " solicita tenedor izquierdo "  << izq << endl;
            
	        MPI_Ssend( &rank, 1, MPI_INT,der,TAGCogeTenedor, MPI_COMM_WORLD);
       		cout << "Filosofo " << rank << " solicita tenedor derecho " << der << endl;

		cout << "Filosofo " << rank << " comiendo " << endl << flush;
		sleep(rand()%2);
	
		cout << "Filosofo " << rank << " suelta tenedor izquierdo  " << izq << endl;
		MPI_Ssend( &rank, 1, MPI_INT,izq,TAGSueltaTenedor, MPI_COMM_WORLD );
              
		cout << "Filosofo " << rank << " suelta tenedor derecho  " << der << endl;
		MPI_Ssend( &rank, 1, MPI_INT,der,TAGSueltaTenedor, MPI_COMM_WORLD );
		
		cout << "Filosofo " << rank << " se levanta " << endl;
		MPI_Ssend( &peticion, 1, MPI_INT,Camarero,TAGLevantarse, MPI_COMM_WORLD );	
	}
}



void Tenedor(int rank, int size){
	int Filo;
	MPI_Status status;
	while(true){
		MPI_Recv(&Filo, 1, MPI_INT, MPI_ANY_SOURCE, TAGCogeTenedor, MPI_COMM_WORLD, &status);
		cout << "Tenedor " << rank << " recibe peticion de " << Filo << endl;
		MPI_Recv(&Filo, 1, MPI_INT, MPI_ANY_SOURCE, TAGSueltaTenedor, MPI_COMM_WORLD, &status);
		cout << "Tenedor " << rank << " recibe liberacion de " << Filo << endl;
	}
}


void camarero(){	
	int peticion,etiqueta;
	int sentados=0;
	MPI_Status status;
	while(true){
		if (sentados < 4)
			if(sentados == 0)
				etiqueta = TAGSentarse;
			else
				etiqueta = rand()%2;
		else
			etiqueta = TAGLevantarse;
			
		MPI_Recv(&peticion,1,MPI_INT,MPI_ANY_SOURCE,etiqueta,MPI_COMM_WORLD,&status);
		if(etiqueta == TAGSentarse)
			sentados++;
		else
			sentados--;
		cout << "Clientes sentados:" << sentados << endl;
	}
}

int main(int argc, char **argv){
	int rank, size;
	srand(time(0));
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(size != 11){
		cout << "Uso mpirun -np 11 camarero" << endl;
		MPI_Finalize();
		return 0;
	}
	if( ((rank % 2) == 0) && (rank != Camarero)){
		Filosofo(rank,size);
	}else
		if(rank == Camarero)
			camarero();
		else
			Tenedor(rank,size);
	
	MPI_Finalize();
	return 0;
}
