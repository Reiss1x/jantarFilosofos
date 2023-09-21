#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NUM_THREADS 5
#define NUM_FORKS 5

typedef enum STATE {
    PENSANDO,
    COMENDO
} STATE;

typedef struct Phylosopher {
    int id;
    STATE state;
    int forkRight;
    int forkLeft;
    int eatingTime;
    int eatingCount;
} Phylosopher;

Phylosopher phylosophers[NUM_THREADS];
int forks[NUM_FORKS] = {0}; // 0 means fork is available, 1 means it's in use

void getForksAndEat(Phylosopher *phylosopher) {
    if (forks[phylosopher->forkRight] == 0 && forks[phylosopher->forkLeft] == 0) {
        // Both forks are available
        forks[phylosopher->forkRight] = 1;
        forks[phylosopher->forkLeft] = 1;

        phylosopher->state = COMENDO;
        int eatingTime = rand() % 3 + 1; // valor varia de 1 a 3
        sleep(eatingTime);
        phylosopher->eatingTime += eatingTime;
        phylosopher->eatingCount++;
        phylosopher->state = PENSANDO;

        // Release the forks
        forks[phylosopher->forkRight] = 0;
        forks[phylosopher->forkLeft] = 0;
    }
}

int main() {

    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

    srand(time(NULL));

    // inicializa filósofos
    for (int i = 0; i < NUM_FORKS; i++) {
        phylosophers[i].id = i;
        phylosophers[i].state = PENSANDO;
        phylosophers[i].eatingCount = 0;
        phylosophers[i].eatingTime = 0;
        phylosophers[i].forkRight = i;
        if (i == NUM_FORKS - 1) {
            phylosophers[i].forkLeft = 0;
        } else {
            phylosophers[i].forkLeft = i + 1;
        }
    }

    for (int time = 0; time < 10; time++) {
        for (int i = 0; i < NUM_THREADS; i++) {
            getForksAndEat(&phylosophers[i]);
        }
    }

    for (int i = 0; i < NUM_FORKS; i++) {
        printf("Filosofo %d comeu %d vezes e gastou %d segundos comendo\n", phylosophers[i].id, phylosophers[i].eatingCount, phylosophers[i].eatingTime);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    double time_spent = (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
    printf("Tempo total: %f\n", time_spent);
    
    return 0;
}
