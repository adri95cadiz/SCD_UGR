#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <vector>
using namespace std;

int numvec
vector <int> v[numvec];
int libre = 0;

sem_t
	puede_producir,
	puede_consumir,
	mutex;     

const int num_iter = 100; 

int producir_dato() {
	static int contador = 1;
	cout << "dato producido: " << contador << endl;	
	return contador++;
}

void consumir_dato (int dato) {
	cout << "dato consumido: " << dato << endl;
}

void* producir ( void* arg ) {
	for ( unsigned i = 0 ; i < num_iter ; i++ ){
		sem_wait( &puede_producir );

		sem_wait( &mutex );

		v[libre] = producir_dato();
		libre++;

		sem_post( &mutex );

		sem_post( &puede_consumir );		                         
	}
	return NULL;
}

void* consumir ( void* arg ) {
	for ( unsigned i = 0; i < num_iter; i++ ) {
		sem_wait( &puede_consumir );

		sem_wait( &mutex );

		consumir_dato( v[libre-1] );
		libre--;

		sem_post( &mutex );

		sem_post( &puede_producir );
	}
	return NULL;
}

int main() {
	pthread_t hebra_productora, hebra_consumidora;

	sem_init( &puede_producir, 0, numvec );
	sem_init( &puede_consumir, 0, 0 );
	sem_init( &mutex, 0, 1);

	pthread_create( &hebra_productora, NULL, producir, NULL );
	pthread_create( &hebra_consumidora, NULL, consumir, NULL );

	pthread_join( hebra_productora, NULL );
	pthread_join( hebra_consumidora, NULL );
}
