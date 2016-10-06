#include "mpi.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

void Filosofo(int id, int nprocesos);
void Tenedor (int id, int nprocesos);

int main(int argc,char** argv )
{

 int rank,size;
 srand(time(0));


 MPI_Init( &argc, &argv );
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 MPI_Comm_size(MPI_COMM_WORLD, &size);

 if( size!=10){
   if(rank == 0)
      cout<<"El numero de procesos debe ser 10"<<endl<<flush;
   MPI_Finalize(); return 0;
 }
 if ((rank%2) == 0)
      Filosofo(rank,size); 
 else Tenedor(rank,size); 
 MPI_Finalize( );
 return 0;
}

void Filosofo(int id, int nprocesos)
{
 int izq=(id+1)%nprocesos;
 int der=(id-1+nprocesos)%nprocesos;


 if(id == 0)
 {
    int intercambia;
    intercambia=izq;
    izq=der;
    der=izq;
 }

 while(1)
 {

  cout<<"Filosofo "<<id<< " solicita tenedor izq ..."<<izq <<endl<<flush;
  MPI_Ssend(&id, 1, MPI_INT, izq, 0, MPI_COMM_WORLD);

  cout<<"Filosofo "<<id<< " coge tenedor der ..."<<der <<endl<<flush;
  MPI_Ssend(&id, 1, MPI_INT, der, 0, MPI_COMM_WORLD);
  cout<<"Filosofo "<<id<< " COMIENDO"<<endl<<flush;
  sleep((rand() % 3)+1); 

  cout<<"Filosofo "<<id<< " suelta tenedor izq ..."<<izq <<endl<<flush;
  MPI_Ssend(&id, 1, MPI_INT, izq, 0, MPI_COMM_WORLD);

  cout<<"Filosofo "<<id<< " suelta tenedor der ..."<<der <<endl<<flush;
  MPI_Ssend(&id, 1, MPI_INT, der, 0, MPI_COMM_WORLD);

  cout<<"Filosofo "<<id<< " PENSANDO"<<endl<<flush;
  sleep((rand()%3)+1 );
 }
}

void Tenedor(int id, int nprocesos)
{
  int buf; MPI_Status status; int Filo;
  while(1)
  {
    MPI_Recv(&Filo, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    cout<<"Ten. "<<id<<" recibe petic. de "<<Filo<<endl<<flush;
    MPI_Recv(&Filo, 1, MPI_INT, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);
    cout<<"Ten. "<<id<<" recibe liberac. de "<<Filo<<endl<<flush;
  }
}
