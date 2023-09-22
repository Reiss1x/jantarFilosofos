import java.util.concurrent.CountDownLatch;

/**
 * App
 */
public class App {

    public static void main(String[] args) {
        Filosofos[] fils = new Filosofos[5];
        Garfos[] garfos = new Garfos[5];

        for (int i = 0; i < garfos.length; i++) {
            garfos[i] = new Garfos();
        }

        for (int i = 0; i < fils.length; i++) {
           Garfos garfoEsq = garfos[i];
           Garfos garfoDir = garfos[(i + 1) % garfos.length];

           if(i == fils.length - 1){
                fils[i] = new Filosofos(garfoDir,garfoEsq);
           } else{
            fils[i] = new Filosofos(garfoEsq,garfoDir);
           }

           Thread thread = new Thread(fils[i], "Filósofo "+ (i+1));

           thread.start();
        }
        
        try {
            Thread.sleep(20000);
            for(Filosofos os : fils){
                os.parar();
            }
            for(Filosofos os : fils){
                os.join();
            }
            Thread.sleep(5000);

        } catch (Exception e) {
            Thread.currentThread().interrupt();
        }
        imprimirContagemComida(fils);
    };

    private static void imprimirContagemComida(Filosofos[] filosofos) {
        for (int i = 0; i < filosofos.length; i++) {
            System.out.println("Filósofo " + (i + 1) + " comeu " + filosofos[i].getContadorComida() + " vezes.");
        }
            
            
    }
}
