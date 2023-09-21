#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREADS 5
#define NUM_FORKS 5

volatile int execute = 1;

typedef enum STATE {
    PENSANDO,
    COMENDO
} STATE;

typedef struct Fork {
    int id;
    pthread_mutex_t got;
} Fork;

typedef struct Phylosopher {
    int id;
    STATE state;
    Fork *forkRight;
    Fork *forkLeft;
    int eatingTime;
    int eatingCount;
    sem_t *semaphore;
} Phylosopher;

Phylosopher phylosophers[NUM_THREADS];
sem_t semaphores[NUM_THREADS];

void* getForksAndEat(void* args) {
    Phylosopher *phylosopher = (Phylosopher*) args;

    while (execute) {
        sem_wait(phylosopher->semaphore);

        pthread_mutex_lock(&phylosopher->forkRight->got);
        pthread_mutex_lock(&phylosopher->forkLeft->got);

        phylosopher->state = COMENDO;
        int eatingTime = rand() % 3 + 1; // valor varia de 1 a 3
        sleep(eatingTime);
        phylosopher->eatingTime += eatingTime;
        phylosopher->eatingCount++;
        phylosopher->state = PENSANDO;

        pthread_mutex_unlock(&phylosopher->forkRight->got);
        pthread_mutex_unlock(&phylosopher->forkLeft->got);

        // Permitir que os vizinhos tentem pegar os garfos.
        // aqui caso seja o filosofo 0 será a conta de (0 + 5 - 1) % 5 = 4 | Pois 4/5 = 0 e resto 4 
        int leftNeighbor = (phylosopher->id + NUM_THREADS - 1) % NUM_THREADS;
        // aqui caso seja o filosofo 4 será a contade de (4 + 1) % 5 = 0 | Pois 0/5 = 0 e resto 0 (caso seja 5 será 1
        int rightNeighbor = (phylosopher->id + 1) % NUM_THREADS;

        sem_post(&semaphores[leftNeighbor]);
        sem_post(&semaphores[rightNeighbor]);
    }

    return NULL;
}

int main() {

    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

    srand(time(NULL));

    // inicializa threads
    pthread_t threads[NUM_THREADS];

    // inicializa garfos
    Fork forks[NUM_FORKS];
    for (int i = 0; i < NUM_FORKS; i++) {
        forks[i].id = i;
        pthread_mutex_init(&forks[i].got, NULL);
    }

    // inicializa semáforos e filósofos
    for (int i = 0; i < NUM_FORKS; i++) {
        sem_init(&semaphores[i], 0, 1); // O último argumento é o valor inicial do semáforo.

        phylosophers[i].id = i;
        phylosophers[i].state = PENSANDO;
        phylosophers[i].forkRight = &forks[i];
        phylosophers[i].eatingCount = 0;
        phylosophers[i].eatingTime = 0;
        phylosophers[i].semaphore = &semaphores[i];

        if (i == NUM_FORKS - 1) {
            phylosophers[i].forkLeft = &forks[0];
        } else {
            phylosophers[i].forkLeft = &forks[i + 1];
        }

        pthread_create(&threads[i], NULL, getForksAndEat, &phylosophers[i]);
    }

    sleep(30);
    execute = 0;

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < NUM_FORKS; i++) {
        pthread_mutex_destroy(&forks[i].got);
        sem_destroy(&semaphores[i]);
    }

    
    for (int i = 0; i < NUM_FORKS; i++) {
        printf("Filosofo %d comeu %d vezes e gastou %d segundos comendo\n", phylosophers[i].id, phylosophers[i].eatingCount, phylosophers[i].eatingTime);
    }

    clock_gettime(CLOCK_REALTIME, &end);
    double time_spent = (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec) / 1e9; // 1e9 = 1 * 10^9 ou 1000000000
    printf("Tempo total: %f\n", time_spent);

    return 0;
}
