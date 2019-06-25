#include "stdio.h"
#include "stdlib.h"
#include "windows.h"
#include "fcntl.h"
#include "sys/types.h"
#include "pthread.h"
#include "semaphore.h"
//=================================================================================
#define N           5           // n�mero de fil�sofos
#define LEFT        (i+N-1)%N   // n�mero do vizinho � esquerda de i
#define RIGHT       (i+1)%N     // n�mero do vizinho � direito de i
#define THINKING    0           // o fil�sofo est� pensando
#define HUNGRY      1           //o fil�sofo est� com fome
#define EATING      2           // o fil�sofo est� comendo
#define TRUE        1

int state[N];                   // arranjo para controlar o estado de cada um
sem_t mutex;                    //exclus�o m�tua para as regi�es cr�ticas
sem_t s[N];                     // um sem�foro por fil�sofo

// prot�tipos
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

   // criando os 5 fil�sofo - 1 thread para cada
   for(i = 0; i < N; i++) {
     pthread_create(&tphil[i], NULL, (void *) philosopher, (void *) &i);
 }

 for(i = 0; i < N; i++) {
     pthread_join(tphil[i], NULL);
 }

 return 0;
}

void* philosopher(void * arg) { //i: o n�mero do fil�sofo, de 0 a N-1
 int i = *((int *) arg);

 while(TRUE) {
     think(i);               // o fil�sofo est� pensando
     take_forks(i);          // pega dois garfos ou bloqueia
     eat(i);                 // o fil�sofo est� comendo
     put_forks(i);           // devolve os dois garfos � mesa
 }

 pthread_exit(NULL);
}

void take_forks(int i) {     //i: o n�mero do fil�sofo, de 0 a N-1
 sem_wait(&mutex);           // entra na regi�o cr�tica
 state[i] = HUNGRY;          // registra que o fil�sofo est� faminto
 test(i);                    // tenta pegar dois garfos
 sem_post(&mutex);           // sai da regi�o cr�tica
 sem_wait(&s[i]);            // bloqueia se os garfos n�o foram pegos
}

void put_forks(int i) {      //i: o n�mero do fil�sofo, de 0 a N-1
 sem_wait(&mutex);           // entra na regi�o cr�tica
 state[i] = THINKING;        // o fil�sofo acabou de comer
 test(LEFT);                 // v� se o vizinho da esquerda pode comer agora
 test(RIGHT);                // v� se o vizinho da direito pode comer agora
 sem_post(&mutex);           // sai da regi�o cr�tica
}

void test(int i) {
 if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
     state[i] = EATING;
     sem_post(&s[i]);
 }
}

void eat(int i) {               //i: o n�mero do fil�sofo, de 0 a N-1
 printf("Filosofo %d esta comendo!\n", i);
 Sleep(2000);
}

void think(int i) {             //i: o n�mero do fil�sofo, de 0 a N-1
 printf("Filosofo %d esta pensando!\n", i);
 Sleep(2000);
}
