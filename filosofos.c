/*
  Autor: Luan J. Chaves

  Solucao do problema "jantar dos filosofos"

  compilar: gcc -o filosofos filosofos.c -lpthread
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5   /*quantidade de filosofos que sera criado*/
#define ESQUERDA (i+N-1)%N
#define DIREITA (i+1)%N
#define PENSANDO 0
#define FAMINTO 1
#define COMENDO 2

/* variaveis globais */
int state[N], i, int_rand;
float float_rand;

/* protótipos */
void mostrar();
void *filosofo(void *j);
void pegar_hashi(int i);
void devolver_hashi(int i);
void teste(int i);
void pensar(int i);
void comer(int i);

/*semaforos*/
sem_t mutex;
sem_t sem_fil[N];

void main(){
  for(i=0; i<N; i++){
    state[i]=PENSANDO;
  }
  mostrar();
  int res;
  pthread_t thread[N];
  void *thread_result;

  /*inicia os semaforos*/
  res = sem_init(&mutex,0,1);
  for(i=0; i<N; i++){
    res = sem_init(&sem_fil[i],0,0);
  }

  /*cria as threads(filosofos)*/
  for(i=0; i<N; i++){
    res = pthread_create(&thread[i],NULL,filosofo,&i);
  }

  /*faz um join nas threads*/
  for(i=0; i<N; i++){
    res = pthread_join(thread[i],&thread_result);
  }
}

/*funcao que ira mostrar o estado de cada filosofo*/
void mostrar(){
   for(i=1; i<=N; i++){
       if(state[i-1] == PENSANDO){
          printf("O filosofo %d esta pensando!\n", i);
       }
       if(state[i-1] == FAMINTO){
          printf("O filosofo %d esta com fome!\n", i);
       }
       if(state[i-1] == COMENDO){
          printf("O filosofo %d esta comendo!\n", i);
       }
   }
   printf("\n");
}

/*comportamento do filosofo*/
void *filosofo(void *j){
  int i=*(int*)j;
  while(1){
    pensar(i);
    pegar_hashi(i);
    comer(i);
    devolver_hashi(i);
  }
}

/*acao de pegar o hashi*/
void pegar_hashi(int i){
  sem_wait(&mutex);
  state[i]=FAMINTO;
  mostrar();
  teste(i);
  sem_post(&mutex);
  sem_wait(&sem_fil[i]);
}

/*acao de devolver o hashi*/
void devolver_hashi(int i){
  sem_wait(&mutex);
  state[i]=PENSANDO;
  mostrar();
  teste(ESQUERDA);
  teste(DIREITA);
  sem_post(&mutex);
}

/*funcao que testa se o filosofo podera comer*/
void teste(int i){
  if((state[i]==FAMINTO) && (state[ESQUERDA]!=COMENDO) && (state[DIREITA]!=COMENDO)){
    state[i]=COMENDO;
    mostrar();
    sem_post(&sem_fil[i]);
  }
}

/*o filosofo passara um tempo aleatorio pensando ou comendo*/
void pensar(int i){
  float_rand=0.001*random();
  int_rand=float_rand;
  usleep(int_rand);
}

/*acao de comer*/
void comer(int i){
  float_rand=0.001*random();
  int_rand=float_rand;
  usleep(int_rand);
}
