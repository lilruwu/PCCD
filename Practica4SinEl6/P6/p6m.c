#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

sem_t varE,EscStart[20],EscStop[20],LecStart[20],LecStop[20],semEsc,semLect,variable;;
int N1,N2,N3;
int lectores = 0;
int quiere=0,escribiendo=0;
void funcion_lector(int *num){
  int id = *num;
  int impr;

  while(1){

    if(impr==0){
      printf("\nEsperando a intentar leer... thread %d ",id);
      impr=1;
    }
    sem_wait(&LecStart[id]);
    printf("\nIntentando leer... thread %d ",id);
    sem_wait(&variable);
    lectores++;
    if(lectores == 1){
      sem_wait(&semEsc);
      sem_wait(&varE);
      if(quiere!=0){
	sem_post(&semEsc);
	while(quiere!=0){
	  sem_post(&varE);
	  sem_wait(&varE);
	}
	sem_wait(&semEsc);
      }
      sem_post(&varE);
      sem_post(&variable);

    }else{
      sem_post(&variable);
    }
    sem_wait(&semLect);
    printf("\nLeyendo ...thread %d",id);
    sem_wait(&LecStop[id]);
    printf("\nFin lectura thread %d.",id);
    impr=0;
    sem_post(&semLect);
    sem_wait(&variable);
    lectores --;
    if(lectores==0){
      sem_post(&semEsc);
      sem_post(&variable);
    }else{
      sem_post(&variable);
    }
  }
}
   
void funcion_escritor(int *num){
  int id = *num;
  int impr=0;

  while(1){
    if(impr==0){
      printf("\nEsperando a intentar escribir... thread %d ",id);
      impr=1;
    }
    sem_wait(&EscStart[id]);
    sem_wait(&varE);
    quiere++;
    sem_post(&varE);
    printf("\nIntentando escribir... thread %d",id);
    sem_wait(&semEsc);
    sem_wait(&varE);
    quiere--;
    sem_post(&varE);
    printf("\nEscribiendo ...thread %d",id);
    sem_wait(&EscStop[id]);
    printf("\nFin escritura thread %d.",id);
    impr=0;
    sem_post(&semEsc);
  }
}


int main(int argc,char *argv[]){

  if(argc != 4){
    printf("\nDebe introducir los valores N1, N2 y N3.\n");
    return 0;
  }

  N1=atoi(argv[1]);
  N2=atoi(argv[2]);
  N3=atoi(argv[3]);

  char cadena[10];
  int opcion,i;
  pthread_t idlect[N1],idEsc[N3];
  int idl[N1],ide[N3];
  int lector,escritor;

  //Creación de THREADS                                                                                                                                                                                                                                                    

  for(i=0;i<N1;i++){

    pthread_create(&idlect[i],NULL,(void*)funcion_lector,(void*)&idl[i]);
    idl[i]=i;
    sem_init(&LecStart[i],0,1);
    sem_init(&LecStop[i],0,1);
    sem_wait(&LecStop[i]);
    sem_wait(&LecStart[i]);

  }

  for(i=0;i<N3;i++){
    pthread_create(&idEsc[i],NULL,(void*)funcion_escritor,(void*)&ide[i]);
    ide[i]=i;
    sem_init(&EscStart[i],0,1);
    sem_init(&EscStop[i],0,1);
    sem_wait(&EscStop[i]);
    sem_wait(&EscStart[i]);

  }
  //Iniciación de semaforos

  sem_init(&semEsc,0,1);
  sem_init(&semLect,0,N2);
  sem_init(&variable,0,1);
  sem_init(&varE,0,1);
  while(1){
    printf("\n1.Intentar leer");
    printf("\n2.Finalizar leer");
    printf("\n3.Intentar escribir");
    printf("\n4.Finalizar escribir.");
    printf("\n5.Salir.\n");
    fgets(cadena,10,stdin);
    opcion=atoi(cadena);
    switch(opcion){
    case 1:
      printf("\n>introduzca el número del lector de 1 a %d",N1);
      fgets(cadena,10,stdin);
      lector=atoi(cadena);
      sem_post(&LecStart[lector-1]);
      break;
    case 2:
      printf("\n>introduzca el número del lector de 1 a %d",N1);
      fgets(cadena,10,stdin);
      lector=atoi(cadena);
      sem_post(&LecStop[lector-1]);
      break;
    case 3:
      printf("\n>introduzca el número del escritor de 1 a %d",N3);
      fgets(cadena,10,stdin);
      escritor=atoi(cadena);
      sem_post(&EscStart[escritor-1]);
      break;
    case 4:
      printf("\n>introduzca el número del escritor de 1 a %d",N3);
      fgets(cadena,10,stdin);
      escritor=atoi(cadena);
      sem_post(&EscStop[escritor-1]);
      break;
    case 5:
      return 0;
    default:
      break;
    }

  }
  return 0;
}  









