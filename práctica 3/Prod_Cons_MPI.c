#include <iostream>
#include "mpi.h"
#include <time.h>
#include "math.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

#define Productor 0
#define Consumidor 1
#define Buffer  5
#define ITERS 8
#define TAM 5

void productor()
{
    int value, rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (unsigned int i=0; i<ITERS; i++)
    {
        value=i;
        cout<< "El Productor"<< rank <<" produce valor "<< value <<endl <<flush;
        sleep(rand() % 2 );
        MPI_Ssend( &value, 1, MPI_INT, Buffer, Productor, MPI_COMM_WORLD );
    }
}


void buffer()
{
    int value[TAM], peticion, pos=0, rama, num;
    MPI_Status status;

    for (unsigned int i=0; i<ITERS*10; i++)
    {
        if (pos==0)
        		rama=0; 
        else if (pos==TAM)
        		rama=1;
        else 
        {
            MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (status.MPI_SOURCE > 5) rama = 1;
            else if (status.MPI_SOURCE < 5) rama=0;
        }
        switch(rama)
        {
        case 0:
        		MPI_Probe(MPI_ANY_SOURCE, Productor, MPI_COMM_WORLD, &status);
				    num = status.MPI_SOURCE;

            MPI_Recv( &value[pos], 1, MPI_INT, num, Productor, MPI_COMM_WORLD,&status);
            cout<< "Buffer recibe "<< value[pos] << " de Productor "<<endl<<flush;
            pos++;
            break;

        case 1:
        		MPI_Probe(MPI_ANY_SOURCE, Consumidor, MPI_COMM_WORLD, &status);
				    num = status.MPI_SOURCE;

            MPI_Recv( &peticion, 1, MPI_INT, num, Consumidor, MPI_COMM_WORLD,&status);
            MPI_Ssend( &value[pos-1], 1, MPI_INT, num, Consumidor, MPI_COMM_WORLD);
            cout<< "Buffer envía "<< value[pos-1] << " a Consumidor "<<endl<<flush;
            pos--;
            break;
        }
    }
}


void consumidor()
{
    int value, peticion=1;
    float raiz;
    MPI_Status status;

    for (unsigned int i=0; i<10; i++)
    {
        MPI_Ssend(&peticion, 1, MPI_INT, Buffer, Consumidor, MPI_COMM_WORLD);
        MPI_Recv(&value, 1,     MPI_INT, Buffer, Consumidor, MPI_COMM_WORLD,&status);
        cout<< "Consumidor recibe valor "<<value<<" de Buffer "<<endl<<flush;
        sleep(rand() % 2 );
        raiz=sqrt(value);
    }
}

int main(int argc, char *argv[])
{
	int rank, size;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(size !=10){
		cout<<"El numero de procesos debe ser 10"<<endl;
		return 0;
	}

	if(rank<5)
		productor();
	else if(rank == 5)
		buffer();
	else
		consumidor();

	MPI_Finalize();
}
