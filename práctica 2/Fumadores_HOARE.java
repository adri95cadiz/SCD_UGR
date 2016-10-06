import monitor.*;
import java.util.Random;

public class MonitorFumadores extends AbstractMonitor {

    public static int mesa = null;
    private Condition puedefumar[];    
    private Condition puedecolocar;
    
    public MonitorFumadores() {
			puedecolocar = makeCondition();
			puedefumar = new Condition[3];
    	puedefumar[0] = makeCondition();
    	puedefumar[1] = makeCondition();
   		puedefumar[2] = makeCondition();
    }
  
    public void entrafumar(int ingrediente){
			enter();
      if(mesa != ingrediente) {
	    	System.out.println("Fumador "+ ingrediente + " no puede fumar aun.");
	    	puedefumar.get(ingrediente).await();
      }
			System.out.println("Fumador "+ ingrediente + " comienza a fumar.");
			mesa = null;
      puedecolocar.signal();
			leave();
    }

    public void terminafumar(){
			enter();
			System.out.println("Fumador "+ ingrediente + " termina de fumar.");			
			leave();
    }

    public void colocar(int ingrediente){
			enter();
      if(mesa != null)
      	puedecolocar.await();        
      mesa = ingrediente;
      System.out.println("El estanquero coloca el ingrediente "+ mesa);
      puedefumar.get(ingrediente).signal();
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

class Fumador implements Runnable {

    private MonitorFumadores fumadores;
    int nveces, ingrediente;
    public Thread thr;
  
    public Fumador( MonitorFumadores p_fumadores, int p_nveces, int p_ingrediente, String nombre ) {
      fumadores = p_fumadores;    
      nveces = p_nveces;
			ingrediente = p_ingrediente;
      thr = new Thread(this, nombre);
    }

    public void run() { 
      for( int i = 0 ; i < nveces ; i++ ) { 
	    	System.out.println(thr.getName()+" solicita fumar.");
        fumadores.entrafumar(ingrediente);
        System.out.println(thr.getName()+" fumando.");
        aux.dormir_max(1000) ;
        fumadores.terminafumar();
			}
    }
}

class Estanquero implements Runnable {

    private MonitorFumadores estanquero;
    int nveces, nfumadores, ingrediente;
    public Thread thr;
    static Random genAlea = new Random();
  
    public Estanquero( MonitorFumadores p_estanquero, int p_nveces, int p_nfumadores, String nombre ) {
      estanquero = p_estanquero  ;    
      nveces = p_nveces ;
			nfumadores = p_nfumadores;
      thr = new Thread(this,nombre);
    }

    public void run() { 
      for( int i = 0 ; i < nveces ; i++ ) { 
	    	ingrediente = genAlea.nextInt() % nfumadores;
	    	estanquero.colocar(ingrediente);
			}
    }
}

class Principal { 
    public static void main(String[] args) { 
			if ( args.length != 1 ) { 
	    	System.err.println("Uso:  num_iteraciones");
        return;
      }
    
    	Fumadores[] fumador = new Fumador[3];
    	Estanquero estanquero = new Estanquero();
			MonitorFumadores monitor = new MonitorFumadores();
    	int iter = Integer.parseInt(args[0]);
    	
    
    	for(int i = 0; i < 3; i++) 
      	fumador[i] = new Fumador(monitor, iter, i, "Fumador"+i);    	 
      estanquero = new Estanquero(monitor, iter, fumador.length, "Estanquero");
    
    	for(int i = 0; i < 3 ; i++) 
	    	fumador[i].thr.start();
    	estanquero.thr.start();
   }	
}
