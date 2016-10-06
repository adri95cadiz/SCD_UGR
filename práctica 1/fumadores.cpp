#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int cigarros[3] = {0, 0, 0};
sem_t estanco;
sem_t fumador[3];
unsigned ingredientes; // 0 es tabaco, 1 es papel y 2 es cerillas.

void fumar(){
	const unsigned miliseg = 100U + (rand() %1900U);
	usleep( 1000U*miliseg );
}

void* estanquero( void* i ){

	for(int i=0; i<100; i++) {

		sem_wait( &estanco );
		ingredientes = rand()%3;

		if(ingredientes == 0)
			sem_post( &fumador[0] );

		if(ingredientes == 1)
			sem_post( &fumador[1] );

		if(ingredientes == 2)
			sem_post( &fumador[2] );
	}
}

void* fumador( void* i_void ){

	unsigned long i = (unsigned long) i_void;

	while(true){
		sem_wait( &fu[i] );
		cigarros[i]++;
		cout << "f" << i << ": "<< cigarros[i] << endl;
		sem_post( &estanco );
		fumar();
	}
}

int main(){

	srand ( time(NULL) );
	unsigned long i;
	pthread_t fumador[0],fumador[1],fumador[2],est;

	for(i = 0 ; i < 3 ; i++)
		sem_init( &fumador[i], 0, 0 );
	sem_init( &estanco,0,1 );

	pthread_create( &est, NULL, estanquero, NULL );
	for(i = 0 ; i < 3 ; i++)
		pthread_create( &fumador[i], NULL, fumador , (void*) i );

	for(i = 0 ; i < 3 ; i++)
		pthread_join( fumador[i], NULL );
	pthread_join( est, NULL );

	cout << endl << endl << "El primer fumador ha fumado: " << cigarros[1] << " cigarros\nEl segundo: " << cigarros[2] << "cigarros\nY el tercero: " << cigarros[3] << " cigarros" << endl;
}

