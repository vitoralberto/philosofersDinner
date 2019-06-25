#include "stdio.h"
#include "stdlib.h"
#include "windows.h"
#include "fcntl.h"
#include "sys/types.h"
#include "pthread.h"
#include "semaphore.h"
//=================================================================================
#define N           5           // número de filósofos
#define LEFT        (i+N-1)%N   // número do vizinho à esquerda de i
#define RIGHT       (i+1)%N     // número do vizinho à direito de i
#define THINKING    0           // o filósofo está pensando
#define HUNGRY      1           //o filósofo está com fome
#define EATING      2           // o filósofo está comendo
#define TRUE        1

int state[N];                   // arranjo para controlar o estado de cada um
sem_t mutex;                    //exclusão mútua para as regiões críticas
sem_t s[N];                     // um semáforo por filósofo

// protótipos
void* philosopher(void* arg);
void take_forks(int i);
void put_forks(int i);
void test(int i);
void eat(int i);
void think(int i);

int main() {
 int i;

 sem_init(&mutex, TRUE, 1);

 for(i = 0; i < N; i++) {
     sem_init(&s[i], TRUE, 1);
 }

 pthread_t tphil[5];

   // criando os 5 filósofo - 1 thread para cada
   for(i = 0; i < N; i++) {
     pthread_create(&tphil[i], NULL, (void *) philosopher, (void *) &i);
 }

 for(i = 0; i < N; i++) {
     pthread_join(tphil[i], NULL);
 }

 return 0;
}

void* philosopher(void * arg) { //i: o número do filósofo, de 0 a N-1
 int i = *((int *) arg);

 while(TRUE) {
     think(i);               // o filósofo está pensando
     take_forks(i);          // pega dois garfos ou bloqueia
     eat(i);                 // o filósofo está comendo
     put_forks(i);           // devolve os dois garfos à mesa
 }

 pthread_exit(NULL);
}

void take_forks(int i) {     //i: o número do filósofo, de 0 a N-1
 sem_wait(&mutex);           // entra na região crítica
 state[i] = HUNGRY;          // registra que o filósofo está faminto
 test(i);                    // tenta pegar dois garfos
 sem_post(&mutex);           // sai da região crítica
 sem_wait(&s[i]);            // bloqueia se os garfos não foram pegos
}

void put_forks(int i) {      //i: o número do filósofo, de 0 a N-1
 sem_wait(&mutex);           // entra na região crítica
 state[i] = THINKING;        // o filósofo acabou de comer
 test(LEFT);                 // vê se o vizinho da esquerda pode comer agora
 test(RIGHT);                // vê se o vizinho da direito pode comer agora
 sem_post(&mutex);           // sai da região crítica
}

void test(int i) {
 if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
     state[i] = EATING;
     sem_post(&s[i]);
 }
}

void eat(int i) {               //i: o número do filósofo, de 0 a N-1
 printf("Filosofo %d esta comendo!\n", i);
 Sleep(2000);
}

void think(int i) {             //i: o número do filósofo, de 0 a N-1
 printf("Filosofo %d esta pensando!\n", i);
 Sleep(2000);
}
