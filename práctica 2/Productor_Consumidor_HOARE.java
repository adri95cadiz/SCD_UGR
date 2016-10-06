import monitor.*;
import java.util.Random;

public class Buffer extends AbstractMonitor { 

  private int numSlots = 0, cont = 0;   
  private double[] buffer = null;
  private Condition consumir = makeCondition();
  private Condition producir = makeCondition();

  public Buffer( int p_numSlots ) { 
    numSlots = p_numSlots; 
    buffer = new double[numSlots];
    }

  public void depositar( double valor ){ 
    enter();
    if ( cont == numSlots ) producir.await();              
    buffer[cont] = valor; cont++;      
    consumir.signal();
    leave();
    }

  public double extraer() {
    enter();
    double valor;
    if( cont == 0 ) consumir.await();
    cont--; 
    valor = buffer[cont];
    leave();
    return valor;
    }
}

class Productor implements Runnable { 

  private Buffer bb;
  int veces; 
  int numP;
  public Thread thr;

  public Productor( Buffer pbb, int pveces, int pnumP ) { 
    bb    = pbb;
    veces = pveces;
    numP  = pnumP ;
    thr   = new Thread(this,"productor "+numP);
  }

  public void run() {
      double item = 100*numP ;
      for( int i=0 ; i<veces ; i++ ) { 
        System.out.println(thr.getName()+", produciendo " + item);
        bb.depositar( item++ );      
  		}
	}
}

class Consumidor implements Runnable { 

	private Buffer bb ;
  int veces; 
  int numC ;
  public Thread thr ;

  public Consumidor( Buffer pbb, int pveces, int pnumC ) { 
		bb    = pbb;
    veces = pveces;
    numC  = pnumC ;
    thr   = new Thread(this,"consumidor "+numC);
  }
  public void run() { 
			for( int i=0 ; i<veces ; i++ ) { 
				double item = bb.extraer ();
        System.out.println(thr.getName()+", consumiendo "+item);
      }
  }
}


class EjemploProductorConsumidor { 
  public static void main( String[] args ) { 
    if ( args.length != 5 ) {  
       System.err.println("Uso: n_cons n_prod tam_buf n_iter_prod n_iter_cons");
       return ;
    }

    Consumidor[] cons = new Consumidor[Integer.parseInt(args[0])] ;
	  Productor[]  prod = new Productor[Integer.parseInt(args[1])] ;
	  Buffer buffer = new Buffer(Integer.parseInt(args[2]));
	  int iter_cons = Integer.parseInt(args[3]);
	  int iter_prod = Integer.parseInt(args[4]);
	  if ( cons.length*iter_cons != prod.length*iter_prod ) { 
			System.err.println("no coinciden número de items a producir con a consumir");
      return ;
    } 

	  for(int i = 0; i < cons.length; i++) 
	    cons[i] = new Consumidor(buffer,iter_cons,i);
	  for(int i = 0; i < prod.length; i++)
	    prod[i] = new Productor(buffer,iter_prod,i);
	  for(int i = 0; i < prod.length; i++) prod[i].thr.start();
	  for(int i = 0; i < cons.length; i++) cons[i].thr.start();
  }
}

