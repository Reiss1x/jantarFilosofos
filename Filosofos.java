import java.util.HashMap;
import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.CountDownLatch;


public class Filosofos implements Runnable {
    private static AtomicInteger id = new AtomicInteger(0);
    private Object garfoDireito;
    private Object garfoEsquerdo;
    private int idF;
    private boolean isRunning = true;
    private int[] contadorComida = new int[5];
    private CountDownLatch finishLatch;
    public Filosofos(Object eGarfo, Object dGarfo){
        this.idF = id.incrementAndGet();
        this.garfoEsquerdo = eGarfo;
        this.garfoDireito = dGarfo;
    }

    @Override
    public void run() {
        try{
            while (isRunning) {
                // pensando
                acao("Filóso: "+ getId() + ": Pensando");
                synchronized (garfoEsquerdo) {
                    acao("Filóso: "+ getId() + ": Pegou garfo esquerdo");
                    synchronized (garfoDireito) {
                        // comendo
                        acao("Filóso: "+ getId() + ": Pegou garfo direito - comendo"); 
                        contadorComida[getId()]++;
                        acao("Filóso: "+ getId() + ": Largou garfo direito");
                    }
                    // pensando de novo
                    acao("Filóso: "+ getId() + ": Largou garfo esquerdo, pensando de novo");
            
                    }          
                }   
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            return;
        }
    }

    public int getId(){
        return this.idF;
    }
    private void acao(String acao) throws InterruptedException{
        System.out.println(acao);
        Thread.sleep(((int)(Math.random() * 100)));
    }
    public int[] getContadorComida() {
        return contadorComida;
    }
    public void parar(){
        
        isRunning = false;
    }
}