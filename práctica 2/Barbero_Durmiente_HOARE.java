import monitor.*;
import java.util.Random;

class Barberia extends AbstractMonitor { 

	public static bool silla = false;
	public static int sala_espera = 0;
	private Condition puedeSentarse = makeCondition();
	private Condition puedeCortar = makeCondition();

	public void cortarPelo(int nCliente) { 
		enter();		
		sala_espera++;
		if (silla) {
			System.out.println("El cliente " + nCliente " no puede sentarse aún");
			puedeSentarse.await();
		}
		System.out.println("El cliente " + nCliente " se sienta a cortarse el pelo");
		puedeCortar.signal();
		leave();
	}

	public void siguienteCliente() { 
		enter();
		if(sala_espera == 0)
			puedeCortar.await();
		sala_espera--;
		silla = true;
		leave;
	}

	public void finCliente(){ 
		enter();
		System.out.println("El cliente ha terminado de cortarse el pelo");
		silla = false;
		puedeSentarse.signal();
		leave();
	}

}

class aux {
	static Random genAlea = new Random();
	static void dormir_max(int milisecsMax) { 
		try { 
				Thread.sleep(genAlea.nextInt(milisecsMax));     
		} catch( InterruptedException e ) { 
			System.err.println("sleep interumpido en 'aux.dormir_max()'");
		}
  }  
}

class Cliente implements Runnable {

	public Thread thr;
	int nCliente
	
	public Cliente(Barberia p_barberia, int p_nCliente, string nombre) {
		barberia = p_barberia;
		nCliente = p_nCliente;
    thr = new Thread(this, nombre);
	}

	public void run() {
		while(true) {
			barberia.cortarPelo(nCliente);
			aux.dormir_max( 2000 );
		}

	}
}

class Barbero implements Runnable {

	public Thread thr;

	public Cliente(Barberia p_barberia, string nombre) {
		barberia = p_barberia
    thr = new Thread(this, nombre);
	}	

	public void run() {
		while(true) {
			barberia.siguienteCliente();
			aux.dormir_max( 2500 );
			barberia.finCliente();
		}

	}
}

class Principal { 
  public static void main(String[] args) { 
		if ( args.length != 1 ) { 
	    System.err.println("Uso:  num_clientes");
      return;
    }

    int nClientes = Integer.parseInt(args[0]);
   	Clientes[] cliente = new Cliente[nClientes];
    Barbero barbero = new Barbero();

		for(int i = 0; i < nClientes; i++) 
      	    cliente[i] = new Cliente(monitor, i, "Fumador"+i);
		barbero = new Barbero(monitor, "Barbero");

   	for(int i = 0; i < 3 ; i++) 
	    cliente[i].thr.start();
    barbero.thr.start();
	}
}
