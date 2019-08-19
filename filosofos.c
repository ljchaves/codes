#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define QUANT 5 //Quantidade de filósofos
#define ESQUERDA (id_filosofo+QUANT-1)%QUANT //Id do filósofo a esquerda do id
#define DIREITA (id_filosofo+1)%QUANT //Id do filósofo a direita do id
#define PENSANDO 0 //Id para estado pensado
#define FAMINTO 1 //Id para estado de fome
#define COMENDO 2 //Id para estado comendo
#define TRUE 1  

int estado [QUANT]; //Estado dos filósofos
pthread_mutex_t mutex; //Região crítica
pthread_mutex_t mux_filo [QUANT]; //Mutex por filósofo
pthread_t jantar[QUANT]; //Todos os filósofos

void *filosofo(void *param);
void pegar_hashi(int id_filosofo);
void devolver_hashi(int id_filosofo);
void intencao(int id_filosofo);
void comer(int id_filosofo);
void pensar(int id_filosofo);


int main(void){
   int cont;//Contador auxiliar
   int status;//Criação da thread

   pthread_mutex_init(&(mutex), NULL);//Inicia os mutexes
   for(cont=0; cont<QUANT; cont++){
      pthread_mutex_init(&(mux_filo[cont]), NULL);
   }

   for(cont=0; cont<QUANT; cont++){ //Inicia threads (filósofos)
      status=pthread_create(&(jantar[cont]), NULL, filosofo, (void *) &(cont)); //verifica se ocorreu erro
      if(status){
         printf("Erro criando thread %d, retornou codigo %d\n", cont, status);
         return EXIT_FAILURE;
      }
   }

   pthread_mutex_destroy(&(mutex)); //Destroi antes de sair
   for(cont=0; cont<QUANT; cont++){
      pthread_mutex_destroy(&(mux_filo[cont]));
   }
   pthread_exit(NULL);

   return EXIT_SUCCESS;
}

void *filosofo(void *vparam)
{
   int *id=(int*)(vparam);//Repassa o id do filósofo
   printf("Filosofo %d foi criado com sucesso\n", *(id) );
   while (TRUE)
   {
      pensar(*(id));//Aguarda o filósofo pensar
      pegar_hashi(*(id));//Filósofo pega os hashis
      comer(*(id));//Aguarda comer
      devolver_hashi(*(id));//Devolver os hashis pra mesa
   }
   pthread_exit((void*)0);//Legado do retorno
}

void pegar_hashi(int id_filosofo){
   pthread_mutex_lock(&(mutex));//Entra na regiсo crítica
   printf("Filosofo %d esta faminto\n", id_filosofo);
   estado[id_filosofo]=FAMINTO;//Altera o estado do filósofo
   intencao(id_filosofo);//Intenção de pegar os hashis
   pthread_mutex_unlock(&(mutex));//Sai na região crítica
   pthread_mutex_lock(&(mux_filo[id_filosofo]));//Bloqueia os hashis
}

void devolver_hashi (int id_filosofo){
   pthread_mutex_lock(&(mutex));//Entra na regiсo crítica
   printf("Filosofo %d esta pensando\n", id_filosofo);
   estado[id_filosofo]=PENSANDO;//Terminou de comer
   intencao(ESQUERDA);//Vê se o vizinho da esquerda pode comer agora
   intencao(DIREITA);//Vê se o vizinho da direita pode comer agora
   pthread_mutex_unlock(&(mutex));//Sai da regiсo crítica
}

void intencao ( int id_filosofo ){
   printf("Filosofo %d esta com intencao de comer\n", id_filosofo);\
   //Se o filosofo esta come fome e se o vizinho da esquerda nao esta comendo e e o vizinho da direita nao esta comendo
   if((estado[id_filosofo]==FAMINTO) && (estado[ESQUERDA]!=COMENDO) && (estado[DIREITA]!=COMENDO)){
      printf("Filosofo %d ganhou a vez de comer\n", id_filosofo);
      estado[id_filosofo]=COMENDO;//Filósofo ganho a vez de comer
      pthread_mutex_unlock(&(mux_filo[id_filosofo]));//Libera os hashis
   }
}

void pensar(int id_filosofo){
   int r=(rand()%10+1);
   printf("Filosofo %d pensa por %d segundos\n", id_filosofo, r);
   sleep(r);//Gasta um tempo em segundos
}

void comer(int id_filosofo){
   int r = (rand() % 10 + 1);
   printf("Filosofo %d come por %d segundos\n", id_filosofo, r);
   sleep(r);//Gasta um tempo em segundos
}
