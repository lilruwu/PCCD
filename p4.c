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
sem_t sem_paso[20];
sem_t sem_salir[20];

//-------------------


void * lectores(int * numero){

  int num=(int)numero;
  while(1){
    printf("[lector %d] -> Esperando a intenta leer...\n",num);
    sem_wait(&sem_paso[num]);
    printf("[lector %d] -> Intentando leer...\n",num);
    sem_wait(&sem_lleno);
    printf("[lector %d] -> Leyendo...\n",num);
    sem_wait(&sem_salir[num]);
    printf("[lector %d] -> Fin lectura\n",num);
    sem_post(&sem_lleno);
  }
  pthread_exit(NULL);
  return 0;

}


int main (int argc,char *argv[]){

  int i;
  int N1;//numero total de lectores
  int N2;//numero maximo de lectores simultaneos


  if(argc!=3){
    printf("Hacen falta 2 parametros\n");
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

  pthread_t mythread[argc];
  sem_init(&sem_lleno,0,N2); //Este semáforo controlará el número de lectores simultáneos

  for(i=0;i<N1;i++){
   int valor = i;
   sem_init(&sem_paso[i],0,0);//Semáforos NO NOMBRADOS: solo pueden ser usados por un proceso y sus hijos (primer 0). Segundo 0 valor inicial semáforo
   sem_init(&sem_salir[i],0,0);
   pthread_create(&(mythread[i]),NULL,(void *)lectores,(void *)(valor+1));
}


  while(1){

    printf("1 -> Intentar leer\n");
    printf("2 -> Finalizar leer\n");
    printf("3 -> Salir\n");
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
	sem_post(&sem_paso[sel2]);
	break;

      case 2:
	printf("Introduce el numero del lector (del 1 al %d)\n",N1);
	printf("");
	scanf("%d",&sel2);
	sem_post(&sem_salir[sel2]);
	break;
      case 3:
	printf("----FIN----\n");
	exit(0);
	break;
      default:printf("Valor incorrecto\n");
	exit(0);
      }

  }
  
  return 0;
 
}
