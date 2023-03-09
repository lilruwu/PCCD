#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

void *rutina(int *);

sem_t semaforo_general;
sem_t semaforo_leer[10];
sem_t semaforo_salir[10];

int main(int argc, char *argv[]){
        int i;
        int N1 = atoi(argv[1]);
        int N2 = atoi(argv[2]);
        pthread_t thread[N1];
	sem_init(&semaforo_general, 0, N2);
        for(i = 0; i < N1; i++){
                if(pthread_create(&thread[i], NULL, (void *)rutina, (void *)i+1)!=0){
                        printf("No se ha podido crear el hilo\n");
                        return -1;
                }else{
			sem_init(&semaforo_leer[i], 0, 0);
			sem_init(&semaforo_salir[i], 0, 0);			
		}
        }

	while(1){
		printf("1. Intentar leer\n");
		printf("2. Finalizar leer\n");
		printf("3. Salir\n");
		int opcion;
		scanf("%i", &opcion);
		int lector;
		switch(opcion){
			case 1:
				printf("Introduzca el numero del lector (del 1 al %i)\n", N1);
				scanf("%i", &lector);
				sem_post(&semaforo_leer[lector]);
				break;
			case 2:
				printf("Introduzca el numero del lector (del 1 al %i)\n", N1);
				scanf("%i", &lector);
				sem_post(&semaforo_salir[lector]);
				break;
			case 3:
				return 0;
		}
	}
}

void *rutina(int *num){
	while(1){
		printf("[Lector %i] -> Esperando a intentar leer...\n", (int) num);
		sem_wait(&semaforo_leer[(int)num]);
		printf("[Lector %i] -> Intentando leer...\n", (int) num);
		sem_wait(&semaforo_general);
		printf("[Lector %i] -> Leyendo...\n", (int) num);
		sem_wait(&semaforo_salir[(int)num]);
		printf("[Lector %i] -> Fin lectura\n", (int) num);
		sem_post(&semaforo_general);
        }
	pthread_exit(NULL);
}
