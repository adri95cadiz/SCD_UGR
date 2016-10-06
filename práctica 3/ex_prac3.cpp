#include <iostream>
#include "mpi.h"
#include <time.h>
#include "math.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

#define Intermedio 8
#define ITERS 10

void productor() {
    int value, rank, peticion = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (unsigned int i=0; i<ITERS; i++) {
	MPI_Ssend(&peticion, 1, MPI_INT, Intermedio, MPI_ANY_TAG, MPI_COMM_WORLD);
        value=(rand() % 10 );
        cout<< "Productor "<< rank <<" produce valor "<< value << endl;
        sleep(rand() % 2 );
        MPI_Ssend( &value, 1, MPI_INT, Intermedio, rank, MPI_COMM_WORLD );
    }
}


void intermedio() {
    int value = 10, peticion, rama, num;
    MPI_Status status;

    for (unsigned int i=0; i<ITERS*5; i++) {
	MPI_Recv( &peticion, 1, MPI_INT, num, MPI_ANY_TAG, MPI_COMM_WORLD,&status);
	if (peticion == 0) 	
        	MPI_Recv( &value, 1, MPI_INT, num, MPI_ANY_TAG, MPI_COMM_WORLD,&status);
	if (peticion == 1) 
		MPI_Ssend( &value, 1, MPI_INT, num, MPI_ANY_TAG, MPI_COMM_WORLD);	        	
    }
}


void consumidor() {
    int value, peticion=1, rank;
    MPI_Status status;
	
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    while(true) {
        MPI_Ssend(&peticion, 1, MPI_INT, Intermedio, MPI_ANY_TAG, MPI_COMM_WORLD);
        MPI_Recv(&value, 1, MPI_INT, Intermedio, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	if( value != 10)
        	cout<< "Consumidor " << rank <<" consume valor "<<value<<" de Intermedio "<<endl<<flush;
        sleep(rand() % 2 );
    }
}

int main(int argc, char *argv[]) {
	int rank, size;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(size !=9){
		cout<<"El numero de procesos debe ser 9"<<endl;
		return 0;
	}

	if(rank<3)
		consumidor();
	else if(rank == 8)
		intermedio();
	else
		productor();

	MPI_Finalize();
}
