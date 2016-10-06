#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "fun_tiempo.h"

using namespace std ;

unsigned long m = 16;
unsigned long n = 4;
double resultado_parcial[4];

double f( double x ){
	return 4.0/(1+x*x);
}

double calcular_integral_secuencial(){

	double suma = 0.0;

	for( unsigned long i = 0; i < m; i++ ) 
		suma += f( (i+0.5)/m );

	return suma/m;
}

void* funcion_concurrente( void* ih_void ){

	unsigned long ih = (unsigned long) ih_void;
	double sumap=0;

	for( unsigned long i = ih ; i < m ; i += n )
		sumap +=f((i+0.5)/m);
	
	resultado_parcial[ih]=sumap;
}

double calcular_integral_concurrente(){

	pthread_t id_hebras[n];
	double resultado=0;
	unsigned long i;

	for( i = 0; i < n; i++ ) 
		pthread_create(&(id_hebras[i]) , NULL, funcion_concurrente, (void*) i);	

	for( i = 0; i < n; i++)
		pthread_join(id_hebras[i], NULL );
	
	for( i = 0; i < n; i++) 
		resultado += resultado_parcial[i];

	return resultado/m;
}

int main(){

	double resultado_secuencial, resultado_concurrente;
	
	struct timespec inicio_secuencial = ahora();
	resultado_secuencial = calcular_integral_secuencial();
	struct timespec final_secuencial = ahora();
	
	cout << "El resultado previsto es: 3.14159265359" << endl;

	cout << "El resultado de calcular la integral de forma secuencial es: " << resultado_secuencial << " y ha tardado: " << duracion( &inicio_secuencial, &final_secuencial ) << "seg" << endl;

	struct timespec inicio_concurrente = ahora();
	resultado_concurrente = calcular_integral_concurrente();
	struct timespec final_concurrente = ahora();

	cout << "El resultado de calcular la integral de forma concurrente es: " << resultado_concurrente << " y ha tardado: " << duracion( &inicio_concurrente, &final_concurrente ) << "seg" << endl;

}
