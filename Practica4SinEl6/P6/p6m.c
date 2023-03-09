#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <semaphore.h>

//Semaforos ---------

sem_t sem_lleno;
sem_t sem_escribir;
sem_t sem_cola;
sem_t sem_paso_e[20];
sem_t sem_paso_l[20];
sem_t sem_salir_e[20];
sem_t sem_salir_l[20];

//-------------------

//se necesitan 2 variables una de cuantos lectores hay y otra de si se está escribiendo o no
// el unico lector que pelea es el primero el ultimo es el que libera
// las variables hay que protejerlas con lectores
int ocupaciones;

void * lectores(int * numero){

  int num=(int)numero;
  while(1){
    sem_wait(&sem_escribir);
    while(){
      printf("[lector %d] -> Esperando a intenta leer...\n",num);
      sem_wait(&sem_paso_l[num]);
      printf("[lector %d] -> Intentando leer...\n",num);
      sem_wait(&sem_lleno);
      ocupaciones++;
      printf("[lector %d] -> Leyendo...\n",num);
      sem_wait(&sem_salir_l[num]);
      printf("[lector %d] -> Fin lectura\n",num);
      ocupaciones--;
      sem_post(&sem_lleno);
    }
    sem_post(&sem_escribir);
  }
  pthread_exit(NULL);
  return 0;
}

void * escritores(int * numero){

  int num=(int)numero;
  while(1){
    printf("[escritor %d] -> Esperando a intenta escribir...\n",num);
    sem_wait(&sem_paso_e[num]);
    //printf("OCUPACIONES: %d\n",ocupaciones);
    while(ocupaciones!=0);
    printf("[escritor %d] -> Intentando escribir...\n",num);
    sem_wait(&sem_escribir);
    printf("[escritor %d] -> Escribiendo...\n",num);
    sem_wait(&sem_salir_e[num]);
    printf("[escritor %d] -> Fin escritura\n",num);
    sem_post(&sem_escribir);
  }
  pthread_exit(NULL);
  return 0;
}


int main (int argc,char *argv[]){

  int i;
  int N1;//numero total de lectores
  int N2;//numero maximo de lectores simultaneos
  int N3;//numero maximo de escritores
  ocupaciones=0;

  if(argc!=4){
    printf("Hacen falta 3 parametros\n");
    exit(0);
  }
  else
    {
      if (atoi(argv[1])<atoi(argv[2])){
	printf("El numero de lectores debe ser mayor que el numero maximo de lectores simultaneos\n");
	exit(0);
      }
    }

  N1=atoi(argv[1]);
  N2=atoi(argv[2]);
  N3=atoi(argv[3]);

  pthread_t mythread[argc];
  sem_init(&sem_lleno,0,N2);
  sem_init(&sem_escribir,0,1);

  for(i=0;i<N1;i++){ //Lectores
   int valor = i;
   sem_init(&sem_paso_l[i],0,0);
   sem_init(&sem_salir_l[i],0,0);
   pthread_create(&(mythread[i]),NULL,(void *)lectores,(void *)(valor+1));
  }
  for(i=0;i<N3;i++){ //Escritores
    int valor = i;
    sem_init(&sem_paso_e[i],0,0);
    sem_init(&sem_salir_e[i],0,0);
    pthread_create(&(mythread[i]),NULL,(void *)escritores,(void *)(valor+1));
  }


  while(1){

    printf("1 -> Intentar leer\n");
    printf("2 -> Finalizar leer\n");
    printf("3 -> Intentar escribir\n");
    printf("4 -> Finalizar escribir\n");
    printf("5 -> Salir\n");
    printf("");

    int sel=0;
    int sel2=0;

    scanf("%d",&sel);

    switch(sel)
      {
      case 1:
	printf("Introduce el numero del lector (del 1 al %d)\n",N1);
	printf("");
	scanf("%d",&sel2);
	sem_post(&sem_paso_l[sel2]);
	break;

      case 2:
	printf("Introduce el numero del lector (del 1 al %d)\n",N1);
	printf("");
	scanf("%d",&sel2);
	sem_post(&sem_salir_l[sel2]);
	break;

      case 3:
	printf("Introduce el numero del escritor (del 1 al %d)\n",N3);
	printf("");
	scanf("%d",&sel2);
	sem_post(&sem_paso_e[sel2]);
	break;

      case 4:
	printf("Introduce el numero del escritor (del 1 al %d)\n",N3);
	printf("");
	scanf("%d",&sel2);
	sem_post(&sem_salir_e[sel2]);
	break;
      case 5:
	printf("----FIN----\n");
	exit(0);
	break;

      default:printf("Valor incorrecto\n");
	exit(0);
      }

  }
  
  return 0;
 
}