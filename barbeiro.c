#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define CADEIRAS 20/*número de cadeiras para os clientes à espera */
#define TRUE 1

sem_t clientes;/* número de cliente à espera de atendimento */
sem_t barbeiros;/* número de barbeiros à espera de clientes */
sem_t mutex;/* para exclusão mútua */
int esperando = 0;/* clientes que estão esperando (não estão cortando) */

/* protótipos */
void* barbeiro(void *arg);
void* cliente(void *arg);

int main() {
  sem_init(&clientes, TRUE, 0);
  sem_init(&barbeiros, TRUE, 0);
  sem_init(&mutex, TRUE, 1);
  pthread_t b, c;
  pthread_create(&b, NULL, (void *) barbeiro, NULL);/*criando único barbeiro */
  while(TRUE){/*criação indefinida de clientes */
    pthread_create(&c, NULL, (void *) cliente, NULL);
    sleep(1);
  }
  return 0;
}

void* barbeiro(void *arg){
  while(TRUE){
    sem_wait(&clientes);/*vai dormir se o número de clientes for 0 */
    sem_wait(&mutex);/*obtém acesso a 'esperando' */
    esperando = esperando-1;/*descresce de um o contador de clientes à espera */
    sem_post(&barbeiros);/*um barbeiro está agora pronto para cortar cabelo */
    sem_post(&mutex);/*libera 'esperando' */
    printf("Barbeiro esta cortando o cabelo de alguem!\n");/*corta o cabelo (fora da região crítica) */
    sleep(3);
  }
  pthread_exit(NULL);
}

void* cliente(void *arg){
  sem_wait(&mutex);/*entra na região crítica */
  if(esperando < CADEIRAS){/*se não houver cadeiras vazias, saia */
    printf("Cliente chegou para cortar o cabelo!\n");
    esperando = esperando+1;/*incrementa o contador de clientes à espera */
    sem_post(&clientes);/*acorda o barbeiro se necessário */
    sem_post(&mutex);/*libera o acesso a 'esperando' */
    sem_wait(&barbeiros);/*vai dormir se o número de barbeiros livres for 0 */
    printf("Cliente esta cortando o cabelo!\n");/*sentado e sendo servido */
  }else{
    sem_post(&mutex);/*a barbearia está cheia; não espera */
    printf("Cliente desistiu! (O salao esta muito cheio!)\n");
  }
  pthread_exit(NULL);
}
